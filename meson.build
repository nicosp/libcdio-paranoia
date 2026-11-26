# Meson build for libcdio-paranoia.  This runs alongside the autotools
# build (configure.ac / Makefile.am); keep the two in step when adding
# sources, checks or options.
#
#   meson setup build
#   meson compile -C build
#   meson test -C build
#
# Copyright (C) 2026 Nicos Panayides
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

project('libcdio-paranoia', 'c',
  # Keep in step with PARANOIA_VERSION and RELEASE_NUM in configure.ac.
  version: '10.3dev+2.1.1',
  license: 'GPL-3.0-or-later',
  meson_version: '>= 1.3.0',
  default_options: [
    'warning_level=1',
    'buildtype=debugoptimized',
    # libtool builds shared and static libraries by default.
    'default_library=both',
  ],
)

fs = import('fs')
pkg = import('pkgconfig')

cc = meson.get_compiler('c')
system = host_machine.system()
is_windows = system == 'windows' or system == 'cygwin'
is_msvc = cc.get_argument_syntax() == 'msvc'

# libcdio, from pkg-config or, failing that, built as a subproject (see
# subprojects/libcdio.wrap); the latter is how MSVC gets one.
libcdio_dep = dependency('libcdio', version: '>= 2.0.0',
  fallback: ['libcdio', 'libcdio_dep'],
  default_options: ['tests=false', 'examples=false', 'cxx=false',
                    'man=disabled', 'info=disabled'])

conf = configuration_data()
conf.set_quoted('PACKAGE', 'libcdio-paranoia')
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('PACKAGE_TARNAME', meson.project_name())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING',
                meson.project_name() + ' ' + meson.project_version())
conf.set_quoted('PACKAGE_BUGREPORT', 'libcdio-help@gnu.org')
conf.set_quoted('PACKAGE_URL', '')
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('LIBCDIO_PARANOIA_SOURCE_PATH',
                meson.project_source_root().replace('\\', '/'))
conf.set('STDC_HEADERS', 1)

project_args = ['-DHAVE_CONFIG_H']

# ---------------------------------------------------------------------
# Compiler flags
# ---------------------------------------------------------------------

if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
  project_args += cc.get_supported_arguments(
    '-Wchar-subscripts', '-Wmissing-prototypes', '-Wmissing-declarations',
    '-Wunused', '-Wpointer-arith', '-Wwrite-strings', '-Wnested-externs',
    '-Wno-sign-compare')
  # AC_USE_SYSTEM_EXTENSIONS
  if not is_windows
    project_args += '-D_GNU_SOURCE'
  endif
elif is_msvc
  project_args += '-DWIN32'
  project_args += cc.get_supported_arguments('/wd4018', '/wd4996')
endif

# ---------------------------------------------------------------------
# Headers
# ---------------------------------------------------------------------

foreach h : [
    # AC_CHECK_INCLUDES_DEFAULT
    'dlfcn.h', 'inttypes.h', 'stdint.h', 'stdio.h', 'stdlib.h',
    'string.h', 'strings.h', 'sys/stat.h', 'sys/types.h', 'unistd.h',
    # configure.ac
    'assert.h', 'errno.h', 'fcntl.h', 'glob.h', 'limits.h', 'pwd.h',
    'stdarg.h', 'stdbool.h', 'sys/cdio.h', 'sys/param.h', 'sys/time.h',
    'sys/timeb.h', 'sys/utsname.h', 'getopt.h',
  ]
  conf.set('HAVE_' + h.underscorify().to_upper(),
           cc.check_header(h) ? 1 : false)
endforeach

# ---------------------------------------------------------------------
# Compiler features
# ---------------------------------------------------------------------

conf.set('WORDS_BIGENDIAN', host_machine.endian() == 'big' ? 1 : false)

if cc.compiles('void f(void) { _Pragma("pack(1)") }', name: 'ISO C99 _Pragma()')
  conf.set('HAVE_ISOC99_PRAGMA', '')
endif

