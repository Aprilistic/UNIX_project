#include "mytest.h"

// client에서 4개의 server로 데이터 전송
void soClientDataProcess1(ipcSet *set, int id, int *data) {

}

// server에서 4개의 client로부터 데이터 수신
void soServerDataProcess1(ipcSet *set, int id, int *data) {

}

void soClientScenario(ipcSet *set, int id) {

}

void soServerScenario(ipcSet *set, int id) {
    
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
