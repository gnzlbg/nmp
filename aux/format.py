#!/usr/bin/env python

"""Recursively formats C/C++ files using clang-format

Usage:
  format.py <path> [options]
  format.py -h | --help
  format.py --version

  <path>     Directory containing source files to format.

Options:
  -h --help  Show this screen.
  --version  Show version.
  --verbose  Verbose output.

  --check    Checks format (without applying it).

"""
from docopt import docopt
import os
import subprocess

file_extensions = ['.c', '.h', '.cpp', '.cc', '.cxx', 
                   '.hpp', '.hh', '.hxx', '.c++', '.h++']

style_file_rel_path = '/aux/.clang-format'


def fmt(file_path, style_file_path, check, check_diff_list, verbose):
    #   cf_cmd = 'clang-format -style=' + style_file_path
    cf_cmd = 'clang-format -style=file'
    if not check:
        cf_cmd = cf_cmd + ' -i'
    cf_cmd = cf_cmd + ' ' + file_path

    if check:
        cf_cmd = cf_cmd + ' | diff -u ' + file_path + ' - '

    if verbose:
        print(' - clang-format cmd: ' + cf_cmd)

    p = subprocess.Popen(cf_cmd, universal_newlines=False, shell=True,
                        stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    out, err = p.communicate('foo\nfoofoo\n')
    if verbose:
        print out
        print err
    if p.returncode == 0:
        return True
    else:
        check_diff_list.append((file_path, out, err))
        return False


if __name__ == '__main__':
    version = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD'])

    # Read cmd line arguments
    args = docopt(__doc__, version='NMP ' + version)
    verbose = args['--verbose']

    check_format = args['--check']
    src_path = args['<path>']
    style_file_path = '\"' + src_path + style_file_rel_path + '\"'

    if check_format:
        print('Checking that NMP is correctly formatted...')
    else:
        print('Formatting NMP...')

    if verbose:
        print(' - path: ' + args['<path>'])
        print(' - format style file: ' + style_file_path)

    # format files
    already_formated = True
    unformatted_files = []
    for subdir, _, files in os.walk(src_path):
        for f in files:
            for ext in file_extensions:
                if f.endswith(ext):
                    fi = subdir + '/' + f
                    fmt(fi, style_file_path, check_format, 
                       unformatted_files, verbose)

    if check_format:
        if not unformatted_files:
            print('...all files are formatted!')
            exit(0) # all files are already formatted
        else:
            if verbose:
                for i in unformatted_files:
                    print(i)
            print('... error: some files are not formatted!')
            exit(1) # some files are not formatted
    else:
        print('...all files have been formatted!')
        exit(0)
