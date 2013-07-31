noinst_PROGRAMS += %D%/debug_graph
%C%_debug_graph_SOURCES = %D%/debug_graph.c \
	%D%/debug_graph_lower.c \
	%D%/debug_graph_engine.c \
	%D%/debug_graph.h \
	%D%/debug_graph_lower.h \
	%D%/debug_graph_engine.h
%C%_debug_graph_LDADD = library/libgraph.la library/libgeometry.la\
	library/libdata_structures.la library/libcolor.la
