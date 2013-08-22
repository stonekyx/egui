noinst_PROGRAMS += debug/samples/calculator/calculator

debug_samples_calculator_calculator_SOURCES = debug/samples/calculator/calculator.c debug/samples/calculator/main.c\
	debug/samples/calculator/calculator.h
debug_samples_calculator_calculator_LDADD = library/libwidget.la library/libapplication.la library/libwidget.la library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la
