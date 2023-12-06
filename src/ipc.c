#include "mytest.h"

int getFd(ipcSet *set, chType ch, int from, int to, trfType trf) {
    int fd = set->fd[from][to][trf];
    return fd;
}

// fd[a][b]에 대해, a가 쓰는 주체고 b가 읽는 주체이다.
// 따라서 fd[a][b][0]의 경우, b가 a로부터 읽는 endpoint이다.
// 반대로 fd[a][b][1]의 경우, a가 b에 쓰는 endpoint이다.
int openChannel(ipcSet *set, ipcType ipc, dirType dir, int from, int to) {
    int fd[2];
    switch (ipc) {
    case IPC_SOCK:
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1) {
            perror("socketpair");
            exit(EXIT_FAILURE);
        }

        // fd[1]은 from이 사용하고, fd[0]은 to가 사용한다.
        // 일반화를 위해 같은 fd를 2개의 변수에 나눠 저장한다.
        set->fd[from][to][0] = fd[0]; // to에서 from으로부터 읽는다.
        set->fd[from][to][1] = fd[1]; // from에서 to로 쓴다.
        if (dir == DIR_BI) {
            set->fd[to][from][0] = fd[1]; // from에서 to로부터 읽는다.
            set->fd[to][from][1] = fd[0]; // from에서 to로 쓴다.
        }
        break;
    case IPC_PIPE:
        if (pipe(fd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        set->fd[from][to][0] = fd[0]; // to에서 from으로부터 읽는다.
        set->fd[from][to][1] = fd[1]; // from에서 to로 쓴다.
        break;
    default:
        printf("Invalid IPC Type\n");
        exit(EXIT_FAILURE);
        break;
    }
}

int closeChannel(ipcSet *set, ipcType ipc, dirType dir, int from, int to) {
    close(set->fd[from][to][0]);
    close(set->fd[from][to][1]);
    switch (ipc) {
    case IPC_SOCK:
        set->fd[from][to][0] = -1;
        set->fd[from][to][1] = -1;
        if (dir == DIR_BI) {
            set->fd[to][from][0] = -1; // from에서 to로부터 읽는다.
            set->fd[to][from][1] = -1; // from에서 to로 쓴다.
        }
        break;
    case IPC_PIPE:
        set->fd[from][to][0] = -1;
        set->fd[from][to][1] = -1;
        break;
    default:
        printf("Invalid IPC Type\n");
        exit(EXIT_FAILURE);
        break;
    }    
}

// 개수: 4P3 / 2(양방향) = 12
void toggleSockC1(ipcSet *set, ipcState state) {
    for (int from = 0; from < 4; from++) {
        for (int to = 0; to < 4; to++) {
            if (from >= to) continue;
            switch (state) {
            case IPC_ON:
                openChannel(set, IPC_SOCK, DIR_BI, from, to);
                break;
            default:
                closeChannel(set, IPC_SOCK, DIR_BI, from, to);
                break;
            }
        }
    }
}

// 개수: 4 * 2 = 8
void toggleSockC2(ipcSet *set, ipcState state) {
    for (int from = 0; from < 4; from++) {
        for (int to = 0; to < 4; to++) {
            if (from * 2 != to && from * 2 + 1 != to) continue;
            switch (state) {
            case IPC_ON:
                openChannel(set, IPC_SOCK, DIR_MONO, from, to);
                break;
            default:
                closeChannel(set, IPC_SOCK, DIR_MONO, from, to);
                break;
            }
        }
    }
}

// 개수: 4 * 4 = 16
void togglePipeS1(ipcSet *set, ipcState state) {
    for (int from = 0; from < 4; from++) {
        for (int to = 0; to < 4; to++) {
            switch (state) {
            case IPC_ON:
                openChannel(set, IPC_PIPE, DIR_MONO, from, to);
                break;
            default:
                closeChannel(set, IPC_PIPE, DIR_MONO, from, to);
                break;
            }
        }
    }
}