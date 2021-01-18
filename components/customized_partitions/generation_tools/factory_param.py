#
# ESPRESSIF MIT License
#
# Copyright (c) 2021 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
#
# Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
# it is free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished
# to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import os
import sys
import argparse
import subprocess
import json

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--partition_name', default='', help='partition name')
    parser.add_argument('--partition_size', default='', help='partition size')
    parser.add_argument('--outdir', default='', help='out dir')
    parser.add_argument('--project_path', default='', help='project path')
    args = parser.parse_args()

    partition_name = args.partition_name
    outdir = args.outdir
    project_path = args.project_path

    tool_path = os.path.join(project_path, 'tools', 'factory_param_generate.py')
    raw_data_path = os.path.join(project_path, 'components', 'customized_partitions', 'raw_data', partition_name)

    if os.path.exists(raw_data_path) == False:
        print('{} does not exist'.format(raw_data_path))
        return

    platform_name = ''
    module_name = ''

    with open(os.path.join(project_path, 'build', 'module_info.json')) as f:
        info = json.load(f)
        platform_name = info['platform']
        module_name = info['module']

    module_file = os.path.join(raw_data_path, 'factory_param_data.csv')
    define_file = os.path.join(raw_data_path, 'factory_param_type.csv')
    log_file = os.path.join(os.path.dirname(outdir), 'factory', 'factory_parameter.log')

    cmd = '{} {} --platform {} --module {} --bin_name {} --define_file {} --module_file {} --log_file {}'.format(sys.executable, 
                tool_path, 
                platform_name,
                module_name,
                os.path.join(outdir, ''.join([partition_name, '.bin'])),
                define_file,
                module_file,
                log_file)
    print('generating {}: {}'.format(''.join([partition_name, '.bin']), cmd))
    subprocess.call(cmd, shell = True)

if __name__ == '__main__':
    main()
