#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	DATA_TYPE result = op_dest->val - op_src->val;

	cpu.CF = op_dest->val < op_src->val;
	cpu.SF = MSB(result);
	cpu.ZF = !result;
	int r1 = MSB(op_dest->val);
	int r2 = MSB(op_src->val);
	cpu.OF = (r1 != r2) && (cpu.SF == r1);
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	
	print_asm_template2();
}

make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(i2a)
#if DATA_BYTE != 1
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
