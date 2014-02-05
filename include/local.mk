# As these global include files are written to reference
# one level up of their living directory, they have to be installed
# one level down the $(pkgincludedir) to work correctly.

nobase_pkginclude_HEADERS += %D%/application.h %D%/base_type.h\
							 %D%/client_lib.h %D%/color.h %D%/comm.h\
							 %D%/config_parser.h %D%/data_structures.h\
							 %D%/event.h %D%/geometry.h %D%/graph.h\
							 %D%/log.h %D%/server_lib.h %D%/widget.h\
							 %D%/compiler.h

