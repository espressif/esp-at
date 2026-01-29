#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
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

    # Parse partition size
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

    # Default littlefs configuration values
    obj_name_len = 64
    block_size = 4096

    # Read littlefs configuration from sdkconfig
    with open(os.path.join(project_path, 'sdkconfig')) as f:
        for line in f.readlines():
            line_str = line.strip()
            if not line_str.startswith('#'):
                index = line_str.find('CONFIG_LITTLEFS_OBJ_NAME_LEN=')
                if index >= 0:
                    obj_name_len = int(line_str[index + len('CONFIG_LITTLEFS_OBJ_NAME_LEN='):])
                    continue

    # Source directory for littlefs image
    base_dir = os.path.join(project_path, 'components', 'fs_image')

    # Output file path
    output_file = os.path.join(outdir, ''.join([partition_name, '.bin']))

    # Build the littlefs-python command
    # littlefs-python create <base_dir> <image_file> -v --fs-size=<size> --name-max=<name_max> --block-size=4096
    cmd = 'littlefs-python create {} {} -v --fs-size={} --name-max={} --block-size={}'.format(
        base_dir,
        output_file,
        bin_size,
        obj_name_len,
        block_size)

    print('generating {}: {}'.format(''.join([partition_name, '.bin']), cmd))
    ret = subprocess.call(cmd, shell=True)
    return ret

if __name__ == '__main__':
    sys.exit(main())
