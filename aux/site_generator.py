import subprocess
import os
import shutil
import copy
import sys
from functools import partial
from operator import itemgetter, attrgetter

def copy_and_overwrite(from_path, to_path):
    os.system("cp -rf " + from_path + " " + to_path)

def get_directory_structure(rootdir):
    """
    Creates a nested dictionary that represents the folder structure of rootdir
    """
    dir = {}
    rootdir = rootdir.rstrip(os.sep)
    start = rootdir.rfind(os.sep) + 1
    for path, dirs, files in os.walk(rootdir):
        folders = path[start:].split(os.sep)
        subdir = dict.fromkeys(files)
        parent = reduce(dict.get, folders[:-1], dir)
        parent[folders[-1]] = subdir
    return dir

def set_up_paths(args):
    dirs = { 'src_dir' : os.path.abspath(args['<source_dir>']),
             'build_dir' : os.path.abspath(args['<build_dir>']),
             'src_site_dir' : os.path.join(os.path.abspath(args['<source_dir>']),
                                           'site/'),
             'build_site_dir' : os.path.join(os.path.abspath(args['<build_dir>']),
                                             'site/') }
    check_paths(dirs)
    args['dirs'] = dirs

    exclude_paths = ['site', args['dirs']['build_dir'], '.git']
    args['exclude_paths'] = exclude_paths

def check_paths(dirs):
    for k,v in dirs.iteritems():
        if not os.path.exists(v):
            if not 'site' in v:
                sys.exit('[E] ' + k + ' directory doesn\'t exist!')
            else:
                print('[M] ' + k + ' directory doesn\'t exist! -> Created')
                os.makedirs(v)

def check_directory_structure(args):
    rel_src_dirs = args['src_dir_structure']
    for rel_dir in rel_src_dirs:
        d = os.path.join(args['dirs']['src_dir'], rel_dir)
        if not os.path.exists(d):
            sys.exit('[E] Error invalid source dir <= Path:\n'
                     + d + '\n doesn\'t exist!')

def src_md_files(args):
    src_path = args['dirs']['src_dir']
    md_files = []
    exclude_paths = args['exclude_paths']
    for subdir, dirn, files in os.walk(src_path):
        for f in files:
            if f.endswith('.md') and not any(e in subdir for e in exclude_paths):
                md_files.append(os.path.join(subdir, f))
    return md_files

def src_to_build_md(src_md_file_path, args):
    relative_path_from_src_dir = os.path.relpath(src_md_file_path, args['dirs']['src_dir'])
    target_dir = os.path.join(args['dirs']['build_site_dir'],
                              os.path.dirname(relative_path_from_src_dir).replace('include/', ''))
    target_fn = os.path.join(target_dir, os.path.basename(src_md_file_path).replace('readme', 'index'))
    return target_fn

def toc_to_file(toc):
    print(toc)
    s = ''
    for src_path, path, name, tree, indent in toc:
        no_indent = indent.count('#')
        ss = ' ' * 2 * no_indent + '- [' + name.replace('#', '') + '](' + path.split('site')[-1][1:] + ')\n'
        s = s + ss
    print(s)
    return s

def create_and_write_toc(args):
    toc = []
    for src_md_file in src_md_files(args):
        build_md_file = src_to_build_md(src_md_file, args)
        d = os.path.dirname(build_md_file)
        site_d = os.path.dirname(args['dirs']['build_site_dir'])
        if d == site_d:
            continue
        toc_name = build_md_file.split('site')[1].replace('.md', '')
        toc_path = ''
        toc_indent = ''
        if toc_name == '/nmp/index':
            toc_name = toc_name.replace('/nmp/index', 'NMP')
        else:
            toc_path = os.path.dirname(toc_name).replace('/nmp/','')
            toc_indent = '##' + ('#' * toc_path.count('/'))
            if not 'index' in build_md_file:
                toc_indent = toc_indent + '#'
            toc_name = toc_name.split('/')[-1]
            toc_name = toc_name.replace('index', toc_path.split('/')[-1])
        toc_path = toc_path.replace('/','')
        toc.append((src_md_file, build_md_file, toc_name, toc_path, toc_indent))
    toc = sorted(toc, key=lambda x:x[3])
    toc = sorted(toc, key=lambda x:x[2])
    args['toc'] = toc
    site_includes_dir = os.path.join(args['dirs']['build_site_dir'], '_includes/')
    if not os.path.exists(site_includes_dir):
        shutils.mkdir(site_includes_dir)
    with file(site_includes_dir + 'toc', 'w') as modified: modified.write(toc_to_file(toc))


def create_site_files(args):
    toc = args['toc']
    for src_path, site_path, _, _, _ in toc:
        site_dir = os.path.dirname(site_path)
        if not os.path.exists(site_dir):
            os.makedirs(site_dir)

        with file(src_path, 'r') as original: data = original.read()
        with file(site_path, 'w') as modified: modified.write("---\n---\n" + data)
    # Copy root dir files
    for f in os.listdir(args['dirs']['src_dir']):
        if f.endswith(".md"):
            target_file = src_to_build_md(f, args)
            with file(f, 'r') as original: data = original.read()
            with file(target_file, 'w') as modified: modified.write("---\n---\n" + data)


def update_links(args):
    for subdir, dirs, files in os.walk(args['dirs']['build_site_dir']):
        for f in files:
            print(f)
            if '.md' in f or '_includes' in subdir:
                fp = os.path.join(subdir, f)
                print('!!' + fp)
                with file(fp, 'r') as original: data = original.read()
                data = data.replace('.md', '.html').replace('include/', 'nmp/')
                with file(fp, 'w') as modified: modified.write(data)

def generate_site(args):
    """Generates site"""
    print('Generating ' + args['project_name'] + '\'s website...')
    verbose = args['--verbose']

    # Check that the paths are correct and create necessary directories:
    set_up_paths(args)
    check_directory_structure(args)

    # Copy the src site to the build directory:
    copy_and_overwrite(args['dirs']['src_site_dir'], args['dirs']['build_site_dir'])

    # Create table of contents
    create_and_write_toc(args)

    # Copy md files to site_build_dir:
    create_site_files(args)

    # Update links
    update_links(args)

    print('...' + args['project_name'] + '\'s website generated!')
    sys.exit()





    md_files = md_files_in(src_dir)
    generate_site_files(md_files, src_dir, target_site_dir)
    return
