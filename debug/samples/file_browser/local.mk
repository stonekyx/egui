noinst_PROGRAMS += debug/samples/file_browser/file_browser

debug_samples_file_browser_file_browser_SOURCES = debug/samples/file_browser/directory_content.c \
	debug/samples/file_browser/my_widget.c \
	debug/samples/file_browser/file_browser.c \
	debug/samples/file_browser/directory_item.h \
	debug/samples/file_browser/directory_content.h \
	debug/samples/file_browser/my_widget.h

debug_samples_file_browser_file_browser_LDADD = library/libwidget.la library/libapplication.la library/libwidget.la library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la
