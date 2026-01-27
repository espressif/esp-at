#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import sys
import argparse
import subprocess

def read_sdkconfig(project_path):
    """Read sdkconfig file and return its content."""
    sdkconfig_path = os.path.join(project_path, 'sdkconfig')
    if not os.path.exists(sdkconfig_path):
        print(f'Error: sdkconfig not found at {sdkconfig_path}')
        return None
    with open(sdkconfig_path, 'r') as f:
        return f.read()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--partition_name', default='', help='partition name')
    parser.add_argument('--partition_size', default='', help='partition size')
    parser.add_argument('--outdir', default='', help='out dir')
    parser.add_argument('--project_path', default='', help='project path')
    args = parser.parse_args()

    # Read sdkconfig from project_path
    sdkconfig_content = read_sdkconfig(args.project_path)
    if sdkconfig_content is None:
        return 1

    # Get the directory where this script is located
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Build common arguments to pass to the target script
    cmd_args = [
        '--partition_name', args.partition_name,
        '--partition_size', args.partition_size,
        '--outdir', args.outdir,
        '--project_path', args.project_path
    ]

    # Check which filesystem is configured and call the corresponding script
    if 'CONFIG_AT_FS_FATFS=y' in sdkconfig_content or 'CONFIG_AT_FS_FATFS_LEGACY=y' in sdkconfig_content:
        target_script = os.path.join(script_dir, 'fatfs.py')
        return subprocess.call([sys.executable, target_script] + cmd_args)
    elif 'CONFIG_AT_FS_LITTLEFS=y' in sdkconfig_content:
        target_script = os.path.join(script_dir, 'littlefs.py')
        return subprocess.call([sys.executable, target_script] + cmd_args)
    else:
        print('Error: No supported filesystem configured in sdkconfig')
        return 1

if __name__ == '__main__':
    sys.exit(main())
