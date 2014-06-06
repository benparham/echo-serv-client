#ifndef _TRANSFER_H_
#define _TRANSFER_H_

int tf_send(int socket_fd, const void *buf, size_t length);

int tf_recv(int socket_fd, void **buf, size_t *length);

#endif