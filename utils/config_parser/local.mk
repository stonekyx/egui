pkglib_LTLIBRARIES += library/libconfig_parser.la

library_libconfig_parser_la_SOURCES = %D%/config_parser.c
library_libconfig_parser_la_LIBADD = library/libdata_structures.la
library_libconfig_parser_la_LDFLAGS = -version-info 1:0:0

nobase_pkginclude_HEADERS += %D%/config_parser.h
