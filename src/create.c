#include "mytest.h"

const char *sourceFile[] = {"./data/p0.dat", "./data/p1.dat", "./data/p2.dat",
                            "./data/p3.dat"};

int makeData(int id) {
	int fd = open(sourceFile[id], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < INT_COUNT; i++) {
		int num = i * 4 + id + 1;
		write(fd, &num, sizeof(int));
	}

	close(fd);
}

int create_source_data() {
	for (int i = 0; i < 4; i++) {
		pid_t pid = fork();
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
			break;
		}
	}
}