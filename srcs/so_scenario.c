#include "mytest.h"

// client에서 4개의 server로 데이터 전송
void soClientDataProcess1(ipcSet *set, int id, int *data) {
    int fdWrite[4];
    
	for (int i = 0; i < 4; i++) {
		fdWrite[i] = getFd(set, id, i, F_WRITE);
	}


    sortData(data);

    size_t bytesStream[4] = { 0, };

    int writing = (1 << 4) - 1;

    while (writing) {
        for (int i = 0; i < 4; i++) {
            ssize_t bytesWrite = write(fdWrite[i % 4], (char *)data + DATA_SIZE / 4 * i + bytesStream[i], DATA_SIZE / 4 - bytesStream[i]);
            if (bytesWrite == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    bytesWrite = 0;
                }
                else {
                    perror("write");
                    exit(1);
                }                
            }
            bytesStream[i] += (size_t)bytesWrite;
            if ((int)bytesStream[i] == DATA_SIZE / 8) {
                writing &= ~(1 << i);
            }
        }
    }
}

// server에서 4개의 client로부터 데이터 수신
void soServerDataProcess1(ipcSet *set, int id, int *data) {
    int fdRead[4];

	for (int i = 0; i < 4; i++) {
		fdRead[i] = getFd(set, i, id, F_READ);
	}

    size_t bytesStream[4] = { 0, };

    int reading = (1 << 4) - 1;

    while (reading) {
        for (int i = 0; i < 4; i++) {
            ssize_t bytesRead = read(fdRead[i], (char *)data + DATA_SIZE / 4 * i + bytesStream[i], DATA_SIZE / 4 - bytesStream[i]);
            if (bytesRead == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    bytesRead = 0;
                }
                else {
                    perror("read");
                    exit(1);
                }                
            }
            bytesStream[i] += (size_t)bytesRead;
            if ((int)bytesStream[i] == DATA_SIZE / 4) {
                reading &= ~(1 << i);
            }
        }
    }
}

void soClientScenario(ipcSet *set, int id) {

#ifdef TIMES
	struct timeval sCompTime, eCompTime, sCommTime, eCommTime;
	struct timeval time_result;
	long int elapsed_time;
	gettimeofday(&sCompTime, NULL);
#endif

    int data[INT_COUNT];

    loadData(id, data);

    saveData(IO_SERVER, NODE_COMPUTE, id, data);

#ifdef TIMES
	gettimeofday(&eCompTime, NULL);
	time_result.tv_sec = eCompTime.tv_sec - sCompTime.tv_sec;
	time_result.tv_usec = eCompTime.tv_usec - sCompTime.tv_usec;

	// Handle case where end microseconds is less than start microseconds
	if (eCompTime.tv_usec < sCompTime.tv_usec) {
    	time_result.tv_sec--;
    	time_result.tv_usec += 1000000;
	}

	// Convert total time to milliseconds
	elapsed_time = time_result.tv_sec * 1000 + time_result.tv_usec / 1000;

	printf("SO - Client %d Comp time: %ld ms\n", id, elapsed_time);
#endif

#ifdef TIMES
	gettimeofday(&sCommTime, NULL);
#endif

	soClientDataProcess1(set, id, data);

#ifdef TIMES
	gettimeofday(&eCommTime, NULL);
	time_result.tv_sec = eCommTime.tv_sec - sCommTime.tv_sec;
	time_result.tv_usec = eCommTime.tv_usec - sCommTime.tv_usec;

	// Handle case where end microseconds is less than start microseconds
	if (eCommTime.tv_usec < sCommTime.tv_usec) {
    	time_result.tv_sec--;
    	time_result.tv_usec += 1000000;
	}

	// Convert total time to milliseconds
	elapsed_time = time_result.tv_sec * 1000 + time_result.tv_usec / 1000;

	printf("SO - Client %d Comm time: %ld ms\n", id, elapsed_time);
#endif
}

void soServerScenario(ipcSet *set, int id) {
#ifdef TIMES
	struct timeval sCompTime, eCompTime, sCommTime, eCommTime;
	struct timeval time_result;
	long int elapsed_time;
	gettimeofday(&sCompTime, NULL);
#endif

    int data[INT_COUNT];

	soServerDataProcess1(set, id, data);

#ifdef TIMES
	gettimeofday(&eCompTime, NULL);
	time_result.tv_sec = eCompTime.tv_sec - sCompTime.tv_sec;
	time_result.tv_usec = eCompTime.tv_usec - sCompTime.tv_usec;

	// Handle case where end microseconds is less than start microseconds
	if (eCompTime.tv_usec < sCompTime.tv_usec) {
    	time_result.tv_sec--;
    	time_result.tv_usec += 1000000;
	}

	// Convert total time to milliseconds
	elapsed_time = time_result.tv_sec * 1000 + time_result.tv_usec / 1000;

	printf("SO - Server %d Comm time: %ld ms\n", id, elapsed_time);
#endif

#ifdef TIMES
	gettimeofday(&sCommTime, NULL);
#endif

	arrange(data, 0, INT_COUNT - 1);

	saveData(IO_SERVER, NODE_IO, id, data);

#ifdef TIMES
	gettimeofday(&eCommTime, NULL);
	time_result.tv_sec = eCommTime.tv_sec - sCommTime.tv_sec;
	time_result.tv_usec = eCommTime.tv_usec - sCommTime.tv_usec;

	// Handle case where end microseconds is less than start microseconds
	if (eCommTime.tv_usec < sCommTime.tv_usec) {
    	time_result.tv_sec--;
    	time_result.tv_usec += 1000000;
	}

	// Convert total time to milliseconds
	elapsed_time = time_result.tv_sec * 1000 + time_result.tv_usec / 1000;

	printf("SO - Server %d IO time: %ld ms\n", id, elapsed_time);
#endif
}


void soRun() {
	ipcSet setSO;

	togglePipeSO1(&setSO, IPC_ON);

	pid_t client[4], server[4], pid;

	for (int i = 0; i < 4; i++) {
		pid = fork();
		switch (pid) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			soClientScenario(&setSO, i);
			togglePipeSO1(&setSO, IPC_OFF);
			exit(EXIT_SUCCESS);
			break;
		default:
			client[i] = pid;
			break;
		}
	}

	for (int i = 0; i < 4; i++) {
		pid = fork();
		switch (pid) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			soServerScenario(&setSO, i);
			togglePipeSO1(&setSO, IPC_OFF);
			exit(EXIT_SUCCESS);
			break;
		default:
			server[i] = pid;
			break;
		}
	}

	togglePipeSO1(&setSO, IPC_OFF);

	for (int i = 0; i < 4; i++) {
		waitpid(client[i], NULL, 0);
		waitpid(server[i], NULL, 0);
	}
}
