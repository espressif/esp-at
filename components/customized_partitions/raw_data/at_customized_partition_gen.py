#
# ESPRESSIF MIT License
#
# Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
#
import os
import sys
import stat
import argparse
import subprocess

def main():
    """ main """
    parser = argparse.ArgumentParser()
    parser.add_argument("--tools_dir", default=".", help="the tools directory")
    parser.add_argument("--output_dir", default="output", help="the output bin directory")
    parser.add_argument("--flash_args_file", default="flash_args_file", help="the file to store flash args")
    args = parser.parse_args()

    tools_dir = args.tools_dir
    output_dir = args.output_dir
    flash_args_file = args.flash_args_file

    if os.path.exists(output_dir) == False:
        os.mkdir(output_dir)

    with open(flash_args_file, 'r') as args_file:
        for line in args_file.readlines():
            line_str = line.strip()
            str_list = line_str.split(' ')
            full_filename = str_list[1]
            file_size = str_list[2]
            file_name = os.path.basename(full_filename)
            partition_name = os.path.splitext(file_name)[0]
            tool_name = os.path.join(tools_dir, ''.join([partition_name, '.sh']))
            if not os.access(tool_name, os.X_OK):
                os.chmod(tool_name, stat.S_IRUSR | stat.S_IXUSR)
            subprocess.call([tool_name, partition_name, output_dir, file_size], shell = False)

if __name__ == '__main__':
    main()