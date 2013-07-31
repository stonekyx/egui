noinst_PROGRAMS += %D%/file_browser

%C%_file_browser_SOURCES = %D%/directory_content.c \
	%D%/my_widget.c \
	%D%/file_browser.c \
	%D%/directory_item.h \
	%D%/directory_content.h \
	%D%/my_widget.h

%C%_file_browser_LDADD = library/libwidget.la library/libapplication.la library/libwidget.la library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la
