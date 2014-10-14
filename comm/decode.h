#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <graph.h>

#include "comm.h"
#include "compiler.h"
#include "egui_uds.h"
#include "data_structures.h"
#include "debug_graph_engine.h"


extern si_t request_handler(struct egui_uds uds_ptr);

extern si_t rev_head_from_comm(struct egui_uds uds_ptr,int* fun_type,int* length);

extern si_t rev_par_from_comm(struct egui_uds uds_ptr,char* buf,int length);
