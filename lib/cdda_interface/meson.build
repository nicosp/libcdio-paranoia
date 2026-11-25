# libtool -version-info current:revision:age
cdda_current = 2
cdda_revision = 0
cdda_age = 0
cdda_major = cdda_current - cdda_age

cdda_sources = files(
  'common_interface.c',
  'cddap_interface.c',
  'interface.c',
  'scan_devices.c',
  'smallft.c',
  'toc.c',
  'utils.c',
  'drive_exceptions.c',
)

cdda_link_args = []
cdda_link_depends = []
if enable_versioned_libs
  cdda_ver = configure_file(
    input: version_script_template,
    output: 'libcdio_cdda.ver',
    configuration: {'NAME': 'CDIO_CDDA_@0@'.format(cdda_major)})
  cdda_link_args += '-Wl,--version-script,' + meson.current_build_dir() / 'libcdio_cdda.ver'
  cdda_link_depends += cdda_ver
endif

cdda_common = {
  'include_directories': [top_inc, include_inc],
  'dependencies': [libcdio_dep, extra_deps, os_deps],
  'install': true,
}
cdda_shared_kwargs = {
  'version': '@0@.@1@.@2@'.format(cdda_major, cdda_age, cdda_revision),
  'soversion': cdda_major,
  'link_args': cdda_link_args,
  'link_depends': cdda_link_depends,
}
cdda_build_args = lib_visibility_args + ['-DCDIO_BUILDING_LIBCDIO_CDDA']

if split_static
  libcdio_cdda = shared_library('cdio_cdda', cdda_sources, paranoia_version_h,
    c_args: cdda_build_args,
    kwargs: cdda_common + cdda_shared_kwargs)
  static_library('cdio_cdda', cdda_sources, paranoia_version_h,
    c_args: ['-DCDIO_STATIC'],
    kwargs: cdda_common)
else
  libcdio_cdda = library('cdio_cdda', cdda_sources, paranoia_version_h,
    c_args: windows_static_only ? ['-DCDIO_STATIC'] : cdda_build_args,
    kwargs: cdda_common + cdda_shared_kwargs)
endif

libcdio_cdda_dep = declare_dependency(
  link_with: libcdio_cdda,
  include_directories: include_inc,
  dependencies: [libcdio_dep, extra_deps],
  compile_args: cdio_static_args,
  sources: paranoia_version_h,
)
meson.override_dependency('libcdio_cdda', libcdio_cdda_dep)

pkg.generate(libcdio_cdda,
  name: 'libcdio_cdda',
  description: 'CD paranoia CD-DA library from libcdio',
  filebase: 'libcdio_cdda',
  extra_cflags: cdio_static_args,
  requires: 'libcdio',
)
