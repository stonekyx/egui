# As these global include files are written to reference
# one level up of their living directory, they have to be installed
# one level down the $(pkgincludedir) to work correctly.

nobase_pkginclude_HEADERS += include/application.h %D%/base_type.h\
							 include/client_lib.h %D%/color.h %D%/comm.h\
							 include/config_parser.h %D%/data_structures.h\
							 include/event.h %D%/geometry.h %D%/graph.h\
							 include/log.h %D%/server_lib.h %D%/widget.h

