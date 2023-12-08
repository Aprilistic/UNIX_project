#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#ifndef TIMES
#define TIMES
#endif 

// #define DATA_SIZE 1024 * 1024
#define INT_COUNT ((int)((DATA_SIZE) / (sizeof(int))))

typedef enum {IO_CLIENT, IO_SERVER} ioType;
typedef enum {NODE_COMPUTE, NODE_IO} nodeType;
typedef enum {CH_SHAKE, CH_PASS} chType;
typedef enum {IPC_SOCK, IPC_PIPE} ipcType;
typedef enum {P_CLIENT, P_SERVER} pType;

typedef enum {F_READ, F_WRITE} fileMode;
typedef enum {DIR_MONO, DIR_BI} dirType;
typedef enum {IPC_OFF, IPC_ON} ipcState;

typedef struct {
    int fd[4][4][2]; 
} ipcSet;

void createSourceData();
void clientOrientedIo();
void serverOrientedIo();

void loadData(int id, int* data);
void saveData(ioType io, nodeType node, int id, int* data);

void arrange(int *data, int start, int end);

int getFd(ipcSet *set, int from, int to, fileMode mode);
void toggleSockCO1(ipcSet *set, ipcState state);
void toggleSockCO2(ipcSet *set, ipcState state);
void togglePipeSO1(ipcSet *set, ipcState state);

void coRun();
void soRun();