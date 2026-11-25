# libtool -version-info current:revision:age
paranoia_current = 2
paranoia_revision = 0
paranoia_age = 0
paranoia_major = paranoia_current - paranoia_age

paranoia_sources = files('gap.c', 'isort.c', 'overlap.c', 'p_block.c',
                         'paranoia.c')

paranoia_link_args = []
paranoia_link_depends = []
if enable_versioned_libs
  paranoia_ver = configure_file(
    input: version_script_template,
    output: 'libcdio_paranoia.ver',
    configuration: {
      'NAME': 'CDIO_PARANOIA_@0@'.format(paranoia_major),
      'SYMBOLS': '    ' + ';\n    '.join(fs.read('libcdio_paranoia.sym').strip().split('\n')) + ';',
    })
  paranoia_link_args += '-Wl,--version-script,' + meson.current_build_dir() / 'libcdio_paranoia.ver'
  paranoia_link_depends += paranoia_ver
endif

libcdio_paranoia = library('cdio_paranoia', paranoia_sources,
  version: '@0@.@1@.@2@'.format(paranoia_major, paranoia_age, paranoia_revision),
  soversion: paranoia_major,
  include_directories: [top_inc, include_inc],
  link_args: paranoia_link_args,
  link_depends: paranoia_link_depends,
  dependencies: [libcdio_cdda_dep],
  install: true,
)

libcdio_paranoia_dep = declare_dependency(
  link_with: libcdio_paranoia,
  dependencies: libcdio_cdda_dep,
)
meson.override_dependency('libcdio_paranoia', libcdio_paranoia_dep)

pkg.generate(libcdio_paranoia,
  name: 'libcdio_paranoia',
  description: 'CD paranoia library from libcdio',
  filebase: 'libcdio_paranoia',
  requires: ['libcdio'],
  libraries: [libcdio_cdda],
)
