#include "cpu/exec/template-start.h"

#define instr seta

static void do_execute() {
	uint32_t result;
	if (cpu.CF == 0 && cpu.ZF == 0)	result = 1;
	else 				result = 0;

	OPERAND_W(op_src, result);

	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
