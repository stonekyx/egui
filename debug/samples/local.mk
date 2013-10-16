debug_samples_LDADD_TEMP = library/libwidget.la library/libapplication.la library/libwidget.la library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la

noinst_PROGRAMS += %D%/multi_windows\
				   %D%/shutdown\
				   %D%/pop_up_window\
				   %D%/single_window\
				   %D%/toplevel_child\
				   %D%/editerbasic\
				   %D%/timer\
				   %D%/text_clock\
				   %D%/tab

%C%_multi_windows_SOURCES = %D%/multi_windows.c
%C%_shutdown_SOURCES = %D%/shutdown.c
%C%_pop_up_window_SOURCES = %D%/pop_up_window.c
%C%_single_window_SOURCES = %D%/single_window.c
%C%_toplevel_child_SOURCES = %D%/toplevel_child.c
%C%_editerbasic_SOURCES = %D%/editerbasic.c
%C%_timer_SOURCES = %D%/timer.c
%C%_text_clock_SOURCES = %D%/text_clock.c
%C%_tab_SOURCES = %D%/tab.c

%C%_multi_windows_LDADD = $(debug_samples_LDADD_TEMP)
%C%_shutdown_LDADD = $(debug_samples_LDADD_TEMP)
%C%_pop_up_window_LDADD = $(debug_samples_LDADD_TEMP)
%C%_single_window_LDADD = $(debug_samples_LDADD_TEMP)
%C%_toplevel_child_LDADD = $(debug_samples_LDADD_TEMP)
%C%_editerbasic_LDADD = $(debug_samples_LDADD_TEMP)
%C%_timer_LDADD = $(debug_samples_LDADD_TEMP)
%C%_text_clock_LDADD = $(debug_samples_LDADD_TEMP)
%C%_tab_LDADD = $(debug_samples_LDADD_TEMP)

include $(top_srcdir)/debug/samples/calculator/local.mk
include $(top_srcdir)/debug/samples/file_browser/local.mk
