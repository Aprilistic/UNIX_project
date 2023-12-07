#include "mytest.h"

const char *sourceFile[] = {"./data/p0.dat", "./data/p1.dat", "./data/p2.dat",
                            "./data/p3.dat"};

void makeData(int id) {
	int fd = open(sourceFile[id], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < (int)INT_COUNT; i++) {
		int num = i * 4 + id + 1;
		if (write(fd, &num, sizeof(int)) < 0) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	
	close(fd);
}

void createSourceData() {
	pid_t child[4], pid;

	for (int i = 0; i < 4; i++) {
		pid = fork();
		switch (pid)
		{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			makeData(i);
			exit(EXIT_SUCCESS);
		default:
			child[i] = pid;
			break;
		}
	}

	for (int i = 0; i < 4; i++) {
		waitpid(child[i], NULL, 0);
	}
}

void loadData(int id, int* data) {
	int fd = open(sourceFile[id], O_RDONLY);

	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < (int)INT_COUNT; i++) {
		if (read(fd, (char *)data + i * sizeof(int), sizeof(int)) < 0) {
			perror("read");
			exit(EXIT_FAILURE);
		}
	}
	
	close(fd);	
}

void saveData(ioType io, int id, int* data) {
	char saveFile[100] = "./data/IO_";

	switch (io) {
	case IO_CLIENT:
		strcat(saveFile, "CLIENT_");
		break;
	case IO_SERVER:
		strcat(saveFile, "SERVER_");
		break;
	default:
		printf("Invalid IO Type\n");
		exit(EXIT_FAILURE);
		break;
	}

	char num[2] = {id + '0', '\0'};
	strcat(saveFile, num);

	strcat(saveFile, ".dat");

	int fd = open(saveFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < (int)INT_COUNT; i++) {
		if (write(fd, &data[i], sizeof(int)) < 0) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}

	close(fd);
}