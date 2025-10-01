#ifndef MESSAGE_H
#define MESSAGE_H

char* getUsernameByFd(int fd);
void handlePrivateChats(char *message, int sender_fd);
void sendMessageToOtherClients(char* message, int socket_fd);
void handle_command(char *buffer, int fd);
void* recieveAndPrintData(void* args);
void recieveAndPrintDataOnSeperateThread(struct AcceptedSocket* pSocket);

#endif
