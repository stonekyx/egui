bin_PROGRAMS += window_manager/server
window_manager_server_SOURCES = window_manager/application_info.c \
	window_manager/cursor.c \
	window_manager/flush_in_z_order.c \
	window_manager/main.c \
	window_manager/window_info.c \
	window_manager/window_manager.c \
	window_manager/window_info_iterator.c \
	window_manager/window_manager_accept_handler.c \
	window_manager/window_manager_input_handler.c \
	window_manager/window_manager_set_request_handler.c \
	window_manager/application_info.h \
	window_manager/cursor.h \
	window_manager/flush_in_z_order.h \
	window_manager/window_info.h \
	window_manager/window_info_iterator.h \
	window_manager/window_manager.h
window_manager_server_LDADD = library/libserver_lib.la library/libgraph.la library/libgeometry.la library/libevent.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la library/libcolor.la
