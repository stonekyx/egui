pkglib_LTLIBRARIES += library/libcolor.la
library_libcolor_la_SOURCES = utils/color/color.c
library_libcolor_la_LDFLAGS = -version-info 1:0:0

nobase_pkginclude_HEADERS += utils/color/color.h
