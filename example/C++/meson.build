add_languages('cpp', native: false, required: true)

foreach name : ['paranoia', 'paranoia2']
  exe = executable('cpp-' + name, name + '.cpp',
    include_directories: top_inc,
    dependencies: libcdio_paranoia_dep,
  )
  if get_option('tests')
    test(name, exe, suite: 'example-cpp', workdir: meson.current_build_dir())
  endif
endforeach
