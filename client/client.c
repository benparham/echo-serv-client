#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>

#include <transfer.h>
#include <option-parser.h>


#define MAX_TARGET_ADDRESS_LEN		128
#define DEFAULT_TARGET_ADDRESS 		"127.0.0.1"

#define MAX_INPUT 					1024
#define MAX_RESPONSE				1024

static void getInput(int socket_fd) {

	// Detect whether we are in interactive mode
	int inAtty = isatty(0);
	int outAtty = isatty(1);

	char *input = malloc(sizeof(char) * MAX_INPUT);
	char *response = malloc(sizeof(char) * MAX_RESPONSE);
	
	while(1) {
		memset(input, 0, sizeof(input));
		memset(response, 0, sizeof(response));
		
		// Prompt/get input
		if (inAtty) {
			printf(">: ");
		}
		fgets(input, MAX_INPUT, stdin);
		char *temp_input = strtok(input, "\n");

		if (temp_input == NULL ||
			strcmp(temp_input, "") == 0 ||
			strcmp(temp_input, "\n") == 0) {
			continue;
		}

		input = temp_input;

		// Display input if input is not from terminal but output is
		if (!inAtty && outAtty) {
			printf("%s", input);
		}

		int size_bytes = strlen(input) + 1;
		if (tf_send(socket_fd, input, size_bytes)) {
			printf("Failed to send '%s' to server\n", input);
		} else {
			printf("Sent '%s' to server\n", input);
		}

		if (strcmp(input, "exit") == 0) {
			break;
		}
	}

	free(input);
	free(response);
}

static int help(void *native_args, int num_user_args, char **user_args) {
	assert(native_args == NULL);

	if (num_user_args != 0) {
		printf("Invalid number of arguments\n");
		return 1;
	}

	printf("Usage:\n");
	printf("-h\t\t\t -> help menu\n");
	printf("-a <target address>\t -> specify target server address\n");

	return 1;
}

static int set_target_address(void *native_args, int num_user_args, char **user_args) {
	if (num_user_args != 1) {
		printf("Invalid number of arguments\n");
		return 1;
	}

	*((char **) native_args) = user_args[0];

	return 0;
}

static int set_file(void *native_args, int num_user_args, char **user_args) {
	if (num_user_args != 1) {
		printf("Invalid number of arguments\n");
		return 1;
	}

	char *filename = user_args[0];

	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("Unable to open file: '%s'\n", filename);
		return 1;
	}

	*((FILE **) native_args) = fp;

	return 0;
}


int main(int argc, char *argv[]) {
	
	char *target_address = DEFAULT_TARGET_ADDRESS;
	FILE *fp = NULL;

	opt_item items[] = {
		{NULL, 0, "-h", &help},
		{&target_address, 1, "-a", &set_target_address},
		{&fp, 1, "-f", &set_file}
	};
	opt_parser parser = {.num_opts = 3, .items = items};

	if (opt_parse(argc, argv, &parser)) {
		goto exit;
	}

	printf("Initiating Simple Messaging Client...\n");
	

	struct servent *serv;
	struct sockaddr_in addr;
	
	// Obtain http service
	serv = getservbyname("http", "tcp");
	
	// Create socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd == -1) {
		printf("Failed to create socket\n");
		goto cleanup_file;
	}
	else {
		printf("Created socket with file descriptor: %d\n", socket_fd);
	}
	
	// Setup address to connect to
	addr.sin_family = AF_INET;
	addr.sin_port = htons(serv->s_port);
	endservent();	 // closes etc/services
	if (inet_pton(AF_INET, target_address, &addr.sin_addr) != 1) {
		printf("Failed to convert string %s to a network address\n", target_address);
		goto cleanup_socket;
	}
	
	// Connect to address
	printf("Attempting to connect to address %s...\n", target_address);
	if (connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) == 0) {
		printf("Connected to address %s\n", target_address);
	}
	else {
		printf("Failed to connect to address %s\n", target_address);
		goto cleanup_socket;
	}

	printf("\n");

	if (fp != NULL) {
		printf("Sending file to server\n");

		int file_bytes;
		if (fseek(fp, 0, SEEK_END) == -1 ||
			(file_bytes = ftell(fp)) == -1) {
			printf("Failed to seek in file\n");
			goto cleanup_socket;
		}

		printf("file bytes = %d\n", file_bytes);

		if (tf_send_file(socket_fd, fp, 0, file_bytes)) {
			printf("Failed to send file\n");
			goto cleanup_socket;
		}

		// Cleanup file
		fclose(fp);
	} else {
		getInput(socket_fd);
	}
	
	// Cleanup socket
	printf("Closing socket %d\n", socket_fd);
	close(socket_fd);
	
	return 0;

cleanup_socket:
	printf("Closing socket %d\n", socket_fd);
	close(socket_fd);
cleanup_file:
	if (fp != NULL) {
		fclose(fp);
	}
exit:
	return 1;
}