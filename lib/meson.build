# Tags every exported symbol with the library's version node
# (CDIO_CDDA_2, CDIO_PARANOIA_2).  Which symbols are exported is decided
# by the CDDA_PUBLIC / PARANOIA_PUBLIC marks in the headers (see
# <cdio/paranoia/export.h>), with the libraries compiled
# -fvisibility=hidden.
version_script_template = files('version-script.in')

# On Windows, objects for the static libraries must not carry dllexport
# (a program linked with them would re-export their functions), so
# there the DLL and the static library are compiled separately: the
# DLL's objects with CDIO_BUILDING_*, the static library's with
# CDIO_STATIC.  Elsewhere one set of objects serves both.
lib_visibility_args = []
if not is_windows
  lib_visibility_args = cc.get_supported_arguments('-fvisibility=hidden')
endif
split_static = is_windows and get_option('default_library') == 'both'
windows_static_only = is_windows and get_option('default_library') == 'static'
# With MSVC, programs linking the static libraries must not import the
# symbols from a DLL.
cdio_static_args = []
if is_msvc and get_option('default_library') == 'static'
  cdio_static_args = ['-DCDIO_STATIC']
endif

subdir('cdda_interface')
subdir('paranoia')
