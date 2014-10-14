#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "incode.h"
#include "comm.h"
/*va系列宏的兼容性.System V Unix把va_start定义为只有一个参数的宏: 
　　va_start(va_list arg_ptr);
    而ANSI C则定义为: 
　　va_start(va_list arg_ptr, prev_param);
　　如果我们要用system V的定义,应该用vararg.h头文件中所定义的宏,
	  ANSI C的宏跟system V的宏是不兼容的,我们一般都用ANSI C,所以用ANSI C的定义就够了,也便于程序的移植. */
static si_t use_function(struct egui_uds* uds_ptr,function_type fun_type,...)
{
	int ret=0;
	char buffer[1024];
	va_list arg_ptr;
	int ptr_num[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	si_t par_buf[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};     //大部分的参数类型都为si_t,最多不超过10个
	int par_buf_i=0;     //这个是par_buf的计数器
	char* title=NULL;     //一般char*为应用程序的标题
	int i=0;
	va_start(arg_ptr,fun_type);
	while(i<10)
	{
		int type;
		if(-1 == (type=get_parameter_type(fun_type,i)))
		{
			break;
		}
		else if(1==type)
		{
			par_buf[par_buf_i]=va_arg(arg_ptr,si_t);  
			par_buf_i++;
		}
		else if(2==type)                   //!!!!在这个之后再补充各个类型的参数
		{
			title=va_arg(arg_ptr,char*);
		}
		i++;
	}
	va_end(arg_ptr);
	char* tmp=buffer;
	int length=0   ; //计算包的长度
	int par_type=-1;      
	//装填头部，先把长度设置为零，等参数填充完成后再修改长度值
	*tmp=fun_type;
	tmp+=sizeof(si_t);
	length+=sizeof(si_t);
	*tmp=0;
	char* length_ptr=tmp;    //记录长度的地址
	tmp+=sizeof(si_t);
	length+=sizeof(si_t);

	//装填参数
	i=0;
	par_buf_i=0;
	while(-1!=(par_type=get_parameter_type(fun_type,i)))
	{
		if(1==par_type)
		{
			*tmp=par_buf[par_buf_i];
			par_buf_i++;
			length+=sizeof(si_t);
			tmp+=sizeof(si_t);
		}
		else if(2==par_type)      //!!!!在这个之后再补充各个类型的参数
		{
			*tmp=title;       //????可能会有问题
			length+=sizeof(char*);
			tmp+=sizeof(char*);
		}
		i++;
	}

	//重新写入长度（length包括头部长度）
	*length_ptr=length;
	ret=send_buffer_to_comm(uds_ptr,buffer,length);
	if(-1==ret)
	{
		EGUI_PRINT_ERROR("comm write error!");
		return ret;
	}
}





static si_t get_parameter_type(int fun_type,int i)
{
	if(i>=10)
	{
		return -1;
	}
	return function_parameter_graph[fun_type].parameter_list[i];
}







static int send_buffer_to_comm(struct egui_uds uds_ptr,char* buf,int length)
{
	if(-1==uds_write(usd_ptr,buf,length))
	{
		EGUI_PRINT_ERROR("cannot write into comm!");
		return -1;
	}
	return 0;
}
