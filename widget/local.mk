pkglib_LTLIBRARIES += library/libwidget.la

library_libwidget_la_SOURCES = widget/widget/widget.c\
					   widget/button/button.c\
					   widget/label/label.c\
					   widget/panel/panel.c\
					   widget/scroll_bar/scroll_bar.c\
					   widget/text_line/text_line.c\
					   widget/image_view/image_view.c\
					   widget/window/window.c\
					   widget/window/paint_window_decoration.c\
					   widget/dialog/dialog.c\
					   widget/icon/icon.c\
					   widget/checkbox/checkbox.c

noinst_HEADERS += widget/window/paint_window_decoration.h

nobase_pkginclude_HEADERS += widget/widget/widget.h\
							 widget/button/button.h\
							 widget/label/label.h\
							 widget/panel/panel.h\
							 widget/scroll_bar/scroll_bar.h\
							 widget/text_line/text_line.h\
							 widget/image_view/image_view.h\
							 widget/window/window.h\
							 widget/dialog/dialog.h\
							 widget/icon/icon.h\
							 widget/checkbox/checkbox.h\
							 widget/external.h

library_libwidget_la_LIBADD = library/libclient_lib.la library/libgeometry.la library/libcomm.la \
					  library/libconfig_parser.la library/libdata_structures.la

library_libwidget_la_LDFLAGS = -version-info 1:0:0

# widget/window/window_status.h is not complete, thus not distributing now.
