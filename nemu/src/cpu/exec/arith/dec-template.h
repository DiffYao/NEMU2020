#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	
	cpu.CF = op_src->val < 1;
	cpu.SF = MSB (result);
	cpu.ZF = !result;
	int r1, r2;
	r1 = MSB(op_src->val);
	r2 = 0;
	cpu.OF = (r1 != r2) && (cpu.SF == r1);
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
