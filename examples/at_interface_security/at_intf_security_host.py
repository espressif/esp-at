#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import sys
import serial
import threading
import time
import signal
from datetime import datetime
from Crypto.Cipher import AES
from Crypto.Util import Counter

# color output on windows
if sys.platform == 'win32':
    from colorama import init

# Please modify the following configurations according to your needs
#----------------------------------------------------------------------------------------------------------------------
# AT log port
at_enable_log_port = True           # Disable it if you don't want to open AT log port
at_log_port_dev = '/dev/ttyUSB0'    # AT log port device name. You may need to set it 'COMx' on Windows.

# AT command port
at_enable_cmd_port = True           # Disable it if you don't want to open AT command port
at_cmd_port_dev = '/dev/ttyUSB1'    # AT command port device name. You may need to set it 'COMx' on Windows.

# Local file
at_enable_local_log = True          # Disable it if you don't want to save logs to a local file

# AT interface security
at_enable_intf_security = True      # Disable it if you don't want to establish security channel
at_tx_key = b'A' * 16               # The default key is 'A' * 16. You should modify it to the same one of the AT rx.
at_tx_iv = b'T' * 16                # The default IV is 'T' * 16. You should modify it to the same one of the AT rx.
at_rx_key = b'A' * 16               # The default key is 'A' * 16. You should modify it to the same one of the AT tx.
at_rx_iv = b'T' * 16                # The default IV is 'T' * 16. You should modify it to the same one of the AT tx.

# Wi-Fi configurations
at_cwjap_ssid = '688018'            # Wi-Fi SSID
at_cwjap_passwd = ''                # Wi-Fi password
#----------------------------------------------------------------------------------------------------------------------

# More optional configurations that you can modify
at_log_port_baud = 115200           # AT log port baudrate
at_log_port_timeout = 1             # AT log port timeout
at_log_port_flowcontrol = False     # AT log port flowcontrol
at_cmd_port_baud = 115200           # AT command port baudrate
at_cmd_port_timeout = 1             # AT command port timeout
at_cmd_port_flowcontrol = False     # AT command port flowcontrol
at_cmd_timeout = 15                 # AT command timeout

# You should not modify the following variables
at_exit_flag = False                # Exit flag
at_log_handle = None                # Write the data to a local file by this variable
at_log_path = None                  # Log file path
at_log_port_thread_handle = None    # AT log port thread handle
at_log_port = None                  # Read the data from AT log port by this variable
at_cmd_port_thread_handle = None    # AT command port thread handle
at_cmd_port = None                  # Read and write the data from AT command port by this variable
at_tx_cipher = None                 # Encrypt the outgoing data by this variable
at_rx_cipher = None                 # Decrypt the incoming data by this variable

def at_intf_security_test():
    ESP_LOGN('AT interface security test...')
    at_cmd_ok('AT')
    at_cmd_ok('AT+GMR')
    at_cmd_ok('AT+CWMODE=1')
    ret = at_cmd_check_ret(f'AT+CWJAP="{at_cwjap_ssid}","{at_cwjap_passwd}"', 'GOT IP')
    if not ret:
        ESP_LOGW(f'Please make sure the wifi ssid ({at_cwjap_ssid}) and password ({at_cwjap_passwd}) are correct!')
    ESP_LOGN('AT interface security test success!')

def at_intf_security_init():
    if at_enable_intf_security:
        # tx cipher
        global at_tx_cipher
        tx_counter = Counter.new(128, initial_value=int.from_bytes(at_tx_iv, byteorder='little'))
        at_tx_cipher = AES.new(at_tx_key, AES.MODE_CTR, counter=tx_counter)

        # rx cipher
        global at_rx_cipher
        rx_counter = Counter.new(128, initial_value=int.from_bytes(at_rx_iv, byteorder='little'))
        at_rx_cipher = AES.new(at_rx_key, AES.MODE_CTR, counter=rx_counter)

# color output with newline
def ESP_LOGI(x, local = True):
    print(f'\033[32m{x}\033[0m')
    if local and at_enable_local_log:
        at_log_handle.write(f'{x}\n')

def ESP_LOGE(x, local = True):
    print(f'\033[31m{x}\033[0m')
    if local and at_enable_local_log:
        at_log_handle.write(f'{x}\n')

def ESP_LOGW(x, local = True):
    print(f'\033[33m{x}\033[0m')
    if local and at_enable_local_log:
        at_log_handle.write(f'{x}\n')

def ESP_LOGN(x, local = True):
    print(f'{x}')
    if local and at_enable_local_log:
        at_log_handle.write(f'{x}\n')

# color output without newline
def ESP_LOGI0(x, local = True):
    print(f'\033[32m{x}\033[0m')
    if local and at_enable_local_log:
        at_log_handle.write(f'{x}')

def ESP_LOGN0(x, local = True):
    print(f'{x}', end='')
    if local and at_enable_local_log:
        at_log_handle.write(f'{x}')

def mkdir(path):
    path=path.strip()
    path=path.rstrip('\\')
    isExists=os.path.exists(path)
    if not isExists:
        os.makedirs(path)

def at_open_log():
    global at_log_path
    logdir = os.getcwd() + '/at_log'
    mkdir(logdir)
    filename = datetime.now().strftime('%Y%m%d_%H%M%S.log')
    at_log_path = logdir + '/' + filename
    return open(at_log_path, 'w+')

