debug_samples_LDADD_TEMP = library/libwidget.la library/libapplication.la library/libwidget.la library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la

noinst_PROGRAMS += debug/samples/multi_windows\
				   debug/samples/shutdown\
				   debug/samples/pop_up_window\
				   debug/samples/single_window\
				   debug/samples/toplevel_child\
				   debug/samples/editerbasic

debug_samples_multi_windows_SOURCES = debug/samples/multi_windows.c
debug_samples_shutdown_SOURCES = debug/samples/shutdown.c
debug_samples_pop_up_window_SOURCES = debug/samples/pop_up_window.c
debug_samples_single_window_SOURCES = debug/samples/single_window.c
debug_samples_toplevel_child_SOURCES = debug/samples/toplevel_child.c
debug_samples_editerbasic_SOURCES = debug/samples/editerbasic.c

debug_samples_multi_windows_LDADD = $(debug_samples_LDADD_TEMP)
debug_samples_shutdown_LDADD = $(debug_samples_LDADD_TEMP)
debug_samples_pop_up_window_LDADD = $(debug_samples_LDADD_TEMP)
debug_samples_single_window_LDADD = $(debug_samples_LDADD_TEMP)
debug_samples_toplevel_child_LDADD = $(debug_samples_LDADD_TEMP)
debug_samples_editerbasic_LDADD = $(debug_samples_LDADD_TEMP)

include $(top_srcdir)/debug/samples/calculator/local.mk
include $(top_srcdir)/debug/samples/file_browser/local.mk
