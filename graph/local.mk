pkglib_LTLIBRARIES += library/libgraph.la
library_libgraph_la_SOURCES = %D%/graph_lower/screen_init.c \
					  %D%/graph_lower/screen_exit.c \
					  %D%/graph_lower/screen_get_width.c \
					  %D%/graph_lower/screen_get_height.c \
					  %D%/graph_lower/screen_color.c \
					  %D%/graph_lower/screen_clear.c \
					  %D%/graph_lower/screen_clear_black.c \
					  %D%/graph_lower/screen_clear_white.c \
					  %D%/graph_lower/screen_set_pixel.c \
					  %D%/graph_lower/screen_set_h_line.c \
					  %D%/graph_lower/screen_get_pixel.c \
					  %D%/graph_lower/screen_set_v_line.c \
					  %D%/graph_lower/screen_set_x_line.c \
					  %D%/graph_lower/screen_set_area.c \
					  %D%/graph_lower/screen_cpy_area.c \
					  %D%/graph_lower/screen_flush.c \
					  %D%/graph_engine/engine_get_color_limit.c \
					  %D%/graph_engine/engine_graphics_device_init.c \
					  %D%/graph_engine/engine_graphics_device_exit.c \
					  %D%/graph_engine/engine_clear.c \
					  %D%/graph_engine/engine_clear_black.c \
					  %D%/graph_engine/engine_clear_white.c \
					  %D%/graph_engine/engine_draw_arrow.c \
					  %D%/graph_engine/engine_draw_bitmap.c \
					  %D%/graph_engine/engine_draw_img.c \
					  %D%/graph_engine/engine_draw_circle.c \
					  %D%/graph_engine/engine_draw_ellipse.c \
					  %D%/graph_engine/engine_draw_line.c \
					  %D%/graph_engine/engine_draw_point.c \
					  %D%/graph_engine/engine_draw_polygon.c \
					  %D%/graph_engine/engine_draw_rectangle.c \
					  %D%/graph_engine/engine_draw_x.c \
					  %D%/graph_engine/engine_draw_cursor.c \
					  %D%/graph_engine/engine_fill_arrow.c \
					  %D%/graph_engine/engine_fill_circle.c \
					  %D%/graph_engine/engine_fill_ellipse.c \
					  %D%/graph_engine/engine_fill_polygon.c \
					  %D%/graph_engine/engine_fill_rectangle.c \
					  %D%/graph_engine/engine_fill_x.c \
					  %D%/graph_engine/engine_show_text.c \
					  %D%/graph_engine/engine_get_font_width.c \
					  %D%/graph_engine/engine_get_font_height.c \
					  %D%/graph_engine/engine_get_area.c \
					  %D%/graph_engine/engine_get_color.c \
					  %D%/graph_engine/engine_set_font.c \
					  %D%/graph_engine/engine_set_area.c \
					  %D%/graph_engine/engine_set_color.c \
					  %D%/graph_engine/engine_cpy_area.c \
					  %D%/graph_engine/engine_update.c \
					  %D%/graph_engine/font_matrix/font_matrix_08.c \
					  %D%/graph_engine/font_matrix/font_matrix_10.c \
					  %D%/graph_engine/font_matrix/font_matrix_12.c \
					  %D%/graph_engine/font_matrix/font_matrix_14.c \
					  %D%/graph_engine/font_matrix/font_matrix_16.c \
					  %D%/graph_engine/font_matrix/font_matrix_20.c \
					  %D%/graph_engine/font_matrix/font_matrix_24.c \
					  %D%/graph_engine/font_matrix/font_matrix_28.c \
					  %D%/graph_engine/font_matrix/font_matrix_32.c \
					  %D%/graph_engine/font_matrix/font_matrix_40.c \
					  %D%/graph_engine/font_matrix/font_matrix_48.c \
					  %D%/graph_engine/font_matrix/font_matrix_56.c \
					  %D%/graph_engine/font_matrix/font_matrix_64.c
nobase_pkginclude_HEADERS += %D%/graph_lower.h \
							 %D%/graph_engine.h \
							 %D%/graph_enumerator.h \
							 %D%/external.h \
							 %D%/bitops.h
library_libgraph_la_LIBADD = library/libgeometry.la library/libcolor.la
library_libgraph_la_LDFLAGS = -version-info 1:0:0

server_cflow_SRC += $(library_libgraph_la_SOURCES)
