#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute() {

	DATA_TYPE_S IncDec;

	if (DATA_BYTE == 1){
	
		reg_b(R_AL) = MEM_R (reg_w(R_SI));

		if (cpu.DF == 0) IncDec = 1;
		else 		 IncDec = -1;

		reg_w (R_SI) += IncDec;

	}
	else if (DATA_BYTE == 2)
	{
		
		reg_w(R_AX) = MEM_R (reg_w(R_SI));

		if (cpu.DF == 0) IncDec = 2;
		else 		 IncDec = -2;

		reg_w (R_SI) += IncDec;
	}
	else
	{
		
		reg_l(R_EAX) = MEM_R (reg_l(R_ESI));

		if (cpu.DF == 0)	IncDec = 4;
		else 			IncDec = -4;

		reg_l (R_ESI) += IncDec;

	}
	print_asm("lods");

}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
