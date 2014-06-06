#include <stdio.h>

#include <transfer.h>

int tf_send(int socket_fd, const void *buf, size_t length) {

	printf("Will send here\n");

	return 0;
}

int tf_recv(int socket_fd, void **buf, size_t *length) {

	printf("Will recv here\n");

	return 0;
}