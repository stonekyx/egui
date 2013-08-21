noinst_PROGRAMS += debug/comm/test_server\
				   debug/comm/test_client\
				   debug/comm/test_uds_server\
				   debug/comm/test_uds_client

debug_comm_test_server_SOURCES = debug/comm/test_server.c
debug_comm_test_client_SOURCES = debug/comm/test_client.c
debug_comm_test_uds_server_SOURCES = debug/comm/test_uds_server.c
debug_comm_test_uds_client_SOURCES = debug/comm/test_uds_client.c

debug_comm_test_server_LDADD = library/libcomm.la library/libdata_structures.la
debug_comm_test_client_LDADD = library/libcomm.la library/libdata_structures.la
debug_comm_test_uds_server_LDADD = library/libcomm.la library/libdata_structures.la
debug_comm_test_uds_client_LDADD = library/libcomm.la library/libdata_structures.la
