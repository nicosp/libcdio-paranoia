# make check runs both examples.
foreach name : ['paranoia', 'paranoia2']
  exe = executable(name, name + '.c',
    include_directories: top_inc,
    dependencies: libcdio_paranoia_dep,
    build_by_default: get_option('examples'),
  )
  if get_option('examples') and get_option('tests')
    test(name, exe, suite: 'example', workdir: meson.current_build_dir())
  endif
endforeach

if get_option('cxx') and get_option('cpp-examples') and get_option('examples')
  subdir('C++')
endif
