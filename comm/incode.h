#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "incode.h"
#include "comm.h"

extern si_t use_function(struct egui_uds* uds_ptr,function_type fun_type,...);

extern si_t get_parameter_type(int fun_type,int i);

