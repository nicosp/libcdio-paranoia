install_headers('cdda.h', 'paranoia.h', 'toc.h', subdir: 'cdio/paranoia')

# Used inside the build only; autotools does not install it either.
paranoia_version_h = configure_file(input: 'version.h.in',
  output: 'version.h',
  configuration: subst)
