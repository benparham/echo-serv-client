#include <transfer.h>


typedef enum MSG_TYPE {
	HDR,
	DATA
} MSG_TYPE;

typedef struct tfr_message {
	MSG_TYPE type;
	void *payload;
} message;



static int connection_send(tfr_connection *con, void *payload, int payload_bytes) {
	(void) payload;
	(void) payload_bytes;

	return 1;



	// send(socketFD, slzr->serial, slzr->serialSizeBytes, 0);

}

static int connection_rec(tfr_connection *con, void **payload, int *payload_bytes) {
	(void) payload;
	(void) payload_bytes;

	return 1;
}

int tfr_connction_create(tfr_connection **con, int socket_fd) {
	*con = (tfr_connection *) malloc(sizeof(tfr_connection));
	if (*con == NULL {
		goto exit;
	}

	(*con)->msg = (tfr_message *) malloc(sizeof(tfr_message));
	if ((*con)->msg == NULL) {
		goto cleanup_con;
	}

	(*con)->socket_fd = socket_fd;
	
	(*con)->send = &connection_send;
	(*con)->rec = &connection_rec;

cleanup_con:
	free(*con);
exit:
	return 1;
}

void tfr_connection_destroy(tfr_connection *con) {
	if (con != NULL) {
		if (con->msg != NULL) {
			free(con->msg);
		}

		free(con);
	}
}