#include "nemu.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM, HEXNUM, NEQ, REGISTER, AND, OR, MINUS, STAR, MARK

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
        {"\\(", '('},                                   // leftpa 40
        {"\\)", ')'},                                   // rightpa 41
	{"0x[0-9a-fA-F]+", HEXNUM},			// hexadecimal number 259
	{"[0-9]+", NUM},                                // number 258
	{"\\$[a-zA-Z]+", REGISTER},			// register
	{"[a-zA-Z_0-9]+", MARK},			// mark

	{" +",	NOTYPE},				// spaces 256
	{"\\+", '+'},					// plus 43 
	{"==", EQ},					// equal 257
	{"!=", NEQ},					// not equal 259
	{"\\-", '-'},					// sub  45
	{"/", '/'},					// div  
	{"\\*", '*'},					// multi 42
	{"!", '!'}, 					// not
	{"&&", AND},					// and
	{"\\|\\|", OR}					// or	
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
			//	printf("s = %s\n", substr_start);
			//	printf("len = %d\n", substr_len);
				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE : break;
					case REGISTER: {
						tokens[nr_token].type = rules[i].token_type;
						strncpy (tokens[nr_token].str, substr_start+1, substr_len-1);
						nr_token++;
					
						break;
					}
					default: {
						tokens[nr_token].type = rules[i].token_type;
						strncpy (tokens[nr_token].str, substr_start, substr_len);
						nr_token++;
						break;				
					}
					
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("NR_REGEX is %lx\n", NR_REGEX);
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}
// check parentheses matching
bool check_parentheses(int p, int q)
{
	int i;
	int count = 0;
	if (tokens[p].type == '(' && tokens[q].type == ')')
	{
		for (i = p + 1; i < q; i++){
			if (tokens[i].type == '(') count++;
			if (tokens[i].type == ')') count--;
			if (count < 0) {
				return false;
			}
		}
		if (count == 0) return true;
	}
	return false;
}

int dominant_operator(int p, int q)
{
	int i;
	int is = 0;
	int result = p;
	int priority = 100;
	for (i = q; i >= p; i--)
	{
		//Log("i is %d, type is %c\n", i, tokens[i].type);
		if (tokens[i].type == ')') is++;
		if (tokens[i].type == '(') is--;
		if (tokens[i].type == NUM || tokens[i].type == HEXNUM || tokens[i].type == REGISTER|| tokens[i].type == MARK|| is != 0) continue;
		if (tokens[i].type == AND || tokens[i].type == OR ) return i;
		if ((tokens[i].type == EQ  || tokens[i].type == NEQ) && priority > 1) { priority = 1; result = i;}
		if ((tokens[i].type == '+' || tokens[i].type == '-') && priority > 2) { priority = 2; result = i;}
		if ((tokens[i].type == '*' || tokens[i].type == '/') && priority > 3) { priority = 3; result = i;}
		if ((tokens[i].type == STAR || tokens[i].type == MINUS || tokens[i].type == '!') && priority > 4) { priority = 4; result = i; }
	}
	
	return result;
}
// evaluate expression
uint32_t eval (int p, int q)
{
	
	if (p > q) 
	{
		panic("Wrong expression\n");
		return 0;
	} 
	else if (p == q)
	{	
		uint32_t i;
		char* check = tokens[p].str;
		if (tokens[p].type == MARK)
		{
			int i;
			for (i=0;i< nr_symtab_entry;i++)
			{
				if ((symtab[i].st_info & 0xf) == STT_OBJECT)
				{
				
					char tmp [32];
					//int tmplen = symtab[i+1].st_name - symtab[i].st_name - 1;
					char* temp2 = strtab + symtab[i].st_name;
					int tmplen = strlen(temp2);
					//printf("str = %s, len = %x\n", temp2, (int)strlen(temp2));
					//printf("tmp2len = %d\n", tmplen);
					strncpy (tmp, temp2 ,tmplen);
					//Log("3\n");
					tmp [tmplen] = '\0';
					if (strcmp (tmp, check) == 0)
					{
						return symtab[i].st_value;
					}
				}
			}
			
		}	
		if (tokens[p].type == HEXNUM) { sscanf(check, "%x", &i); return i;}
		if (tokens[p].type == NUM)    { sscanf(check, "%u", &i); return i;}
		if (tokens[p].type == REGISTER) {
			int k;
			//Log("register is %s\n", check);
			for (k = R_EAX; k <= R_EDI; k++){
				if (strcmp (check, regsl[k]) == 0) return reg_l(k); 	
			}
			if (strcmp (check, "eip") == 0) return cpu.eip;
			for (k = R_AX; k <= R_DI; k++) {
				if (strcmp (check, regsw[k]) == 0) return reg_w(k);
			}	
			for (k = R_AL; k <= R_AH; k++) {
				if (strcmp (check, regsb[k]) == 0) return reg_b(k);
			}
			Assert(0, "Register Not Found\n");
		}
		Assert(0,"tokens[p].type is %s", tokens[p].str);
	}
	else if (check_parentheses(p, q) == true){

		return eval (p+1, q-1);
	}else {
		//Log("p = %d, q= %d\n", p, q);
		int op = dominant_operator(p, q);
		//Log("op is %d\n", op);
		if (p == op){
			uint32_t val = eval(p+1, q);
			switch (tokens[op].type) {
				case MINUS : return -val;
				case STAR  : return swaddr_read(val, 4, 3);
				case '!'   : return !val;
				default:  Assert(0, "Wrong Token Type\n"); 
			} 
		}
		
		uint32_t val1 = eval(p, op-1);
		uint32_t val2 = eval(op+1, q);
	
		switch (tokens[op].type){
			case '+' : return val1 + val2;
			case '-' : return val1 - val2;
			case '*' : return val1 * val2;
			case '/' : return val1 / val2;
			case EQ  : return val1 == val2;
			case NEQ : return val1 != val2;
			case AND : return val1 && val2;
			case OR  : return val1 || val2;
			default : assert(0);
		}
	
	}
}
uint32_t expr(char *e, bool *success) {
	if (strlen(e) > 32) panic("EXPR is too long\n");
	if(!make_token(e)) {
		*success = false;
		Log("False \n");
		return 0;
	}
	*success = true;
	int i = 0;
	for (; i < nr_token; i++){
		int check = i > 0?  tokens[i-1].type : 0;
		if (tokens[i].type == '*' && (i == 0 || (check != NUM && check != HEXNUM && check != REGISTER && check != ')')))	tokens[i].type = STAR;
		if (tokens[i].type == '-' && (i == 0 || (check != NUM && check != HEXNUM && check != REGISTER && check != ')')))   	tokens[i].type = MINUS;
	//	printf("i = %d, str = %s, type is %d\n", i, tokens[i].str, tokens[i].type);
	}
	
		
	return eval(0, nr_token-1);
}

