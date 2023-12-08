#include "mytest.h"

// client 간 데이터 교환
void coClientDataProcess1(ipcSet *set, int id, int *data) {
    int writing = (1 << 4) - 1;
    int reading = (1 << 4) - 1;

    writing &= ~(1 << id);
    reading &= ~(1 << id);

    int buf[INT_COUNT];

    for (int i = 0; i < INT_COUNT / 4; i++) {
        buf[i + INT_COUNT / 4 * id] = data[i + INT_COUNT / 4 * id];
    }

    size_t bytesStream[4][4] = { 0, };

    while (writing || reading) {
        for (int i = 0; i < 4; i++) {
            if (i == id) continue;

            int fdWrite = getFd(set, id, i, F_WRITE);
            int fdRead = getFd(set, i, id, F_READ);

            if (writing & (1 << i)) {
                ssize_t bytesWrite = write(fdWrite, (char *)data + DATA_SIZE / 4 * i + bytesStream[id][i], DATA_SIZE / 4 - bytesStream[id][i]);
                if (bytesWrite == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        bytesWrite = 0;
                    }
                    else {
                        perror("write");
                        exit(1);
                    }
                }
                bytesStream[id][i] += (size_t)bytesWrite;
                if ((int)bytesStream[id][i] == DATA_SIZE / 4) {
                    writing &= ~(1 << i);
                }
            }

            if (reading & (1 << i)) {
                ssize_t bytesRead = read(fdRead, (char *)buf + DATA_SIZE / 4 * i + bytesStream[i][id], DATA_SIZE / 4 - bytesStream[i][id]);
                if (bytesRead == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        bytesRead = 0;
                    }
                    else {
                        perror("write");
                        exit(1);
                    }
                }
                bytesStream[i][id] += (size_t)bytesRead;
                if ((int)bytesStream[i][id] == DATA_SIZE / 4) {
                    reading &= ~(1 << i);
                }                
            }

        }

    }

    for (int i = 0; i < INT_COUNT; i++) {
        data[i] = buf[i];
    }
}

// client에서 2개의 server로 데이터 전송
void coClientDataProcess2(ipcSet *set, int id, int *data) {
    int fdWrite[2];
    fdWrite[0] = getFd(set, id, (id * 2) % 4, F_WRITE);
    fdWrite[1] = getFd(set, id, (id * 2 + 1) % 4, F_WRITE);

    size_t bytesStream[2] = { 0, };

    int writing = (1 << 2) - 1;

    while (writing) {
        for (int i = 0; i < 2; i++) {
            ssize_t bytesWrite = write(fdWrite[i], (char *)data + DATA_SIZE / 2 * i + bytesStream[i], DATA_SIZE / 2 - bytesStream[i]);
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
            if ((int)bytesStream[i] == DATA_SIZE / 2) {
                writing &= ~(1 << i);
            }
        }
    }
}

// server에서 2개의 client로부터 데이터 수신
void coServerDataProcess1(ipcSet *set, int id, int *data) {
    int fdRead[2];
    fdRead[0] = getFd(set, (id < 2 ? 0 : 1), id, F_READ);
    fdRead[1] = getFd(set, (id < 2 ? 2 : 3), id, F_READ);

    size_t bytesStream[2] = { 0, };

    int reading = (1 << 2) - 1;

    while (reading) {
        for (int i = 0; i < 2; i++) {
            ssize_t bytesRead = read(fdRead[i], (char *)data + DATA_SIZE / 2 * i + bytesStream[i], DATA_SIZE / 2 - bytesStream[i]);
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
            if ((int)bytesStream[i] == DATA_SIZE / 2) {
                reading &= ~(1 << i);
            }
        }
    }
}

void coClientScenario(ipcSet *set, int id) {
    int data[INT_COUNT];

    loadData(id, data);

    coClientDataProcess1(&set[0], id, data);

    arrange(data, 0, INT_COUNT - 1);

    saveData(IO_CLIENT, NODE_COMPUTE, id, data);

    coClientDataProcess2(&set[1], id, data);
}

void coServerScenario(ipcSet *set, int id) {
    int data[INT_COUNT];

    coServerDataProcess1(&set[1], id, data);

    saveData(IO_CLIENT, NODE_IO, id, data);
}

void coRun() {
	ipcSet setCO[2];

	toggleSockCO1(&setCO[0], IPC_ON);
	toggleSockCO2(&setCO[1], IPC_ON);

	pid_t client[4], server[4], pid;

	for (int i = 0; i < 4; i++) {
		pid = fork();
		switch (pid) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			coClientScenario(setCO, i);
			toggleSockCO1(&setCO[0], IPC_OFF);
			toggleSockCO2(&setCO[1], IPC_OFF);
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
			coServerScenario(setCO, i);
			toggleSockCO1(&setCO[0], IPC_OFF);
			toggleSockCO2(&setCO[1], IPC_OFF);
			exit(EXIT_SUCCESS);
			break;
		default:
			server[i] = pid;
			break;
		}
	}

	toggleSockCO1(&setCO[0], IPC_OFF);
	toggleSockCO2(&setCO[1], IPC_OFF);

	for (int i = 0; i < 4; i++) {
		waitpid(client[i], NULL, 0);
		waitpid(server[i], NULL, 0);
	}
}