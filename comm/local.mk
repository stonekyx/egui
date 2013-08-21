pkglib_LTLIBRARIES += library/libcomm.la
library_libcomm_la_SOURCES = comm/egui_uds.c \
					 comm/event_listener.c \
					 comm/request.c \
					 comm/message.c \
					 comm/respond.c \
					 comm/egui_comm.c
library_libcomm_la_LIBADD = library/libdata_structures.la
nobase_pkginclude_HEADERS += comm/egui_uds.h \
							 comm/event_listener.h \
							 comm/egui_comm.h \
							 comm/message.h \
							 comm/packet.h \
							 comm/request.h \
							 comm/respond.h \
							 comm/external.h
library_libcomm_la_LDFLAGS = -version-info 1:0:0
