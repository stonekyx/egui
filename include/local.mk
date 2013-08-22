# As these global include files are written to reference
# one level up of their living directory, they have to be installed
# one level down the $(pkgincludedir) to work correctly.

nobase_pkginclude_HEADERS += include/application.h include/base_type.h\
							 include/client_lib.h include/color.h include/comm.h\
							 include/config_parser.h include/data_structures.h\
							 include/event.h include/geometry.h include/graph.h\
							 include/log.h include/server_lib.h include/widget.h

