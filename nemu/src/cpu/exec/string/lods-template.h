#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute() {

	
	uint32_t src =  reg_l(R_ESI);

	if (DATA_BYTE == 1) {

		reg_b(R_AL) = MEM_R(src);

	}
	else if (DATA_BYTE == 2)
	{
		
		reg_w(R_AX) = MEM_R(src);
	}
	else
	{
		
		reg_l(R_EAX) = MEM_R (src);

	}

	
	if (cpu.DF == 0)	reg_l(R_ESI) += DATA_BYTE;
	else 			reg_l(R_ESI) -= DATA_BYTE;
	
	print_asm("lods");

}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
