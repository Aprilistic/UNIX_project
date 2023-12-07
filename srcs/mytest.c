#include "mytest.h"

int main() {

	create_source_data(); // one-time execution

	client_oriented_io();
	server_oriented_io();
	printf("Client_oriented_io time result\n");
	printf("Server_oriented_io time result\n");

	return 0;
}
