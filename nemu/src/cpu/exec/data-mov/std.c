#include "cpu/helper.h"
#include "cpu/exec/helper.h"

#define instr std

make_helper(std){

	cpu.DF = 1;
	print_asm("std");
	return 1;
}
