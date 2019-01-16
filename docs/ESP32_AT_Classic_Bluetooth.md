# Overview
Classic bluetooth is Disabled by default. If you want to use classic bluetooth commands, you need to enable BT commands in menuconfig.

```
Component config -> AT -> [*] AT bt command support.
```

# Command Description

## initialization
There are two initialization-related commands. Firstly, initializing the bluetooth protocol stack, and then initializing the profile, such as:

```
AT+BTINIT=1      // init BT statck
AT+BTSPPINIT=2   // init SPP profile, the role is slave
```

## Basic parameters setting
After initialization, there are some basic parameter setting commands that may be need to be invoked.

### 1. device name
The default device name is `esp32`, If use command to set the device name, it will be stored in NVS.

```
AT+BTNAME="EXAMPLE"
```

### 2. scan mode
Sets whether it can be discovered and connected.

```
AT+BTSCANMODE=2    // both discoverable and connectable
```

### 3. security parameters
ESP32 supports both Simple pair and Legacy pair by default.

Using this command, you can set the IO capability, PIN type and PIN code of the device.

```
AT+BTSECPARAM=3,1,"9527"  // NO input NO output, fixed PIN code, 9527
```

If the PIN type is variable, the PIN code will be ignored.
If use the Simple pair encryption, the PIn code will be ignored.

## BT SPP EXAMPLE
### 1. PC CONNECTS TO ESP32
In this case, generally PC is master and ESP32 is slave. ESP32 needs to do this before the connection is established:

 - initialization
 
 ```
 AT+BTINIT=1     // init BT stack
 AT+BTSPPINIT=2  // init spp profile as shave
 AT+BTSPPSTART   // if role is client, this command is not required
 ```
 
 - parameters setting
 
 ```
 AT+BTNAME="EXAMPLE"       // set device name
 AT+BTSCANMODE=2           // discoverable and connectable
 AT+BTSECPARAM=3,1,"9527"  // NoInputNoOutput, fixed PIN code
 ```
 
At this point, the PC should be able to find the bluetooth device with name "EXAMPLE". If the PC initiates a connection and the connection succeed, ESP32 will print this log:

```
+BTSPPCONN:<conn index>,<remote_addr>
```
ESP32 can also initiate connections, before initiating a connection, please scan the surrounding devices at first:

```
// General inquiry mode, inquiry duration: 10, inquiry response: 10
AT+BTSTARTDISC=0,10,10
```
The scanning results are as follows:

```
+BTSTARTDISC:<bt_addr>,<dev_name>,<major_dev_class>,<minor_dev_class>,<major_srv_class>
```
Initiate the connection using the connection command：

```
// conn_index: 0, sec_mode: 0 -> No security, remote_address
AT+BTSPPCONN=0,0,"24:0a:c4:09:34:23"
```
After the connection is established, the data can be sent and received：

```
// conn_index: 0, data length: 30
AT+BTSPPSEND=0,30

>
OK
```
If you want to enter passthrough mode:

```
AT+BTSPPSEND
```
If you want to exit passthrough mode, you can input `+++`.

### ESP32 CONNECTS TO ESP32
If you use two ESP32 boards connected to each other，The process is basically the same as described above, The only difference is the initialization. the client initialization is as follow:

 ```
 AT+BTINIT=1     // init BT stack
 AT+BTSPPINIT=1  // init spp profile as master
 ```
All other steps are the same as described above.

## Encryption-related operation
If the IO capability is not NoInputNoOutput, the encryption process  will involve the exchange of key and PIN code.

If need to input the PIN code for Legacy Pair:

```
// conn_index, PIN code
AT+BTPINREPLY=0,"0000"
```

If need to input the Simple Pair Key:

```
// conn_index, Key
AT+BTKEYREPLY=0,123456
```
If ESP32 has the ability to output, You need to enter this password on the remote device：

```
+BTKEYNTF:0,123456
```
The ESP32 can also choose to accept or reject the encryption request from the remote device:

```
// conn_index, accept or reject
AT+BTSECCFM=0,1
```

There are also two commands to manage bonding devices:

```
//get the bonded devices list
AT+BTENCDEV?   
//remove a device from the security database list with a specific index.
AT+BTENCCLEAR=<enc_dev_index>
//remove all devices from the security database
AT+BLEENCCLEAR
```
