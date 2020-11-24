#include "cpu/exec/helper.h"

make_helper(sti) {
    cpu.IF = 1;
    print_asm("sti");
	return 1;
}