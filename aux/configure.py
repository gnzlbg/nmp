#!/usr/bin/env python

"""Configures Hom3's build.

Usage:
  configure.py [<mode>] [options] [<build_path>]
  configure.py -h | --help
  configure.py --version

  <mode>                Build mode [default: release (simulation ready)].
  <build_path>          Build directory [default: repository/build].

Modes:
  asan                  Builds with Address Sanitizer.
  coverage              Builds with code-coverage analysis.
  debug                 Builds in optimized debug mode.
  debug-unopt           Builds in unoptimized debug mode.
  release               Builds in release mode.

Options:
  -h --help             Show this screen.
  --version             Show version.
  --verbose             Verbose output.

  --asan                With Address Sanitizer.
  --assertions          With assertions.
  --debug               Debug mode.
  --coverage            Generates code coverage information.
  --debug-info          With debug information.
  --warnings-as-errors  Warnings are treated as errors.

"""
from docopt import docopt
import os
import shutil
import subprocess
import copy

flags = {'--asan'               : 'NMP_WITH_ASAN',
         '--assertions'         : 'NMP_WITH_ASSERTIONS',
         '--debug'              : 'NMP_WITH_DEBUG',
         '--coverage'           : 'NMP_WITH_COVERAGE',
         '--debug-info'         : 'NMP_WITH_DEBUG_INFO',
         '--warnings-as-errors' : 'NMP_WITH_WARNINGS_AS_ERRORS',
         '--verbose'            : 'NMP_VERBOSE_CONFIGURE'
      }

# Release mode flags:
release = { '--asan'               : False,
            '--assertions'         : False,
            '--debug'              : False,
            '--coverage'           : False,
            '--debug-info'         : False,
            '--warnings-as-errors' : False,
            '--verbose'            : False,
         }

# Debug mode flags:
debug = { '--asan'               : False,
          '--assertions'         : True,
          '--debug'              : False,
          '--coverage'           : False,
          '--debug-info'         : True,
          '--warnings-as-errors' : False,
          '--verbose'            : False,
       }

# Debug unopt mode flags:
debug_unopt = copy.deepcopy(debug)
debug_unopt['--debug'] = True

# Asan mode flags:
asan = copy.deepcopy(debug)
asan['--asan'] = True

# Commit mode flags:
commit = copy.deepcopy(asan)
commit['--warning-as-errors'] = True

# Commit mode flags:
coverage = copy.deepcopy(debug_unopt)
coverage['--coverage'] = True



mode_flags = { 'asan'        : asan,
               'commit'      : commit,
               'coverage'    : coverage,
               'debug'       : debug,
               'debug-unopt' : debug_unopt,
               'release'     : release
}

def value(k, v, mode):
   if mode is None:
      return 'On' if v else 'Off'
   else:
      return 'On' if v or mode_flags.get(mode, 'release').get(k, v) else 'Off'

def cmake_options(args):
   mode = args['<mode>']
   options=''
   for k,v in args.iteritems():
      try:
         options += ' -D' + flags[k] + '=' + value(k, v, mode)
      except Exception:
         pass
   return options

if __name__ == '__main__':
   # Read environment variables
   src_dir = os.getcwd() + '/'
   version = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD'])

   # Read cmd line arguments
   args = docopt(__doc__, version='NMP ' + version)
   verbose = args['--verbose']

   print('Configuring NMP...')

   # Set up the build dir: use either user provided path or default path
   if args['<build_path>'] is None:
      build_dir = src_dir + 'build'
   else:
      build_dir = args['<build_path>']
   if not os.path.exists(build_dir):
      os.makedirs(build_dir)
   else:
      shutil.rmtree(build_dir)
      os.makedirs(build_dir)

   # Move to the build dir:
   os.chdir(build_dir)

   # Build up the cmake command:
   cmake_cmd = ('cmake ' + src_dir)
   cmake_cmd = cmake_cmd + cmake_options(args)

   # Run cmake
   if verbose:
      print('[I] CMake CMD: ' + cmake_cmd)
   p = subprocess.Popen(cmake_cmd, universal_newlines=False, shell=True,
                        stdout=subprocess.PIPE, stderr=subprocess.PIPE)

   out, err = p.communicate('foo\nfoofoo\n')
   if verbose:
      print out
      print err
   if p.returncode == 0:
      print('... done!')
   else:
      print('[E] Configuring failed!')
   exit(p.returncode)
