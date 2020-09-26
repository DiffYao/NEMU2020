#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	DATA_TYPE_S result = (DATA_TYPE_S) op_src->val;
	
	//if (DATA_BYTE == 1) result = (int8_t) op_src->val;
	//else if (DATA_BYTE == 2) result = (int16_t) op_src->val;
	//else if (DATA_BYTE == 4) result = (int32_t) 
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

