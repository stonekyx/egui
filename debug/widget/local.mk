%C%_LDADD_TEMP = library/libwidget.la library/libapplication.la library/libwidget.la library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la

noinst_PROGRAMS += %D%/test_widget \
                   %D%/test_label \
                   %D%/test_button \
                   %D%/test_text_line \
                   %D%/test_scroll_bar \
                   %D%/test_image_view \
                   %D%/test_icon

%C%_test_widget_SOURCES = %D%/test_widget.c
%C%_test_label_SOURCES = %D%/test_label.c
%C%_test_button_SOURCES = %D%/test_button.c
%C%_test_text_line_SOURCES = %D%/test_text_line.c
%C%_test_scroll_bar_SOURCES = %D%/test_scroll_bar.c
%C%_test_image_view_SOURCES = %D%/test_image_view.c
%C%_test_icon_SOURCES = %D%/test_icon.c

%C%_test_widget_LDADD = $(%C%_LDADD_TEMP)
%C%_test_label_LDADD = $(%C%_LDADD_TEMP)
%C%_test_button_LDADD = $(%C%_LDADD_TEMP)
%C%_test_text_line_LDADD = $(%C%_LDADD_TEMP)
%C%_test_scroll_bar_LDADD = $(%C%_LDADD_TEMP)
%C%_test_image_view_LDADD = $(%C%_LDADD_TEMP)
%C%_test_icon_LDADD = $(%C%_LDADD_TEMP)

