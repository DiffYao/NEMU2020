#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	int32_t result = (DATA_TYPE_S) (op_src->val);
	
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

