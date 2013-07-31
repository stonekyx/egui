noinst_PROGRAMS += %D%/test_server\
				   %D%/test_client\
				   %D%/test_uds_server\
				   %D%/test_uds_client

%C%_test_server_SOURCES = %D%/test_server.c
%C%_test_client_SOURCES = %D%/test_client.c
%C%_test_uds_server_SOURCES = %D%/test_uds_server.c
%C%_test_uds_client_SOURCES = %D%/test_uds_client.c

%C%_test_server_LDADD = library/libcomm.la library/libdata_structures.la
%C%_test_client_LDADD = library/libcomm.la library/libdata_structures.la
%C%_test_uds_server_LDADD = library/libcomm.la library/libdata_structures.la
%C%_test_uds_client_LDADD = library/libcomm.la library/libdata_structures.la
