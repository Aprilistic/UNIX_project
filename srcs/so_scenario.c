#include "mytest.h"

// client에서 4개의 server로 데이터 전송
void soClientDataProcess1(ipcSet *set, int id, int *data) {
    int fdWrite[4];
    
	for (int i = 0; i < 4; i++) {
		fdWrite[i] = getFd(set, id, i, F_WRITE);
	}

    size_t bytesStream[8] = { 0, };

    int writing = (1 << 8) - 1;

    while (writing) {
        for (int i = 0; i < 8; i++) {
            ssize_t bytesWrite = write(fdWrite[i % 4], (char *)data + DATA_SIZE / 8 * i + bytesStream[i], DATA_SIZE / 8 - bytesStream[i]);
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
    int data[INT_COUNT];

    loadData(id, data);

    saveData(IO_SERVER, NODE_COMPUTE, id, data);

	soClientDataProcess1(set, id, data);
}

void soServerScenario(ipcSet *set, int id) {
	int data[INT_COUNT];

	soServerDataProcess1(set, id, data);

	arrange(data, 0, INT_COUNT - 1);

	saveData(IO_SERVER, NODE_IO, id, data);
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
