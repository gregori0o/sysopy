#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H

#define CHAT 1
#define INIT 2
#define CONNECT 3
#define LIST 4
#define DISCONNECT 5
#define STOP 6

#define text_size 1000
#define number_of_clients 100
#define proj 10

typedef struct Message {
	long mtype;
	int cid;
	char mtext[text_size];
} Message;

int msgsize = sizeof (struct Message) - sizeof (long);

#endif