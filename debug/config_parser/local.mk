noinst_PROGRAMS += %D%/main
%C%_main_SOURCES = %D%/main.c
%C%_main_LDADD = library/libevent.la library/libcomm.la\
	library/libconfig_parser.la\
	library/libdata_structures.la
