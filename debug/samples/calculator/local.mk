noinst_PROGRAMS += %D%/calculator

%C%_calculator_SOURCES = %D%/calculator.c %D%/main.c\
	%D%/calculator.h
%C%_calculator_LDADD = library/libwidget.la library/libapplication.la library/libwidget.la library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libconfig_parser.la library/libdata_structures.la
