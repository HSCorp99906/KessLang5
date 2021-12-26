#include <stdlib.h>
#include <stdio.h>


int main() {
	unsigned int data[] = {10, 1};

	for (int i = 0; i <= 10; ++i) {
		data[0] = i;
		printf("%d\n", data[0] ^ (data[1] / 4));
	}
}
