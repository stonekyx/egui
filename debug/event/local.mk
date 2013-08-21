noinst_PROGRAMS += debug/event/main
debug_event_main_SOURCES = debug/event/main.c
debug_event_main_LDADD = library/libevent.la library/libcomm.la\
	library/libdata_structures.la
