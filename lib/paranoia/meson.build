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
    configuration: {'NAME': 'CDIO_PARANOIA_@0@'.format(paranoia_major)})
  paranoia_link_args += '-Wl,--version-script,' + meson.current_build_dir() / 'libcdio_paranoia.ver'
  paranoia_link_depends += paranoia_ver
endif

paranoia_common = {
  'include_directories': [top_inc, include_inc],
  'dependencies': [libcdio_cdda_dep],
  'install': true,
}
paranoia_shared_kwargs = {
  'version': '@0@.@1@.@2@'.format(paranoia_major, paranoia_age, paranoia_revision),
  'soversion': paranoia_major,
  'link_args': paranoia_link_args,
  'link_depends': paranoia_link_depends,
}
paranoia_build_args = lib_visibility_args + ['-DCDIO_BUILDING_LIBCDIO_PARANOIA']

if split_static
  libcdio_paranoia = shared_library('cdio_paranoia', paranoia_sources,
    c_args: paranoia_build_args,
    kwargs: paranoia_common + paranoia_shared_kwargs)
  static_library('cdio_paranoia', paranoia_sources,
    c_args: ['-DCDIO_STATIC'],
    kwargs: paranoia_common)
else
  libcdio_paranoia = library('cdio_paranoia', paranoia_sources,
    c_args: windows_static_only ? ['-DCDIO_STATIC'] : paranoia_build_args,
    kwargs: paranoia_common + paranoia_shared_kwargs)
endif

libcdio_paranoia_dep = declare_dependency(
  link_with: libcdio_paranoia,
  dependencies: libcdio_cdda_dep,
)
meson.override_dependency('libcdio_paranoia', libcdio_paranoia_dep)

pkg.generate(libcdio_paranoia,
  name: 'libcdio_paranoia',
  description: 'CD paranoia library from libcdio',
  filebase: 'libcdio_paranoia',
  extra_cflags: cdio_static_args,
  requires: ['libcdio'],
  libraries: [libcdio_cdda],
)
