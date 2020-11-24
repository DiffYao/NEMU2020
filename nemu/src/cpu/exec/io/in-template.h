#include "device/port-io.h"
#include "cpu/exec/template-start.h"

#define instr in

make_helper(concat(in_i_, SUFFIX)) {
	uint32_t len = decode_i_b(eip + 1);
	REG(R_EAX) = pio_read(op_src->val, DATA_BYTE);
	print_asm("in");
	return 1 + len;
}

make_helper(concat(in_d_, SUFFIX)) {
	REG(R_EAX) = pio_read(reg_w(R_DX), DATA_BYTE);
	print_asm("in");
	return 1;
}

#include "cpu/exec/template-end.h"