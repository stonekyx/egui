pkglib_LTLIBRARIES += library/libapplication.la
library_libapplication_la_SOURCES = application/application.c\
							application/manipulate_window.c\
							application/exec.c
library_libapplication_la_LIBADD = library/libwidget.la library/libclient_lib.la library/libgeometry.la\
						   library/libcomm.la library/libconfig_parser.la\
						   library/libdata_structures.la
nobase_pkginclude_HEADERS += application/application.h\
							 application/manipulate_window.h\
							 application/exec.h\
							 application/external.h
library_libapplication_la_LDFLAGS = -version-info 1:0:0
