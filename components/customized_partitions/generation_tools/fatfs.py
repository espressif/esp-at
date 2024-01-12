#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import sys
import argparse
import subprocess

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
    partition_size = args.partition_size

    bin_size = 0
    multiple = 1
    end_index = len(partition_size)
    if partition_size.endswith('K') or partition_size.endswith('k'):
        multiple = 1024
        end_index = -1
    elif partition_size.endswith('M') or partition_size.endswith('m'):
        multiple = 1024 * 1024
        end_index = -1

    if partition_size[:end_index].isdigit() == True:
        bin_size = int(partition_size[:end_index]) * multiple
    else:
        print('{} size error'.format(partition_name))
        sys.exit(1)

    sector_size = 512
    sectors_per_cluster = 1
    fat_type = 0
    long_name_support = ''

    with open(os.path.join(project_path, 'sdkconfig')) as f:
        for line in f.readlines():
            line_str = line.strip()
            if not line_str.startswith('#'):
                index = line_str.find('CONFIG_WL_SECTOR_SIZE=')
                if index >= 0:
                    sector_size = int(line_str[index + len('CONFIG_WL_SECTOR_SIZE='):])
                    continue

                index = line_str.find('CONFIG_FATFS_SECTORS_PER_CLUSTER_')
                if index >= 0:
                    substr = line_str[index + len('CONFIG_FATFS_SECTORS_PER_CLUSTER_'):].split('=')
                    if substr[1] == 'y':
                        sectors_per_cluster = int(substr[0])
                    continue

                index = line_str.find('CONFIG_FATFS_LFN_HEAP=y')

                if index < 0:
                    index = line_str.find('CONFIG_FATFS_LFN_STACK=y')

                if index >= 0:
                    long_name_support = '--long_name_support'
                    continue

                index = line_str.find('CONFIG_FATFS_AUTO_TYPE=y')
                if index >= 0:
                    fat_type = 0
                    continue

                index = line_str.find('CONFIG_FATFS_FAT12=y')
                if index >= 0:
                    fat_type = 12
                    continue

                index = line_str.find('CONFIG_FATFS_FAT16=y')
                if index >= 0:
                    fat_type = 16
                    continue

    fatfs_param = '--sector_size {} {} --sectors_per_cluster {} --fat_type {}'.format(
            sector_size, long_name_support, sectors_per_cluster, fat_type)

    if sys.platform == 'win32':
        sys_python_path = sys.executable
    else:
        if os.environ.get('IDF_PYTHON_ENV_PATH') is None:
            sys_python_path = 'python'
        else:
            sys_python_path = os.path.join(os.environ.get('IDF_PYTHON_ENV_PATH'), 'bin', 'python')

    cmd = '{} {} {} --partition_size {} --output_file {} {}'.format(
        sys_python_path,
        os.path.join(project_path, 'esp-idf', 'components', 'fatfs', 'wl_fatfsgen.py'),
        os.path.join(project_path, 'components', 'fs_image'),
        bin_size,
        os.path.join(outdir, ''.join([partition_name, '.bin'])),
        fatfs_param)

    print('generating {}: {}'.format(''.join([partition_name, '.bin']), cmd))
    subprocess.call(cmd, shell = True)

if __name__ == '__main__':
    main()
