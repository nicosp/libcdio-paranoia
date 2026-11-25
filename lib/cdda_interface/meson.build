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
    configuration: {
      'NAME': 'CDIO_CDDA_@0@'.format(cdda_major),
      'SYMBOLS': '    ' + ';\n    '.join(fs.read('libcdio_cdda.sym').strip().split('\n')) + ';',
    })
  cdda_link_args += '-Wl,--version-script,' + meson.current_build_dir() / 'libcdio_cdda.ver'
  cdda_link_depends += cdda_ver
endif

libcdio_cdda = library('cdio_cdda', cdda_sources, paranoia_version_h,
  version: '@0@.@1@.@2@'.format(cdda_major, cdda_age, cdda_revision),
  soversion: cdda_major,
  include_directories: [top_inc, include_inc],
  link_args: cdda_link_args,
  link_depends: cdda_link_depends,
  dependencies: [libcdio_dep, extra_deps, os_deps],
  install: true,
)

libcdio_cdda_dep = declare_dependency(
  link_with: libcdio_cdda,
  include_directories: include_inc,
  dependencies: [libcdio_dep, extra_deps],
  sources: paranoia_version_h,
)
meson.override_dependency('libcdio_cdda', libcdio_cdda_dep)

pkg.generate(libcdio_cdda,
  name: 'libcdio_cdda',
  description: 'CD paranoia CD-DA library from libcdio',
  filebase: 'libcdio_cdda',
  requires: 'libcdio',
)
