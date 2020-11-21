#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */

typedef struct {
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t  args[4];
}PartOfStackFrame;

char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_x(char *args);

static int cmd_info(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static int cmd_bt(char *args);

static int cmd_page(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Execute N instructions in a single step and then pause, the default is 1", cmd_si},
	{ "info", " r for  Disply info about register \n\tw for print watchpoint infomation", cmd_info},
	{ "x", "Scanf memory, Disply N 4 Byte", cmd_x},
	{ "p", "Calculate the value of EXPR", cmd_p},
	{ "w", "Set watchpoint for EXPR", cmd_w},
	{ "d", "Delete No.N watchpoint", cmd_d},
	{ "bt", "Print stack frame chain",cmd_bt},
	{ "page", "Show page translate", cmd_page}

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))
static int cmd_info(char *args){
	char* arg = strtok(NULL, " ");
	if (arg == NULL || strlen(arg) > 2 || (arg[0] != 'r' && arg[0] != 'w')){
		printf("Invalid Input\n");
	}
	else if (strcmp(arg , "rs") == 0){
		printf("base = %x,\nlimit = %x\n", cpu.gdtr.base, cpu.gdtr.limit);
	}
	else if (strcmp(arg , "rc") == 0){
		printf("CR0 = %x\n", cpu.cr0.val);
	}
	else if(strcmp(arg, "r") == 0){
		printf("%%cpu.eax : %#08x\t \n", cpu.eax );//swaddr_read(cpu.eax, 4));
		printf("%%cpu.ecx : %#08x\t \n", cpu.ecx );//swaddr_read(cpu.ecx, 4));
		printf("%%cpu.edx : %#08x\t \n", cpu.edx );//swaddr_read(cpu.edx, 4));
		printf("%%cpu.ebx : %#08x\t \n", cpu.ebx );//swaddr_read(cpu.ebx, 4));
		printf("%%cpu.esp : %#08x\t \n", cpu.esp );//swaddr_read(cpu.esp, 4));
		printf("%%cpu.ebp : %#08x\t \n", cpu.ebp );//swaddr_read(cpu.ebp, 4));
		printf("%%cpu.esi : %#08x\t \n", cpu.esi );//swaddr_read(cpu.esi, 4));
		printf("%%cpu.edi : %#08x\t \n", cpu.edi );//swaddr_read(cpu.edi, 4));
		printf("%%cpu.eip : %#08x\t \n", cpu.eip );//swaddr_read(cpu.eip, 4));
	}
	else if(strcmp(arg, "rf") == 0){
		printf("%%cpu.CF : %x\n", cpu.CF);
        printf("%%cpu.OF : %x\n", cpu.OF);
        printf("%%cpu.ZF : %x\n", cpu.ZF);
        printf("%%cpu.PF : %x\n", cpu.PF);
		printf("%%cpu.SF : %x\n", cpu.SF);
		printf("%%cpu.DF : %x\n", cpu.DF);
	
	}
	else if (strcmp(arg, "w") == 0){
		printf("NUM\t\tEXPR\t\tVAL\n"); 
		print_wp();
	}
	return 0;
}
static int cmd_p(char *args){
	if (args == NULL) {
		printf("Invalid Input\n");
		return 0;
	}
	bool is;
	uint32_t num = expr(args, &is);
	if (is) {
		printf("result is 0x%x\n", num);
	}else{
		printf("EXPR IS WRONG\n");
	}			
	return 0;
}
static int cmd_d(char *args){
	char *arg = strtok(NULL, " ");
	int num;
	int suc;
	sscanf(arg,"%d", &num);
	suc = delete_wp(num);
	if (suc == 0) Assert(0, "Invalid Num"); 
	return 0;
}
static void read_ebp (swaddr_t addr , PartOfStackFrame *ebp)
{
	
	ebp -> prev_ebp = swaddr_read (addr , 4, 3);
	ebp -> ret_addr = swaddr_read (addr + 4 , 4, 3);
	int i;
	for (i = 0;i < 4;i ++)
	{
		ebp -> args [i] = swaddr_read (addr + 8 + 4 * i , 4, 3);
	}
}
static int cmd_bt(char *args){
	
	int i,j = 0;
	PartOfStackFrame now_ebp;
	char tmp [32];
	int tmplen;
	swaddr_t addr = reg_l (R_EBP);
	now_ebp.ret_addr = cpu.eip;
	while (addr > 0)
	{
		printf ("#%d  0x%08x in ",j++,now_ebp.ret_addr);
		for (i=0;i<nr_symtab_entry;i++)
		{
			if (symtab[i].st_value <= now_ebp.ret_addr && symtab[i].st_value +  symtab[i].st_size >= now_ebp.ret_addr && (symtab[i].st_info&0xf) == STT_FUNC)
			{
				tmplen = (int)strlen(strtab+symtab[i].st_name);
				strncpy (tmp,strtab+symtab[i].st_name,tmplen);
				tmp [tmplen] = '\0';
				break;
			}
		}
		printf("%s\t",tmp);
		read_ebp (addr,&now_ebp);
		if (strcmp (tmp,"main") == 0)printf ("( )\n");
		else printf ("( %d , %d , %d , %d )\n", now_ebp.args[0],now_ebp.args[1],now_ebp.args[2],now_ebp.args[3]);
		addr = now_ebp.prev_ebp;
		
	}
	return 0;

}
static int cmd_w(char *args){
	if (args == NULL) {
		printf("Invalid Input\n");
		return 0;
	}
	WP *w = new_wp();
	strcpy (w->str, args);
	bool is;
	w->val = expr(args, &is);
	if (!is) Assert(0, "Wroing EXPR\n");
	printf("Set WatchPoint NO.%d for %s, Now val is %d\n", w->NO, w->str, w->val);
	return 0;
}
static int cmd_x(char *args){
	if (args == NULL){
		printf("Invalid Input\n");
		return 0;
	}
	char *arg = strtok(NULL, " ");
	int num; 
	sscanf(arg, "%d", &num);
	char *str = strtok(NULL, " ");
	uint32_t result;
	bool is;
	result = expr(str, &is);
	if (!is) {printf("EXPR WRONG\n"); return 0;}
	int digit;
	int i = 0;
	for (; num > 0; num--){
		digit = swaddr_read(result+i, 4, 3);
		printf("address:0x%x  \t%08x\n",result+i,digit);
		i+=4;
	}	
	return 0;
}
static int cmd_page(char *args){
	if (args == NULL){
		printf("Invalid Input\n");
		return 0;
	}
	char *str = strtok(NULL, " ");
	uint32_t result;
	bool is;
	result = expr(str, &is);
	if (!is) {printf("EXPR WRONG\n"); return 0;}
	int digit;
	result = page_translate(result);
	digit = hwaddr_read(result, 4);
	printf("page address:0x%x  \t%08x\n",result,digit);

	return 0;
}
static int cmd_si(char *args){
	char *arg = strtok(NULL, " ");
	int i;
	if (arg == NULL) {
		cpu_exec(1);
		return 0;
	}
  	sscanf(arg, "%d", &i);
	
	for (; i > 0; i--){
		cpu_exec(1);
	}
	return 0;
}
static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);
		assert(0);
		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}
		
		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
