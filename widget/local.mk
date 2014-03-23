pkglib_LTLIBRARIES += library/libwidget.la

library_libwidget_la_SOURCES = %D%/widget/widget.c\
					   %D%/button/button.c\
					   %D%/label/label.c\
					   %D%/panel/panel.c\
					   %D%/scroll_bar/scroll_bar.c\
					   %D%/text_line/text_line.c\
					   %D%/image_view/image_view.c\
					   %D%/window/window.c\
					   %D%/window/paint_window_decoration.c\
					   %D%/dialog/dialog.c\
					   %D%/icon/icon.c\
					   %D%/checkbox/checkbox.c\
					   %D%/radiobutton/radiobutton.c\
					   %D%/spinbox/spinbox.c\
					   %D%/timer/timer.c\
					   %D%/flowbox/flowbox.c\
					   %D%/tab/tab.c\
					   %D%/tab/tab_page.c\
					   %D%/tab/tab_header.c

noinst_HEADERS += %D%/window/paint_window_decoration.h

nobase_pkginclude_HEADERS += %D%/widget/widget.h\
							 %D%/button/button.h\
							 %D%/label/label.h\
							 %D%/panel/panel.h\
							 %D%/scroll_bar/scroll_bar.h\
							 %D%/text_line/text_line.h\
							 %D%/image_view/image_view.h\
							 %D%/window/window.h\
							 %D%/dialog/dialog.h\
							 %D%/icon/icon.h\
							 %D%/checkbox/checkbox.h\
							 %D%/radiobutton/radiobutton.h\
							 %D%/spinbox/spinbox.h\
							 %D%/timer/timer.h\
							 %D%/flowbox/flowbox.h\
							 %D%/tab/tab.h\
							 %D%/tab/tab_page.h\
							 %D%/tab/tab_header.h\
							 %D%/external.h

library_libwidget_la_LIBADD = library/libapplication.la library/libclient_lib.la library/libgeometry.la library/libcomm.la \
					  library/libconfig_parser.la library/libdata_structures.la \
					  -lrt

library_libwidget_la_LDFLAGS = -version-info 1:0:0

# %D%/window/window_status.h is not complete, thus not distributing now.

client_cflow_SRC += $(library_libwidget_la_SOURCES)