def at_cmd_port_write(data):
    if at_tx_cipher:
        ESP_LOGN0(f'[{datetime.now()}] intf-tx: {data}')
    else:
        ESP_LOGI0(f'[{datetime.now()}] intf-tx: {data}')
    data = data.encode()
    if at_tx_cipher:
        data = at_tx_cipher.encrypt(data)
        ESP_LOGI(f'[{datetime.now()}] intf-sec-tx: ' + ' '.join(f'{byte:02x}' for byte in data))
    at_cmd_port.write(data)

def at_cmd_port_read():
    global at_cmd_port
    if at_cmd_port.in_waiting:
        data = at_cmd_port.read(at_cmd_port.in_waiting)
        if at_rx_cipher:
            ESP_LOGI(f'[{datetime.now()}] intf-sec-rx: ' + ' '.join(f'{byte:02x}' for byte in data))
            data = at_rx_cipher.decrypt(data)
        data = data.decode('utf-8', 'ignore')
        if at_rx_cipher:
            ESP_LOGN0(f'[{datetime.now()}] intf-rx: {data}')
        else:
            ESP_LOGI0(f'[{datetime.now()}] intf-rx: {data}')
        return data
    return None

def at_log_port_read():
    global at_log_port
    if at_log_port.in_waiting:
        data = at_log_port.readline()
        data = data.decode('utf-8', 'ignore')
        ESP_LOGN0(f'[{datetime.now()}] {data}')
        return data
    return None

def at_cmd_check_ret(cmd, check_str, cmd_timeout = at_cmd_timeout, cmd_tail = '\r\n'):
    # read out all the data before sending AT cmd
    at_cmd_port_read()

    # send AT cmd
    to_send_cmd = cmd + cmd_tail
    at_cmd_port_write(to_send_cmd)

    # receive expected data from AT cmd port
    start_time = datetime.now()
    all_data = ''
    while True:
        data = at_cmd_port_read()
        if data:
            all_data += data
            if check_str in all_data:
                return True
            elif 'ERROR' in all_data:
                ESP_LOGE(f'[{datetime.now()}] {cmd} returns ERROR')
                return False
        if (datetime.now() - start_time).seconds > cmd_timeout:
            break
        if at_exit_flag:
            return False

    ESP_LOGW(f'[{datetime.now()}] {cmd} cmd timeout ({(datetime.now() - start_time).seconds}s)')
    return False

def at_cmd_ok(cmd):
    return at_cmd_check_ret(cmd, 'OK\r\n')

def at_test_set_up():
    while True:
        # always do the interface security init if AT is not ready
        at_intf_security_init()

        ESP_LOGN('Waiting for AT ready... (You may need to reset or power on the ESP module)')
        data = at_cmd_port_read()
        if data:
            if 'ready' in data:
                return True
        if at_exit_flag:
            return False
        time.sleep(3)

def at_cmd_port_thread():
    global at_cmd_port
    global at_exit_flag
    try:
        at_cmd_port = serial.Serial(port=at_cmd_port_dev, baudrate=at_cmd_port_baud, timeout=at_cmd_port_timeout, rtscts=at_cmd_port_flowcontrol)
        ESP_LOGI(f'Open AT command port success on {at_cmd_port_dev}')
    except Exception as e:
        ESP_LOGE(f'{e}')
        at_exit_flag = True
        return

    # set up the test environment
    ret = at_test_set_up()
    if not ret:
        at_exit_flag = True
        ESP_LOGE('AT test set up failed, exit now...')
        return

    # do the interface security test
    at_intf_security_test()

    at_exit_flag = True
    ESP_LOGN('at_cmd_port_thread exited.')

def at_log_port_thread():
    global at_log_port
    global at_exit_flag
    try:
        at_log_port = serial.Serial(port=at_log_port_dev, baudrate=at_log_port_baud, timeout=at_log_port_timeout, rtscts=at_log_port_flowcontrol)
        ESP_LOGI(f'Open AT log port success on {at_log_port_dev}')
    except Exception as e:
        ESP_LOGE(f'{e}')
        at_exit_flag = True
        return

    while True:
        at_log_port_read()
        if at_exit_flag:
            break
    pass
    ESP_LOGN('at_log_port_thread exited.')

def at_exit_deinit():
    global at_cmd_port
    global at_log_port
    global at_log_handle

    if at_cmd_port:
        at_cmd_port_read()
        at_cmd_port.close()
    if at_log_port:
        at_log_port_read()
        at_log_port.close()
    if at_log_handle:
        at_log_handle.flush()
        at_log_handle.close()
        ESP_LOGN(f'\r\nAll done! You can check the log:\r\n\r\n  {at_log_path}\r\n', False)

def signal_handler(sig, frame):
    global at_exit_flag
    at_exit_flag = True
    ESP_LOGI('Ctrl+C pressed, exit now...')

def main():
    global at_log_handle
    global at_log_port_thread_handle
    global at_cmd_port_thread_handle

    signal.signal(signal.SIGINT, signal_handler)

    if at_enable_local_log:
        at_log_handle = at_open_log()
    if at_enable_log_port:
        at_log_port_thread_handle = threading.Thread(target = at_log_port_thread)
        at_log_port_thread_handle.start()
    if at_enable_cmd_port:
        at_cmd_port_thread_handle = threading.Thread(target = at_cmd_port_thread)
        at_cmd_port_thread_handle.start()

    # wait for the threads exit
    if at_cmd_port_thread_handle:
        at_cmd_port_thread_handle.join()
    if at_log_port_thread_handle:
        at_log_port_thread_handle.join()

    at_exit_deinit()

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        ESP_LOGE(f'Exception: {e}')
        at_exit_deinit()
        sys.exit(1)
