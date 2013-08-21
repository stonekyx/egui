pkglib_LTLIBRARIES += library/libdata_structures.la
library_libdata_structures_la_SOURCES = utils/data_structures/vector.c \
								utils/data_structures/list.c \
								utils/data_structures/stack.c \
								utils/data_structures/queue.c \
								utils/data_structures/tree.c \
								utils/data_structures/object.c
nobase_pkginclude_HEADERS += utils/data_structures/external.h %D%/object.h %D%/stack.h\
							 utils/data_structures/vector.h %D%/list.h %D%/queue.h\
							 utils/data_structures/tree.h
library_libdata_structures_la_LDFLAGS = -version-info 1:0:0
