debug_widget_LDADD_TEMP = library/libwidget.la library/libapplication.la library/libwidget.la library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la

noinst_PROGRAMS += debug/widget/test_widget \
                   debug/widget/test_label \
                   debug/widget/test_button \
                   debug/widget/test_text_line \
                   debug/widget/test_scroll_bar \
                   debug/widget/test_image_view

debug_widget_test_widget_SOURCES = debug/widget/test_widget.c
debug_widget_test_label_SOURCES = debug/widget/test_label.c
debug_widget_test_button_SOURCES = debug/widget/test_button.c
debug_widget_test_text_line_SOURCES = debug/widget/test_text_line.c
debug_widget_test_scroll_bar_SOURCES = debug/widget/test_scroll_bar.c
debug_widget_test_image_view_SOURCES = debug/widget/test_image_view.c

debug_widget_test_widget_LDADD = $(debug_widget_LDADD_TEMP)
debug_widget_test_label_LDADD = $(debug_widget_LDADD_TEMP)
debug_widget_test_button_LDADD = $(debug_widget_LDADD_TEMP)
debug_widget_test_text_line_LDADD = $(debug_widget_LDADD_TEMP)
debug_widget_test_scroll_bar_LDADD = $(debug_widget_LDADD_TEMP)
debug_widget_test_image_view_LDADD = $(debug_widget_LDADD_TEMP)

