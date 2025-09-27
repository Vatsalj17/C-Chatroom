#ifndef MESSAGE_H
#define MESSAGE_H

#include "connection.h"

void sendMessageToOtherClients(char* message, int socket_fd);
void* recieveAndPrintData(void* args);
void recieveAndPrintDataOnSeperateThread(struct AcceptedSocket* pSocket);

#endif
