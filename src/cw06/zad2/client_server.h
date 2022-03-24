#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H

#define CHAT 1
#define INIT 2
#define CONNECT 3
#define LIST 4
#define DISCONNECT 5
#define STOP 6

#define text_size 300
#define number_of_msg 5
#define number_of_clients 100
#define proj 10
#define NAME "/server_queue"

int msgsize = text_size + 2 * sizeof(int) + 2;

#endif