noinst_PROGRAMS += debug/graph/debug_graph
debug_graph_debug_graph_SOURCES = debug/graph/debug_graph.c \
	debug/graph/debug_graph_lower.c \
	debug/graph/debug_graph_engine.c \
	debug/graph/debug_graph.h \
	debug/graph/debug_graph_lower.h \
	debug/graph/debug_graph_engine.h
debug_graph_debug_graph_LDADD = library/libgraph.la library/libgeometry.la\
	library/libdata_structures.la library/libcolor.la
