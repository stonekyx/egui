pkglib_LTLIBRARIES += library/libapplication.la
library_libapplication_la_SOURCES = %D%/application.c\
							%D%/manipulate_window.c\
							%D%/exec.c
library_libapplication_la_LIBADD = library/libwidget.la library/libclient_lib.la library/libgeometry.la\
						   library/libcomm.la library/libconfig_parser.la\
						   library/libdata_structures.la
nobase_pkginclude_HEADERS += %D%/application.h\
							 %D%/manipulate_window.h\
							 %D%/exec.h\
							 %D%/external.h
library_libapplication_la_LDFLAGS = -version-info 1:0:0

client_cflow_SRC += $(library_libapplication_la_SOURCES)
