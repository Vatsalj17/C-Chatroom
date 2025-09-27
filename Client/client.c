#include "../utils/socketutils.h"

#define IP "127.0.0.1"
#define START "\033[1G"

void *listenPrintChatsOfOthers(void *arg) {
    int fd = *(int *)arg;
    free(arg);
    char buffer[BUF_SIZE];
    while (true) {
        ssize_t bytes_received = recv(fd, buffer, BUF_SIZE - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf(START"%s\n", buffer);
            printf("> ");
            fflush(stdout);
        } else if (bytes_received == 0) {
            printf(START"Disconnected.\n");
            break;
        } else {
            perror("recv");
            break;
        }
    }
    close(fd);
    return NULL;
}

void listenOnNewThread(int fd) {
    int *arg = malloc(sizeof(int));
    *arg = fd;
    pthread_t id;
    pthread_create(&id, NULL, listenPrintChatsOfOthers, arg);
}

int main() {
	int socket_fd;
	if ((socket_fd = createTCPipv4Socket()) < 0) {
		perror("socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in *addr;
    if ((addr = createIpv4Address(IP, PORT)) == NULL) {
        printf("Invalid Address\n");
        return EXIT_FAILURE;
    }

	if (connect(socket_fd, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
		perror("connect");
		return EXIT_FAILURE;
	}
    char *name = NULL;
    size_t nameSize = 0;
    printf("Enter you name: ");
    ssize_t byteCount = getline(&name, &nameSize, stdin);
    name[byteCount - 1] = '\0';
    send(socket_fd, name, strlen(name), 0);

    printf("Welcome to my chat app.. (Enter exit to exit)\n");
    listenOnNewThread(socket_fd);

    char *line = NULL;
    size_t lineSize = 0;
    char buffer[BUF_SIZE];
    while (true) {
        printf("> ");
        fflush(stdout);
        ssize_t charCount = getline(&line, &lineSize, stdin);
        if (charCount > 0) {
            if (line[charCount - 1] == '\n') {
                line[charCount - 1] = '\0';
            }
            sprintf(buffer, BBLU"%s:"reset" %s", name, line);
            if (strcmp(line, "exit") == 0) break;
            ssize_t amountSent = send(socket_fd, buffer, strlen(buffer), 0);
        }
    }

    close(socket_fd);
    return EXIT_SUCCESS;
}