if not cc.compiles('static inline int f(void) { return 0; }', name: 'inline')
  foreach kw : ['__inline__', '__inline']
    if not conf.has('inline') and cc.compiles(
        'static @0@ int f(void) { return 0; }'.format(kw), name: kw)
      conf.set('inline', kw)
    endif
  endforeach
endif

stat_prefix = cc.check_header('sys/stat.h') ? '#include <sys/stat.h>\n' : ''
if cc.links(stat_prefix + 'int main(void) { return S_ISLNK(0); }',
            name: 'S_ISLNK() macro')
  conf.set('HAVE_S_ISLNK', '')
endif
if cc.links(stat_prefix + 'int main(void) { return S_ISSOCK(0); }',
            name: 'S_ISSOCK() macro')
  conf.set('HAVE_S_ISSOCK', '')
endif
if cc.compiles('#include <time.h>\nstruct timespec ts;', name: 'struct timespec')
  conf.set('HAVE_STRUCT_TIMESPEC', '')
endif

if cc.compiles('void f(void) { struct { int foo; int bar[]; } doo; (void)doo; }',
               name: 'empty arrays with []')
  conf.set('EMPTY_ARRAY_SIZE', '')
elif cc.compiles('void f(void) { struct { int foo; int bar[0]; } doo; (void)doo; }',
                 name: 'empty arrays with [0]')
  conf.set('EMPTY_ARRAY_SIZE', 0)
else
  error('compiler is unable to create empty arrays')
endif

# ---------------------------------------------------------------------
# Libraries and functions
# ---------------------------------------------------------------------

m_dep = cc.find_library('m', required: false)
rt_dep = cc.find_library('rt', required: false)
extra_deps = [m_dep, rt_dep]

# The Microsoft C runtime defines snprintf and others inline in its
# headers, so look for them through the headers there.
function_check_prefix = ''
if is_msvc
  function_check_prefix = '''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <direct.h>
'''
endif

foreach f : ['bzero', 'chdir', 'drand48', 'ftruncate', 'geteuid', 'getgid',
             'getuid', 'getpwuid', 'gettimeofday', 'lstat', 'memcpy',
             'memset', 'rand', 'seteuid', 'setegid', 'snprintf', 'setenv',
             'unsetenv', 'tzset', 'sleep', 'usleep', 'vsnprintf', 'readlink',
             'realpath', 'gmtime_r', 'localtime_r', 'clock_gettime']
  conf.set('HAVE_' + f.to_upper(),
           cc.has_function(f, args: project_args, dependencies: extra_deps,
                           prefix: function_check_prefix) ? 1 : false)
endforeach

# ---------------------------------------------------------------------
# Operating system
# ---------------------------------------------------------------------

os_deps = []
if system == 'darwin'
  if cc.check_header('IOKit/IOKitLib.h') and cc.check_header('CoreFoundation/CFBase.h')
    conf.set('HAVE_IOKIT_IOKITLIB_H', 1)
    conf.set('HAVE_COREFOUNDATION_CFBASE_H', 1)
    conf.set('HAVE_DARWIN_CDROM', 1)
  endif
elif system == 'linux' or system == 'android'
  conf.set('HAVE_LINUX_VERSION_H', cc.check_header('linux/version.h') ? 1 : false)
  conf.set('HAVE_LINUX_MAJOR_H', cc.check_header('linux/major.h') ? 1 : false)
  if cc.check_header('linux/cdrom.h')
    conf.set('HAVE_LINUX_CDROM_H', 1)
    if cc.has_member('struct cdrom_generic_command', 'timeout',
                     prefix: '#include <linux/cdrom.h>')
      conf.set('HAVE_LINUX_CDROM_TIMEOUT', 1)
    endif
    conf.set('HAVE_LINUX_CDROM', 1)
  endif
elif system == 'sunos'
  conf.set('HAVE_SOLARIS_CDROM', 1)
elif system == 'cygwin'
  conf.set('CYGWIN', 1)
  conf.set('HAVE_WIN32_CDROM', 1)
  conf.set('NEED_TIMEZONEVAR', 1)
  os_deps += cc.find_library('winmm')
