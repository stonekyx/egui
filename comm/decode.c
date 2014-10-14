#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <graph.h>

#include "comm.h"
#include "compiler.h"
#include "decode.h"
#include "debug_graph_engine.h"


char buffer[1024];


static si_t request_handler(struct egui_uds uds_ptr)
{
	int i=0;
	si_t par_buf[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};         //跟解码一样，储存参数
	int par_buf_i=0;
	char* title=NULL;
	int fun_type=-1;
	int length=0;
	if(-1==rev_head_from_comm(uds_ptr,&fun_type,&length))
	{
		EGUI_PRINT_ERROR("read head error!");
		return -1;
	}
	if(-1==rev_par_from_comm(uds_ptr,buffer,length-sizeof(int)*2))
	{
		EGUI_PRINT_ERROR("read parameter error!");
		return -1;
	}
	char* tmp=buffer;
	while(i<10)
	{
		int type=-1;
		if(-1==(type=get_function_type(fun_type,i)))
		{
			break;
		}
		else if(1==type)
		{
			par_buf[par_buf_i]=*((si_t)tmp);
			par_buf_i++;
			tmp+=sizeof(si_t);
		}
		else if(2==type)      //之后会补齐各种类型
		{
			title=*tmp;     //????可能会有问题
			tmp+=sizeof(char*);
		}
		i++;
	}


	//处理各个请求
	switch(fun_type)
	{
	case 0:

		break;
	case 1:

		break;
	case 2:

		break;
	case 3:

		break;
	case 4:

		break;
	case 5:

		break;
	case 6:

		break;
	case 7:

		break;
	case 8:

		break;
	case 9:

		break;
	case 10:

		break;
	case 11:

		break;
	case 12:

		break;
	case 13:

		break;
	case 14:

		break;
	case 15:

		break;
	case 16:

		break;
	case 17:

		break;
	case 18:
		engine_draw_line(par_buf[0],par_buf[1],par_buf[2],par_buf[3],par_buf[4]);     //??之前写的程序并没有真正回应该请求，好像图形编辑器的请求都没有真正调用函数。
		break;
	case 19:

		break;
	case 20:

		break;
	case 21:

		break;
	case 22:

		break;
	case 23:

		break;
	case 24:

		break;
	case 25:

		break;
	case 26:

		break;
	case 27:

		break;
	case 28:

		break;
	case 29:

		break;
	case 30:

		break;
	case 31:

		break;
	case 32:

		break;
	case 33:

		break;
	case 34:

		break;
	case 35:

		break;
	case 36:

		break;
	case 37:

		break;
	case 38:

		break;
	case 39:

		break;
	case 40:

		break;
	case 41:

		break;
	case 42:

		break;
	case 43:

		break;
	case 44:

		break;
	case 45:

		break;
	default:
		EGUI_ERROR_PRINT("fail find function of this type!);
		break;

	}
}




static si_t rev_head_from_comm(struct egui_uds uds_ptr,int* fun_type,int* length)
{
	char tmp[16];
	if(uds_read(fun_type,tmp,sizeof(int)*2)<0)
	{
		EGUI_PRINT_ERROR("fail to read from comm!");
		return -1;
	}
	*fun_type=*((int)tmp);
	*length=*((int)(tmp+sizeof(int)));
	return 0;
}






static si_t rev_par_from_comm(struct egui_uds uds_ptr,char* buf,int length)
{
	if(uds_read(uds_ptr,buffer,length)<0)
	{
		EGUI_PRINT_ERROR("fail to read from comm!");
		return -1;
	}
	return 0;
}
