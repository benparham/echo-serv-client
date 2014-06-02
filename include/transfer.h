#ifndef _TRANSFER_H_
#define _TRANSFER_H_

#define CON_SEND(con, payload, payload_bytes)	con->send(con, payload, payload_bytes)
#define CON_REC(con, payload, payload_bytes)	con->rec(con, payload, payload_bytes)

typedef struct tfr_message;

typedef struct tfr_connection {
	int socket_fd;
	tfr_message *msg;

	// Functions
	int (* send)	(tfr_connection *con, void *payload, int payload_bytes);
	int (* rec)		(tfr_connection *con, void **payload, int *payload_bytes);

} tfr_connection;

int tfr_connction_create(tfr_connection **con, int socket_fd);
void tfr_connection_destroy(tfr_connection *con);

#endif
