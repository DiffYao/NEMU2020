#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute() {

	DATA_TYPE_S IncDec;
	if (ops_decoded.is_operand_size_16)
	{
		
		swaddr_write (reg_w(R_AX),2,reg_w(R_SI));
		if (cpu.DF == 0) IncDec = 2;
		else 		 IncDec = -2;

		reg_w (R_SI) += IncDec;
	}
	else
	{
		
		swaddr_write (reg_l(R_EAX),4,reg_l(R_ESI));
		if (cpu.DF == 0)	IncDec = 4;
		else 			IncDec = -4;

		reg_l (R_ESI) += IncDec;

	}
	print_asm("lods");

}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
