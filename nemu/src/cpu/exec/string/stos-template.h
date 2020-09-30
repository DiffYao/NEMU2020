#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute() {

	uint32_t dest =  reg_l(R_EDI);
	
	if (DATA_BYTE == 1) {
		
		MEM_W (dest, reg_b(R_AL));

	}
	else if (DATA_BYTE == 2)
	{
		MEM_W (dest, reg_w(R_AX));
	}
	else
	{
		MEM_W (dest, reg_l(R_EAX));
		
	}

	
	if (cpu.DF == 0)	reg_l (R_EDI) += DATA_BYTE;
	else 			reg_l (R_EDI) -= DATA_BYTE;
	
	print_asm("stos");

}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
