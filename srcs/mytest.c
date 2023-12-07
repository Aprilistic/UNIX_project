#include "mytest.h"

int main() {

	createSourceData(); // one-time execution

	clientOrientedIo();
	serverOrientedIo();
	printf("Client_oriented_io time result\n");
	printf("Server_oriented_io time result\n");

	return 0;
}
