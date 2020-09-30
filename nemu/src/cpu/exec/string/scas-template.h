#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute() {

	uint32_t src,dest;
	
	dest = MEM_R (reg_l (R_EDI));
	if (DATA_BYTE == 1) {

		src = reg_b(R_AL);

	}
	else if (DATA_BYTE == 2)
	{
		
		src = reg_w(R_AX);

	}
	else
	{
		
		src = reg_l(R_EAX);

	}

	DATA_TYPE result = src - dest;


	cpu.CF = src < dest;
	cpu.SF = MSB(result);
	cpu.ZF = !result;
	
	int r1 = MSB(op_dest->val);
	int r2 = MSB(op_src->val);
	cpu.OF = (r1 != r2) && (cpu.SF == r2);
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);

	if (cpu.DF == 0)	reg_l (R_EDI) += DATA_BYTE;
	else			reg_l (R_EDI) -= DATA_BYTE;
	
	print_asm("scas");

}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
