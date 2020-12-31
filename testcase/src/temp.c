#include "trap.h"


int main() {
	set_bp();
	nemu_assert(1);
	return 0;
}
