#include "mytest.h"

#define SIZE 1024 * 1024
#define INT_CNT ((SIZE) / (sizeof(int)))

int main() {
    
    int data[INT_CNT];

    int fd[2];

    //pipe(fd);

    socketpair(AF_UNIX, SOCK_STREAM, 0, fd);

    fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
    fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);

    int totalBytes = 0;
    
    pid_t pid = fork();

    switch (pid)
    {
    case -1: // error
        perror("pid");
        exit(1);
        break;
    case 0: // child

        printf("IM CHILD\n");

        while (totalBytes < SIZE) {
            int readBytes = read(fd[0], (char *)data + totalBytes, SIZE - totalBytes);
            if (readBytes == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    readBytes = 0;
                }
                else {
                    perror("read");
                    exit(1);
                }
            }
            totalBytes += readBytes;
        }

        for (int i = INT_CNT - 100; i < INT_CNT; i++) {
            printf("%d ", data[i]);
        }

        close(fd[0]);
        close(fd[1]);

        exit(0);
        break;
    default: // parent

        for (int i = 0; i < INT_CNT; i++) {
            data[i] = i;
        }
        printf("IM PARENT\n");

        while (totalBytes < SIZE) {
            int writeBytes = write(fd[1], (char *)data + totalBytes, SIZE - totalBytes);
            if (writeBytes == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    writeBytes = 0;
                }
                else {
                    perror("write");
                    exit(1);
                }
            } 
            totalBytes += writeBytes;
        }

        close(fd[0]);
        close(fd[1]);

        waitpid(pid, NULL, 0);
        break;
    }
}