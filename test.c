#include <stdio.h>


int main() {
	__asm__("push %0": : "i" (2));
	register int* val asm("esp");
	printf("%d\n", *
	__asm__("pop rax");
}
