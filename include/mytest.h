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

#define DATA_SIZE 1024 * 1024
#define INT_COUNT ((DATA_SIZE) / (sizeof(int)))

typedef enum {IO_CLIENT, IO_SERVER} ioType;
typedef enum {CH_SHAK, CH_PASS} chType;
typedef enum {IPC_SOCK, IPC_PIPE} ipcType;
typedef enum {TRF_SEND, TRF_RECV} trfType;
typedef enum {P_CLIENT, P_SERVER} pType;

typedef enum {DIR_MONO, DIR_BI} dirType;
typedef enum {IPC_OFF, IPC_ON} ipcState;


// chType과 ipcType에 따라 각각 생성
typedef struct {
    int fd[4][4][2]; 
} ipcSet;

int create_source_data();
int client_oriented_io();
int server_oriented_io();


int getFd(ipcSet *set, chType ch, int from, int to, trfType trf);
void toggleSockC1(ipcSet *set, ipcState state);
void toggleSockC2(ipcSet *set, ipcState state);
void togglePipeS1(ipcSet *set, ipcState state);