#include "cpu/exec/template-start.h"

#define instr sar

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;

	cpu.CF = MSB(src << ( (DATA_BYTE << 3) - 1) );
	uint8_t count = src & 0x1f;
	dest >>= count;
	OPERAND_W(op_dest, dest);

	DATA_TYPE result = dest;
	cpu.ZF = !result;
	cpu.SF = MSB(result);	
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
