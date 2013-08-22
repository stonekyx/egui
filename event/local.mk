pkglib_LTLIBRARIES += library/libevent.la
library_libevent_la_SOURCES = event/keybd.c event/mouse.c
nobase_pkginclude_HEADERS += event/event.h
library_libevent_la_LIBADD = library/libcomm.la library/libdata_structures.la
library_libevent_la_LDFLAGS = -version-info 1:0:0
