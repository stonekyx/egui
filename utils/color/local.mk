pkglib_LTLIBRARIES += library/libcolor.la
library_libcolor_la_SOURCES = %D%/color.c
library_libcolor_la_LDFLAGS = -version-info 1:0:0

nobase_pkginclude_HEADERS += %D%/color.h

client_cflow_SRC += $(library_libcolor_la_SOURCES)

server_cflow_SRC += $(library_libcolor_la_SOURCES)
