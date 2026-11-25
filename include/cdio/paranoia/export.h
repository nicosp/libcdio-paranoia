/*
  Copyright (C) 2026 Nicos Panayides

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file export.h
 *
 *  \brief Marks which functions and variables libcdio_cdda and
 *  libcdio_paranoia export from a shared library.
 *
 *  This follows libcdio's <cdio/export.h> but does not need it, so
 *  libcdio-paranoia still builds against libcdio releases without it.
 *
 *  Each public declaration carries CDDA_PUBLIC or PARANOIA_PUBLIC, for
 *  the library that defines it.  While that library is being built the
 *  macro exports the symbol: __declspec(dllexport) on Windows, default
 *  visibility elsewhere (the libraries are compiled with
 *  -fvisibility=hidden, so nothing unmarked is exported).  In programs
 *  that use the library, MSVC needs __declspec(dllimport) to use a
 *  variable from a DLL, so the macro adds it there; MinGW and Cygwin
 *  import without it.
 *
 *  With MSVC, define CDIO_STATIC before including any libcdio or
 *  libcdio-paranoia header when linking the static libraries.
 */

#ifndef CDIO_PARANOIA_EXPORT_H_
#define CDIO_PARANOIA_EXPORT_H_

#if defined(_WIN32) || defined(__CYGWIN__)
#  define CDIO_PARANOIA_EXPORT_ __declspec(dllexport)
#  if defined(_MSC_VER)
#    define CDIO_PARANOIA_IMPORT_ __declspec(dllimport)
#  else
     /* The MinGW and Cygwin linkers import from a DLL by themselves. */
#    define CDIO_PARANOIA_IMPORT_
#  endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#  define CDIO_PARANOIA_EXPORT_ __attribute__((visibility("default")))
#  define CDIO_PARANOIA_IMPORT_ __attribute__((visibility("default")))
#else
#  define CDIO_PARANOIA_EXPORT_
#  define CDIO_PARANOIA_IMPORT_
#endif

/* On Windows libtool compiles a library's sources twice, defining
   DLL_EXPORT only for the DLL.  The static library's objects must not
   export anything, or every program linked with them would re-export
   them. */
#if defined(CDIO_PARANOIA_LIBTOOL) && (defined(_WIN32) || defined(__CYGWIN__)) \
    && !defined(DLL_EXPORT)
#  define CDIO_PARANOIA_STATIC_ 1
#endif

#if defined(CDIO_STATIC) || defined(CDIO_PARANOIA_STATIC_)
#  define CDDA_PUBLIC
#  define PARANOIA_PUBLIC
#else
#  if defined(CDIO_BUILDING_LIBCDIO_CDDA)
#    define CDDA_PUBLIC CDIO_PARANOIA_EXPORT_
#  else
#    define CDDA_PUBLIC CDIO_PARANOIA_IMPORT_
#  endif
#  if defined(CDIO_BUILDING_LIBCDIO_PARANOIA)
#    define PARANOIA_PUBLIC CDIO_PARANOIA_EXPORT_
#  else
#    define PARANOIA_PUBLIC CDIO_PARANOIA_IMPORT_
#  endif
#endif

#endif /* CDIO_PARANOIA_EXPORT_H_ */
