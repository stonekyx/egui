noinst_PROGRAMS += %D%/test_server %D%/test_client
%C%_test_server_SOURCES = %D%/test_server.c
%C%_test_client_SOURCES = %D%/test_client.c
%C%_test_server_LDADD = library/libserver_lib.la library/libgraph.la library/libgeometry.la library/libcomm.la library/libdata_structures.la library/libcolor.la
%C%_test_client_LDADD = library/libclient_lib.la library/libgeometry.la library/libcomm.la library/libdata_structures.la library/libcolor.la
