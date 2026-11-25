# Installed under the ja locale's man directory, as doc/ja/Makefile.am does.
configure_file(input: 'cd-paranoia.1.in',
  output: cd_paranoia_name + '.1',
  configuration: subst,
  # The Japanese man page is EUC-JP, not UTF-8.
  encoding: 'euc_jp',
  install: true,
  install_dir: get_option('mandir') / 'ja' / 'man1')
