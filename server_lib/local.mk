pkglib_LTLIBRARIES += library/libserver_lib.la
library_libserver_lib_la_SOURCES = server_lib/server_lib.c
nobase_pkginclude_HEADERS += server_lib/server_lib.h
library_libserver_lib_la_LIBADD = library/libgraph.la library/libgeometry.la library/libcomm.la\
						  library/libdata_structures.la library/libcolor.la
library_libserver_lib_la_LDFLAGS = -version-info 1:0:0