elif system == 'windows'
  conf.set('HAVE_WINDOWS_H', cc.check_header('windows.h') ? 1 : false)
  if not is_msvc
    conf.set('MINGW32', 1)
  endif
  conf.set('HAVE_WIN32_CDROM', 1)
  if is_msvc
    # winmm is part of Windows, so link it without searching: with
    # prefer_static, or static: either way, Meson searches its compiler's
    # library directories, and MSVC reports none
    os_deps += declare_dependency(link_args: '-lwinmm')
  else
    os_deps += cc.find_library('winmm')
  endif
elif system == 'freebsd' or system == 'dragonfly' or system == 'kfreebsd'
  conf.set('HAVE_FREEBSD_CDROM', 1)
  os_deps += cc.find_library('cam')
elif system == 'netbsd' or system == 'openbsd'
  conf.set('HAVE_NETBSD_CDROM', 1)
endif

# Versioned symbols need a linker that takes a GNU-style version script.
versioned_libs = get_option('versioned-libs').require(
  cc.get_linker_id() in ['ld.bfd', 'ld.gold', 'ld.lld', 'ld.mold'],
  error_message: 'the linker does not take --version-script')
enable_versioned_libs = versioned_libs.allowed()

# ---------------------------------------------------------------------
# Programs used by the tests and docs
# ---------------------------------------------------------------------

perl = find_program('perl', required: false)
doxygen = find_program('doxygen', required: false)
cmp = find_program('cmp', required: false)
diff = find_program('diff', required: false)
wc = find_program('wc', required: false)
awk = find_program('awk', 'gawk', 'mawk', 'nawk', required: false)

# ---------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------

add_project_arguments(project_args, language: 'c')

configure_file(output: 'config.h', configuration: conf)
top_inc = include_directories('.')
include_inc = include_directories('include')

# Absolute paths handed to tests and scripts, with forward slashes: on
# Windows meson gives backslashes, which C strings and the shell read as
# escapes.
abs_srcdir = meson.project_source_root().replace('\\', '/')
abs_builddir = meson.project_build_root().replace('\\', '/')

cd_paranoia_name = get_option('cd-paranoia-name')

subst = configuration_data()
subst.set('PACKAGE', 'libcdio-paranoia')
subst.set('VERSION', meson.project_version())
subst.set('PACKAGE_VERSION', meson.project_version())
subst.set('build', '@0@-@1@'.format(host_machine.cpu(), system))
subst.set('prefix', get_option('prefix'))
subst.set('exec_prefix', '${prefix}')
subst.set('libdir', '${exec_prefix}' / get_option('libdir'))
subst.set('includedir', '${prefix}' / get_option('includedir'))
subst.set('COS_LIB', m_dep.found() ? '-lm' : '')
subst.set('CDPARANOIA_NAME', cd_paranoia_name)
subst.set('LIBCDIO_PARANOIA_SOURCE_PATH', abs_srcdir)
subst.set('abs_top_srcdir', abs_srcdir)
subst.set('abs_top_builddir', abs_builddir)
subst.set('native_abs_top_srcdir', abs_srcdir)
subst.set('EXEEXT', is_windows ? '.exe' : '')
# The scripts use these commands unquoted, so give the name meson found
# on PATH rather than a full path that may contain spaces.
subst.set('CMP', cmp.found() ? fs.name(cmp.full_path()) : 'no')
subst.set('WC', wc.found() ? fs.name(wc.full_path()) : 'no')
subst.set('AWK', awk.found() ? fs.name(awk.full_path()) : 'awk')

subdir('include/cdio/paranoia')
subdir('lib')
subdir('src')
subdir('doc')
if get_option('examples') or get_option('tests')
  subdir('example')
endif
if get_option('tests')
  subdir('test')
endif

summary({
  'libcdio': libcdio_dep.version(),
  'Versioned symbols': enable_versioned_libs,
  'Program name': cd_paranoia_name,
  'Examples': get_option('examples'),
  'Tests': get_option('tests'),
}, section: 'Configuration')
