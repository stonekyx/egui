/**
 * @file main.c
 *
 * @author Dalian University of Technology
 *
 * @section LICENSE
 *
 * Copyright (C) 2010 Dalian University of Technology
 *
 * This file is part of EDUGUI.
 *
 * EDUGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * EDUGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EDUGUI; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/

#include "log.h"
#include <signal.h>
#include <color.h>
#include "window_manager.h"

#include <unistd.h>
#include <graph.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	/**
	 * TODO LOG
	 **/
	/**
	 * TODO 命令行参数
	 **/
#if 0 /** commented by padme 2012-05-12 {{{ **/
	if(SIG_ERR == signal(SIGCHLD,SIG_IGN))
	{
		EGUI_PRINT_ERROR("failed to set singal handler for SIGCHLD");
		return -1;
	}

	if(SIG_ERR == signal(SIGPIPE,SIG_IGN))
	{
		EGUI_PRINT_ERROR("failed to set singal handler for SIGPIPE");
		return -1;
	}
#endif /** commented by padme 2012-05-12 }}} **/


    struct color testa, testb, testblend;
    printf("%ld %ld %ld\n", testa.r, testb.r, testblend.r);

    if(argc<5) {
        fprintf(stderr, "Please give test arguments.\n");
        exit(1);
    }

	if(0 != window_manager_init())
	{
		EGUI_PRINT_ERROR("failed to init window_manager");
		return -1;
	}

    {
        static struct color **cmap;
        int stx=atoi(argv[1]), sty=atoi(argv[2]), row=atoi(argv[3]), col=atoi(argv[4]);
        int i;
        FILE *fpre, *fpost;
        struct rectangle fullscreen={
            .x=0,
            .y=0,
            .width=global_screen.width,
            .height=global_screen.height,
        };
        if(global_screen.color_depth<32) {
            goto run_server;
        }
        cmap=(struct color **)malloc(sizeof(struct color *)*row);
        for(i=0; i<row; i++) {
            cmap[i]=(struct color *)malloc(sizeof(struct color)*col);
        }
        fpre=fopen("pre_cmap.out", "w");
        for(i=0; i<row; i++) {
            int j;
            for(j=0; j<col; j++) {
                cmap[i][j].r=rand()&255;
                cmap[i][j].g=rand()&255;
                cmap[i][j].b=rand()&255;
                cmap[i][j].a=rand()&255;
                fprintf(fpre, "%ld %ld %ld %ld\n", cmap[i][j].r, cmap[i][j].g, cmap[i][j].b, cmap[i][j].a);
                screen_set_pixel(&global_screen, &fullscreen, cmap[i]+j,
                        stx+j, sty+i);
            }
        }
        fclose(fpre);
        screen_flush(fullscreen.x, fullscreen.y, fullscreen.width, fullscreen.height);
        fpost=fopen("post_cmap.out", "w");
        for(i=0; i<row; i++) {
            int j;
            for(j=0; j<col; j++) {
                struct color actu;
                screen_get_pixel(&global_screen, &fullscreen, &actu,
                        stx+j, sty+i);
                fprintf(fpost, "%ld %ld %ld %ld\n", actu.r&255, actu.g&255, actu.b&255, actu.a&255);
            }
        }
        fclose(fpost);
        sleep(10);
    }
run_server:
	if(0 != window_manager_run())
	{
		EGUI_PRINT_ERROR("failed to run window_manager");
	}

	window_manager_exit();

	return 0;
}

