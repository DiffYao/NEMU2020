#include "cpu/helper.h"
#include "cpu/exec/helper.h"
#define instr cld

make_helper(cld){

	cpu.DF = 0;
	print_asm("cld");
	return 1;
}

