pkglib_LTLIBRARIES += library/libgraph.la
library_libgraph_la_SOURCES = graph/graph_lower/screen_init.c \
					  graph/graph_lower/screen_exit.c \
					  graph/graph_lower/screen_get_width.c \
					  graph/graph_lower/screen_get_height.c \
					  graph/graph_lower/screen_color.c \
					  graph/graph_lower/screen_clear.c \
					  graph/graph_lower/screen_clear_black.c \
					  graph/graph_lower/screen_clear_white.c \
					  graph/graph_lower/screen_set_pixel.c \
					  graph/graph_lower/screen_set_h_line.c \
					  graph/graph_lower/screen_get_pixel.c \
					  graph/graph_lower/screen_set_v_line.c \
					  graph/graph_lower/screen_set_x_line.c \
					  graph/graph_lower/screen_set_area.c \
					  graph/graph_lower/screen_cpy_area.c \
					  graph/graph_lower/screen_flush.c \
					  graph/graph_engine/engine_get_color_limit.c \
					  graph/graph_engine/engine_graphics_device_init.c \
					  graph/graph_engine/engine_graphics_device_exit.c \
					  graph/graph_engine/engine_clear.c \
					  graph/graph_engine/engine_clear_black.c \
					  graph/graph_engine/engine_clear_white.c \
					  graph/graph_engine/engine_draw_arrow.c \
					  graph/graph_engine/engine_draw_bitmap.c \
					  graph/graph_engine/engine_draw_img.c \
					  graph/graph_engine/engine_draw_circle.c \
					  graph/graph_engine/engine_draw_ellipse.c \
					  graph/graph_engine/engine_draw_line.c \
					  graph/graph_engine/engine_draw_point.c \
					  graph/graph_engine/engine_draw_polygon.c \
					  graph/graph_engine/engine_draw_rectangle.c \
					  graph/graph_engine/engine_draw_x.c \
					  graph/graph_engine/engine_fill_arrow.c \
					  graph/graph_engine/engine_fill_circle.c \
					  graph/graph_engine/engine_fill_ellipse.c \
					  graph/graph_engine/engine_fill_rectangle.c \
					  graph/graph_engine/engine_fill_x.c \
					  graph/graph_engine/engine_show_text.c \
					  graph/graph_engine/engine_get_font_width.c \
					  graph/graph_engine/engine_get_font_height.c \
					  graph/graph_engine/engine_get_area.c \
					  graph/graph_engine/engine_get_color.c \
					  graph/graph_engine/engine_set_font.c \
					  graph/graph_engine/engine_set_area.c \
					  graph/graph_engine/engine_set_color.c \
					  graph/graph_engine/engine_cpy_area.c \
					  graph/graph_engine/engine_update.c \
					  graph/graph_engine/font_matrix/font_matrix_08.c \
					  graph/graph_engine/font_matrix/font_matrix_10.c \
					  graph/graph_engine/font_matrix/font_matrix_12.c \
					  graph/graph_engine/font_matrix/font_matrix_14.c \
					  graph/graph_engine/font_matrix/font_matrix_16.c \
					  graph/graph_engine/font_matrix/font_matrix_20.c \
					  graph/graph_engine/font_matrix/font_matrix_24.c \
					  graph/graph_engine/font_matrix/font_matrix_28.c \
					  graph/graph_engine/font_matrix/font_matrix_32.c \
					  graph/graph_engine/font_matrix/font_matrix_40.c \
					  graph/graph_engine/font_matrix/font_matrix_48.c \
					  graph/graph_engine/font_matrix/font_matrix_56.c \
					  graph/graph_engine/font_matrix/font_matrix_64.c
nobase_pkginclude_HEADERS += graph/graph_lower.h \
							 graph/graph_engine.h \
							 graph/graph_enumerator.h \
							 graph/external.h \
							 graph/bitops.h
library_libgraph_la_LIBADD = library/libgeometry.la library/libcolor.la
library_libgraph_la_LDFLAGS = -version-info 1:0:0
