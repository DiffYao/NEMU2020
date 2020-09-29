#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {
	DATA_TYPE src = op_src->val;
	if (op_src->size == 1 && op_dest->size != 1)
	{
		op_src->val = (int8_t)op_src->val;
	}
	src += cpu.CF;
	DATA_TYPE result = op_dest->val - src;
	
	OPERAND_W(op_dest, result);

	cpu.CF = op_dest->val < src;
	cpu.SF = MSB(result);
	cpu.ZF = !result;
	int r1 = MSB(op_dest->val);
	int r2 = MSB(op_src->val);
	cpu.OF = (r1 == r2) && (cpu.SF != r1);
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	print_asm_template2();

}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"
