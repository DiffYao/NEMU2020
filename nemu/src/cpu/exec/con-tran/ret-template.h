#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_o_, SUFFIX)){
	
	cpu.eip = MEM_R (reg_l (R_ESP));
	print_asm("ret %x", cpu.eip);
	reg_l (R_ESP) += DATA_BYTE;
	
	return 1;

}


#include "cpu/exec/template-end.h"
