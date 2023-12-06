#include "mytest.h"

int client_oriented_io() {

	#ifdef TIMES
		struct timeval stime, etime;
		int time_result;
	#endif
		/* Client_oriented_io. Measure io time, communication time, and time for the rest.
			*/

	#ifdef TIMES
		gettimeofday(&stime, NULL);
	#endif

	progress();

	#ifdef TIMES
		gettimeofday(&etime, NULL);
		time_result = etime.tv_usec - stime.tv_usec;
		printf("Client_oriented_io TIMES == %ld %ld %ld\n", etime.tv_usec, stime.tv_usec, time_result);
	#endif

}

int progress() {
	ipcSet setC1, setC2;

	toggleSockC1(&setC1, IPC_ON);
	toggleSockC2(&setC2, IPC_ON);

	pid_t client[4], server[4], pid;
	pType type;
	int id;

	for (int i = 0; i < 4; i++) {
		pid = fork();
		switch (pid) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			break;
		default:
			break;
		}
	}
}