#ifndef GUI_COMM_H
#define GUI_COMM_H

#include "common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define DEFAULT_IPC_RECV_PORT 13131
#define DEFAULT_IPC_SEND_PORT 13132
#define SOCK_PATH  "com.llc24x7lab.console24x7.gui" //TODO put IPC working on unix sockets

extern void doReverseConnect();

int sendMsgToGui(char *msg);
int bindIPCserver();
void unbindIPCserver();
void *handle_connections();

#endif
