#include "mytest.h"

int main() {

	createSourceData(); // one-time execution

	printf("Source data created.\n");
	for (int i=10; i>0; i--) {
		printf("%d seconds left to start...\n", i);
		sleep(1);
	}

	clientOrientedIo();
	serverOrientedIo();
	

	return 0;
}
