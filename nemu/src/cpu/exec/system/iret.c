#include "cpu/exec/helper.h"

make_helper(iret) {
	//if (cpu.PE == 0) {
		cpu.eip = swaddr_read(cpu.esp, 4, 2);
		cpu.esp += 4;
		cpu.sreg[1].selector = swaddr_read(cpu.esp, 4, 2);
		cpu.esp += 4;
		cpu.eflags = swaddr_read(cpu.esp, 4, 2);
		cpu.esp += 4;
		return 0;
	//}
}
