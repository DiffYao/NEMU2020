#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute() {

	DATA_TYPE src,dest;
	if (ops_decoded.is_operand_size_16)
	{
		
		src = reg_w (R_AX);
		
		dest = swaddr_read (reg_w (R_DI),DATA_BYTE);
	}
	else
	{
		
		src = reg_l (R_EAX);
		
		dest = swaddr_read (reg_l (R_EDI),DATA_BYTE);
	}
	DATA_TYPE result = src - dest;

	cpu.CF = dest < src;
	cpu.SF = MSB(result);
	cpu.ZF = !result;
	int r1 = MSB(op_dest->val);
	int r2 = MSB(op_src->val);
	cpu.OF = (r1 != r2) && (cpu.SF == r2);
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);

	if (cpu.DF == 0)REG (R_EDI) += DATA_BYTE;
	else REG (R_EDI) -= DATA_BYTE;
	
	print_asm("scas");

}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
