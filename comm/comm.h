#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "log.h"
#include "compiler.h"

enum function_type
{
    REGISTER_APPLICATION=0,
	CANCEL_APPLICATION,
	GRAPHICS_DEVICE_INIT,
	GRAPHICE_DEVICE_EXIT,
	WINDOW_MANAGER_QUIT,
	UPDATA,
    DESKTOP_DIRTY,
	REGIDTER_WINDOW,
	CANCEL_WINDOW,
	MOVE_WINDOW,
	RESIZE_WINDOW,
	MINIMIZE_WINDOW,
	MAXIMIZE_WINDOW,
	RESTORE_WINDOW,
	ACTIVETE_WINDOW,
	CLEAR,
	CLEAR_WHITE,
	CLEAR_BLACK,
	DRAW_LINE,
	DRAW_POINT,
	DRAW_RECTANGLE,
	DRAW_POLYGON,
	DRAW_CIRCLE,
	DRAW_ELLIPSE,
	DRAW_ARROW,
	DRAW_X,
	DRAW_BITMAP,
	DRAW_IMG,
	FILL_RECTANGLE,
	FILL_CIRCLE,
	FILL_ELLIPSE,
	FILL_X,
	FILL_ARROW,
	FILL_POLYGON,
	CPY_AREA,
	SHOW_TEXT,
	GET_AREA,
	GET_COLOR,
	GET_COLOR_LIMIT,
	GET_FONT_WIDTH,
	GET_FONT_HEIGHT,
	GET_SCREEN_WIDTH,
	GET_SCREEN_HEIGHT,
	SET_COLOR,
	SET_AREA,
	SET_FONT,
	GET_WINDOW_TITLE
};
enum data_type
{
    UI_T=0,
	SI_T,
	CHAR_POINT,
	STRUCT_RECTANGLE_POINT£¬
	STRUCT_POINT_POINT£¬
	INT,
	STRUCT_COLOR_POINT,
	STRUCT_COLOR_LIMIT_POINT
};
struct function_parameter_member
{
	enum data_type parameter_list[10];
};


static int get_parameter_type(int fun_type,int i);





struct function_parameter_member function_parameter_graph[47];
function_parameter_graph[0]={1,1,2,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[1]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[2]={1,1,1,1,1,1,1,1,1,-1};
function_parameter_graph[3]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[4]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[5]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[6]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[7]={1,2,1,1,1,1,1,1,1,-1};
function_parameter_graph[8]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[9]={1,1,1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[10]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[11]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[12]={1,3,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[13]={1,3,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[14]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[15]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[16]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[17]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[18]={1,1,1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[19]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[20]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[21]={1,4,0,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[22]={1,1,1,1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[23]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[24]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[25]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[26]={1,2,1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[27]={1,2,5,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[28]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[29]={1,1,1,1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[30]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[31]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[32]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[33]={1,4,0,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[34]={1,1,1,1,1,1,1,1,-1,-1};
function_parameter_graph[35]={1,1,1,2,1,-1,-1,-1,-1,-1};
function_parameter_graph[36]={1,3,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[37]={1,6,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[38]={7,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[39]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[40]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[41]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[42]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[43]={1,0,0,0,0,-1,-1,-1,-1,-1};
function_parameter_graph[44]={1,1,1,1,1,-1,-1,-1,-1,-1};
function_parameter_graph[45]={1,1,-1,-1,-1,-1,-1,-1,-1,-1};
function_parameter_graph[46]={1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
