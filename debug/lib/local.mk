noinst_PROGRAMS += debug/lib/test_server debug/lib/test_client
debug_lib_test_server_SOURCES = debug/lib/test_server.c
debug_lib_test_client_SOURCES = debug/lib/test_client.c
debug_lib_test_server_LDADD = library/libserver_lib.la library/libgraph.la library/libgeometry.la library/libcomm.la library/libdata_structures.la library/libcolor.la
debug_lib_test_client_LDADD = library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libdata_structures.la library/libcolor.la
