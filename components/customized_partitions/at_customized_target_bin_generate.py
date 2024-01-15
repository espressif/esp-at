#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import sys
import stat
import argparse
import subprocess
import re

def main():
    """ main """
    parser = argparse.ArgumentParser()
    parser.add_argument('--project_dir', default='.', help='project path')
    parser.add_argument('--tools_dir', default='.', help='the tools directory')
    parser.add_argument('--output_dir', default='output', help='the output bin directory')
    parser.add_argument('--flash_args_file', default='flash_args_file', help='the file to store flash args')
    args = parser.parse_args()

    project_dir = args.project_dir.strip()
    tools_dir = args.tools_dir.strip()
    output_dir = args.output_dir.strip()
    flash_args_file = args.flash_args_file.strip()

    if os.path.exists(output_dir) == False:
        os.mkdir(output_dir)

    with open(flash_args_file, 'r') as args_file:
        for line in args_file.readlines():
            line_str = line.strip()
            line_str = re.sub(' +', ' ', line_str)
            if line_str != '':
                str_list = line_str.split(' ')
                full_filename = str_list[1]
                file_size = str_list[2]
                file_name = os.path.basename(full_filename)
                partition_name = os.path.splitext(file_name)[0]
                tool_name = os.path.join(tools_dir, ''.join([partition_name, '.py']))
                if not os.access(tool_name, os.X_OK):
                    os.chmod(tool_name, stat.S_IRUSR | stat.S_IXUSR)
                ret = subprocess.call([sys.executable, tool_name, '--partition_name', partition_name, '--partition_size', file_size, '--outdir', output_dir, '--project_path', project_dir], shell = False)

if __name__ == '__main__':
    main()
