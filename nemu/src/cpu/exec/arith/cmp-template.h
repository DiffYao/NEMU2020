#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	DATA_TYPE result = op_dest->val - op_src->val;
	int len = (result << 3) - 1;
	cpu.CF = op_dest->val < op_src->val;
	cpu.SF = result >> len;
	cpu.ZF = !result;
	int r1 = op_dest->val >> len;
	int r2 = op_src->val >> len;
	cpu.OF = (r1 != r2) && (cpu.SF == r1);
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	print_asm_template2();
	
	
}

make_instr_helper(i2rm)
#if DATA_BYTE != 1
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
