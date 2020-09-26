#include "trap.h"

int A[10];
int b;
char C[10];
int main() {
	A[0] = 0;
	A[1] = 1;

	C[0] = 'a';
	nemu_assert(C[0] == 'a');
/*
	C[1] = C[0];
	nemu_assert(C[1] == 'a');

	A[0] = (int)C[0];
	nemu_assert(A[0] ==  0x61);

	C[1] = 0x80;
	A[0] = (int)C[1];

	nemu_assert(C[1] == 0xffffff80);
	nemu_assert(A[0] == 0xffffff80);
*/

	return 0;
}
