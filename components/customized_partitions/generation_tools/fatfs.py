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
            if not line_str.startswith("#"):
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

# deprecated fs compilation
def main_deprecated():
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

    cmd = 'make -C {} BUILD_DIR_BASE={}'.format(os.path.join(project_path, 'tools', 'mkfatfs'), os.path.join(project_path, 'build'))
    subprocess.call(cmd, shell = True)

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

    cmd = '{} -c {} -s {} {}'.format(os.path.join(project_path, 'build', 'mkfatfs', 'mkfatfs'), 
                os.path.join(project_path, 'components', 'fs_image'),
                bin_size,
                os.path.join(outdir, ''.join([partition_name, '.bin'])))

    print('generating {}: {}'.format(''.join([partition_name, '.bin']), cmd))
    subprocess.call(cmd, shell = True)

def has_fs_gen_tool():
    parser = argparse.ArgumentParser()
    parser.add_argument('--partition_name', default='', help='partition name')
    parser.add_argument('--partition_size', default='', help='partition size')
    parser.add_argument('--outdir', default='', help='out dir')
    parser.add_argument('--project_path', default='', help='project path')
    args = parser.parse_args()
    fs_gen_tool = os.path.join(args.project_path, 'esp-idf', 'components', 'fatfs', 'wl_fatfsgen.py')
    if os.path.exists(fs_gen_tool):
        print('Use {} to generate fatfs.bin'.format(fs_gen_tool))
        return True
    print('Use {} to generate fatfs.bin'.format(os.path.join(args.project_path, 'tools', 'mkfatfs')))
    return False

if __name__ == '__main__':
    if has_fs_gen_tool():
        main()
    else:
        main_deprecated()
