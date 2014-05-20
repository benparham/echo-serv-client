#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>


// #define BACKLOG 				10
#define HOST_LOOKUP_CMD 		"ifconfig | grep -P 'inet (?!127.0.0.1)'"

// // Initialize all overhead
// static int bootstrap() {

// 	// Add other bootstraps to this with ||
// 	if (varMapBootstrap()) {
// 		return 1;
// 	}

// 	return 0;
// }

// static void cleanup() {
// 	varMapCleanup();
// }


void *listenToClient(void *tempArgs) {

	// Setup global connection struct for thread
	threadArgs *args = (threadArgs *) tempArgs;
	connection *con;
	if (connectionCreate(&con, args)) {
		goto exit;
	}

	// Begin command loop
	int done = 0;
	while (!done) {
		if (connectionReceiveCommand(con)) {
			done = connectionSendError(con);
		} else {
			if (executeCommand(con)) {//con->tbl, con->cmd, con->res, con->err)) {
				done = connectionSendError(con);
			} else {
				done = connectionSendResponse(con);
			}
		}
	}

	// Destroy thread's global connection struct
	connectionDestroy(con);

exit:
	pthread_exit(NULL);
}

static void handleOption(char *opt) {
	if (strcmp(opt, "-h") == 0) {
		printf("Help Menu goes here\n");
		return;
	}

	printf("Unknown option specified\n");
}

int main(int argc, char *argv[]) {

	// // Check optional args
	// int portNumber;
	// if (argc == 2) {
	// 	portNumber = atoi(argv[1]);
	// }
	// else {
	// 	// Obtain http service
	// 	struct servent *serv  = getservbyname("http", "tcp");
	// 	portNumber = htons(serv->s_port);
	// 	endservent();
	// }
	
	int portNumber = -1;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (strncmp(argv[i], "-", 1) == 0) {
				handleOption(argv[i]);
			} else {
				portNumber = atoi(argv[i]);
			}
		}
	}

	if (portNumber == -1) {
		// Obtain http service
		struct servent *serv  = getservbyname("http", "tcp");
		portNumber = htons(serv->s_port);
		endservent();
	}

	printf("Initiating Echo Server...\n\n");
	
	// struct servent *serv;
	struct sockaddr_in addr;
	
	// Create socket
	int sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockListen < 0) {
		printf("Failed to create listening socket\n");
		return 1;
	}
	else {
		printf("Created socket with file descriptor: %d\n", sockListen);
	}
	
	// Setup the address (port number) to bind to
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = portNumber;
	addr.sin_addr.s_addr = INADDR_ANY;
	
	// Bind the socket to the address
	if (bind(sockListen, (struct sockaddr *) &addr, sizeof(addr)) == 0) {
		printf("Successfully bound socket to port %d (%d)\n", ntohs(addr.sin_port), addr.sin_port);
	}
	else {
		printf("Failed to bind socket to port %d\n", ntohs(addr.sin_port));
		close(sockListen);
		return 1;
	}
	
	// Listen on socket
	if (listen(sockListen, BACKLOG) == 0) {
		printf("Listening on socket %d\n", sockListen);

		// Get current hosting ip address
		FILE *ptr = popen(HOST_LOOKUP_CMD, "r");
		if (ptr != NULL) {
			printf("Host(s): ");

			char buf[BUFSIZE];
			char *success;
			do {
				memset(buf, 0, BUFSIZE);
				success = fgets(buf, BUFSIZE, ptr);
				if (buf != NULL) {
					printf("%s", buf);
				}
			} while(success != NULL);
		}
	}
	else {
		printf("Failed listen on socket %d\n", sockListen);
		close(sockListen);
		return 1;
	}

	// Run bootstrap for overhead initialization
	if (bootstrap()) {
		printf("Failed to initialize overhead in bootstrap\n");
		close(sockListen);
		return 1;
	}
	
	struct sockaddr clientAddress;
	socklen_t addressLen;
	
	// Loop for accepting connections
	printf("Ready for client connections...\n");
	while (1) {
		int sockAccept = accept(sockListen, &clientAddress, &addressLen);
		if (sockAccept == -1) {
			printf("Failed to accept connection on listening socket %d\n", sockListen);
		}
		else {
			printf("Accepted new connection. Created socket with file descriptor: %d\n", sockAccept);

			pthread_t newThread;
			threadArgs *args = malloc(sizeof(threadArgs));
			args->socketFD = sockAccept;

			if (pthread_create(&newThread, NULL, listenToClient, (void *) args)) {
				printf("Failed to create thread for connection with file descriptor: %d\n", sockAccept);
			}
		}
	}
	
	// Cleanup
	close(sockListen);
	cleanup();

	return 0;
}