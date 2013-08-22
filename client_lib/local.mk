pkglib_LTLIBRARIES += library/libclient_lib.la
library_libclient_lib_la_SOURCES = client_lib/client_lib.c
library_libclient_lib_la_LIBADD = library/libcomm.la library/libdata_structures.la
nobase_pkginclude_HEADERS += client_lib/client_lib.h client_lib/external.h
library_libclient_lib_la_LDFLAGS = -version-info 1:0:0
