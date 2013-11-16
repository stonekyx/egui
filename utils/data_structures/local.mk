pkglib_LTLIBRARIES += library/libdata_structures.la
library_libdata_structures_la_SOURCES = %D%/vector.c \
								%D%/list.c \
								%D%/stack.c \
								%D%/queue.c \
								%D%/tree.c \
								%D%/object.c \
								%D%/hashmap.c \
								%D%/cursor.c
nobase_pkginclude_HEADERS += %D%/external.h %D%/object.h %D%/stack.h\
							 %D%/vector.h %D%/list.h %D%/queue.h\
							 %D%/tree.h %D%/hashmap.h %D%/cursor.h
library_libdata_structures_la_LDFLAGS = -version-info 1:0:0

client_cflow_SRC += $(library_libdata_structures_la_SOURCES)

server_cflow_SRC += $(library_libdata_structures_la_SOURCES)
