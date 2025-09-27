#include "connection.h"
#include "message.h"

void sendMessageToOtherClients(char* message, int socket_fd) {
	for (int i = 0; i < BACKLOG; i++) {
		if (acceptedSockets[i].socket_fd == socket_fd || acceptedSockets[i].socket_fd < 0) continue;
		send(acceptedSockets[i].socket_fd, message, strlen(message), 0);
	}
}

void* recieveAndPrintData(void* args) {
	int socket_fd = *(int*)args;
	free(args);
	char buffer[BUF_SIZE];
	while (true) {
		ssize_t bytes_received = recv(socket_fd, buffer, BUF_SIZE - 1, 0);
		if (bytes_received > 0) {
			buffer[bytes_received] = '\0';
			printf("%s\n", buffer);
			sendMessageToOtherClients(buffer, socket_fd);
		} else if (bytes_received == 0) {
			printf("Client disconnected.\n");
            socketsCount--;
			break;
		} else {
			perror("recv");
			break;
		}
	}
	for (int i = 0; i < BACKLOG; i++) {
		if (acceptedSockets[i].socket_fd == socket_fd) {
			acceptedSockets[i].socket_fd = -1;
            char leftMsg[100];
            sprintf(leftMsg, RED"%s left the chat"reset, acceptedSockets[i].name);
            printf("%s\n", leftMsg);
            sendMessageToOtherClients(leftMsg, socket_fd);
			break;
		}
	}
	close(socket_fd);
	return NULL;
}

// worker threads

void recieveAndPrintDataOnSeperateThread(struct AcceptedSocket* pSocket) {
	pthread_t id;

	if (!pSocket->acceptedSuccessfully) {
		free(pSocket);
		return;
	}
	int* arg = malloc(sizeof(int));
	*arg = pSocket->socket_fd;
	free(pSocket);
	pthread_create(&id, NULL, recieveAndPrintData, arg);
}
