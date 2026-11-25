test_srcdir = abs_srcdir / 'test'

foreach name, deps : {
    'testparanoia': [libcdio_paranoia_dep],
    'testutils': [libcdio_paranoia_dep],
    'get_libcdio_version': [libcdio_dep],
  }
  exe = executable(name, name + '.c',
    include_directories: top_inc,
    dependencies: deps,
  )
  if name == 'get_libcdio_version'
    get_libcdio_version = exe
  endif
  test(name, exe, suite: 'lib', workdir: meson.current_build_dir())
endforeach

subdir('cdda_interface')

# Regression scripts.  They find cd-paranoia through abs_top_builddir.
sh = find_program('sh', required: false)
script_env = environment({
  'abs_top_srcdir': abs_srcdir,
  'abs_top_builddir': abs_builddir,
  'abs_srcdir': test_srcdir,
  'abs_builddir': abs_builddir / 'test',
})
# autotools sets abs_srcdir and abs_builddir per directory.
test_subst = configuration_data()
test_subst.merge_from(subst)
test_subst.set('abs_srcdir', test_srcdir)
test_subst.set('abs_builddir', abs_builddir / 'test')
if sh.found()
  foreach s : ['check_paranoia.sh', 'endian.sh', 'check_start_track_not_one.sh']
    script = configure_file(input: s + '.in', output: s, configuration: test_subst)
    test(s.split('.')[0], sh,
      args: script,
      suite: 'regression',
      env: script_env,
      workdir: meson.current_build_dir(),
      depends: [cd_paranoia, get_libcdio_version],
      timeout: 300,
    )
  endforeach
endif
