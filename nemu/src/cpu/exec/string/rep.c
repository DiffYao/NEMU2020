#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(rep) {
	int len;
	int count = 0;
	if(instr_fetch(eip + 1, 1) == 0xc3) {
		/* repz ret */
		exec(eip + 1);
		len = 0;
	}
	else {
		while(cpu.ecx) {
			exec(eip + 1);
			count ++;
			cpu.ecx --;

			/* TODO: Jump out of the while loop if necessary. */
			if ((ops_decoded.opcode == 0xa6	
				|| ops_decoded.opcode == 0xa7	
				|| ops_decoded.opcode == 0xae	
				|| ops_decoded.opcode == 0xaf) && cpu.ZF == 0) break;			

		}
		len = 1;
		
	}
	
	print_asm ("%s", assembly);
#ifdef DEBUG
	char temp[80];
	sprintf(temp, "rep %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif
	
	return len + 1;
}

make_helper(repnz) {
	int count = 0;
	while(cpu.ecx) {
		exec(eip + 1);
		count ++;
		cpu.ecx --;
		

		/* TODO: Jump out of the while loop if necessary. */
		if ((ops_decoded.opcode == 0xa6	
				|| ops_decoded.opcode == 0xa7	
				|| ops_decoded.opcode == 0xae	
				|| ops_decoded.opcode == 0xaf) && cpu.ZF == 1) break;		
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "repnz %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif

	return 1 + 1;
}
