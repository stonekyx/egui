pkglib_LTLIBRARIES += library/libevent.la
library_libevent_la_SOURCES = %D%/keybd.c %D%/mouse.c
nobase_pkginclude_HEADERS += %D%/event.h
library_libevent_la_LIBADD = library/libcomm.la library/libdata_structures.la
library_libevent_la_LDFLAGS = -version-info 1:0:0

server_cflow_SRC += $(library_libevent_la_SOURCES)
