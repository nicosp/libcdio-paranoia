toc_c = configure_file(input: 'toc.c.in', output: 'toc.c', configuration: subst)
toc = executable('toc', toc_c,
  c_args: ['-DDATA_DIR="@0@"'.format(abs_srcdir / 'test' / 'data')],
  include_directories: top_inc,
  dependencies: libcdio_paranoia_dep,
)
test('toc', toc, suite: 'lib', workdir: meson.current_build_dir())
