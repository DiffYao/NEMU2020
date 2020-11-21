#include "device/port-io.h"
#include "cpu/exec/template-start.h"

#define instr out

make_helper(concat(out_i_, SUFFIX)) {
	uint32_t len = decode_i_b(eip + 1);
	pio_write(op_src->val, DATA_BYTE, REG(R_EAX));

	return 1 + len;
}

make_helper(concat(out_d_, SUFFIX)) {
	pio_write(reg_w(R_DX), DATA_BYTE, REG(R_EAX));

	return 1;
}

#include "cpu/exec/template-end.h"