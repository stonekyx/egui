bin_PROGRAMS += %D%/server
%C%_server_SOURCES = %D%/application_info.c \
	%D%/cursor.c \
	%D%/flush_in_z_order.c \
	%D%/main.c \
	%D%/window_info.c \
	%D%/window_manager.c \
	%D%/window_info_iterator.c \
	%D%/window_manager_accept_handler.c \
	%D%/window_manager_input_handler.c \
	%D%/window_manager_set_request_handler.c \
	%D%/application_info.h \
	%D%/cursor.h \
	%D%/flush_in_z_order.h \
	%D%/window_info.h \
	%D%/window_info_iterator.h \
	%D%/window_manager.h
%C%_server_LDADD = library/libserver_lib.la library/libgraph.la library/libgeometry.la library/libevent.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la library/libcolor.la

server_cflow_SRC += $(%C%_server_SOURCES)
