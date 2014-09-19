#!/usr/bin/env python

"""Generate NMP's website

Usage:
  generate_website.py <source_dir> <build_dir> [options]
  generate_website.py -h | --help

  <source_dir>  Path to the NMP's source directory.

Options:
  -h --help     Show this screen.
  --verbose     Verbose output.

  --serve       Starts a server to view the site locally.

"""
from docopt import docopt
import site_generator

if __name__ == '__main__':
    # Read cmd line arguments
    args = docopt(__doc__, version='NMP')

    src_dir_structure =  [ 'include', 'tests', 'site']
    args['src_dir_structure'] = src_dir_structure
    args['project_name'] = 'NMP'

    site_generator.generate_site(args)
