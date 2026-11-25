usage_txt = configure_file(input: 'usage.txt.in', output: 'usage.txt',
                           configuration: subst)
if perl.found()
  usage_h = custom_target('usage.h',
    input: ['pod2c.pl', usage_txt],
    output: 'usage.h',
    command: [perl, '@INPUT0@', '@INPUT1@'],
    capture: true)
else
  usage_h = fs.copyfile('usage-copy.h', 'usage.h')
endif

cd_paranoia = executable(cd_paranoia_name,
  'cd-paranoia.c', 'buffering_write.c', 'cachetest.c', 'header.c',
  'report.c', 'getopt.c', 'getopt1.c', usage_h,
  include_directories: top_inc,
  dependencies: libcdio_paranoia_dep,
  install: true,
)
