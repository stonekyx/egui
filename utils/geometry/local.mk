pkglib_LTLIBRARIES += library/libgeometry.la
library_libgeometry_la_SOURCES = %D%/geometry.c
nobase_pkginclude_HEADERS += %D%/geometry.h
library_libgeometry_la_LDFLAGS = -version-info 1:0:0
