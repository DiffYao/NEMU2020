#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
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

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Execute N instructions in a single step and then pause, the default is 1", cmd_si},
	{ "info", "r for  Disply info about register \n \t w for print watchpoint infomation", cmd_info},
	{ "x", "Scanf memory, Disply N Byte", cmd_x},
	{ "p", "Calculate the value of EXPR", cmd_p},
	{ "w", "Set watchpoint for EXPR", cmd_w},
	{ "d", "Delete No.N watchpoint", cmd_d},
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))
static int cmd_info(char *args){
	char* arg = strtok(NULL, " ");
	if (arg == NULL || strlen(arg) != 1 || (arg[0] != 'r' && arg[0] != 'w')){
	printf("Invalid Input\n"); 
	}
	if(strcmp(arg, "r") == 0){
		printf("%%cpu.eax : %#08x\n", cpu.eax);
		printf("%%cpu.ecx : %#08x\n", cpu.ecx);
		printf("%%cpu.edx : %#08x\n", cpu.edx);
		printf("%%cpu.ebx : %#08x\n", cpu.ebx);
		printf("%%cpu.esp : %#08x\n", cpu.esp);
		printf("%%cpu.ebp : %#08x\n", cpu.ebp);
		printf("%%cpu.esi : %#08x\n", cpu.esi);
		printf("%%cpu.edi : %#08x\n", cpu.edi);
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
		printf("result is %d\n", num);
	}	
	
	return num;
}
static int cmd_d(char *args){
	return 0;
}
static int cmd_w(char *args){
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
	sscanf(str, "%x", &result);
	int digit;
	int i = 0;
	for (; num > 0; num--){
		digit = swaddr_read(result+i, 4);
		printf("0x%08x\n", digit);
		i+=4;
	}	
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
