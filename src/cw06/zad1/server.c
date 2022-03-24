#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>

#include "client_server.h"


int list_clients [number_of_clients][3];
int ind = 0;

int loop = 1;
int queue_ID = -1;


void init_handler (struct Message msg)
{
	list_clients[ind][0] = msgget (msg.cid, 0);
	list_clients[ind][2] = msg.cid;
	struct Message init_msg;
	init_msg.mtype = INIT;
	init_msg.cid = ind;
	msgsnd (list_clients[ind][0], &init_msg, msgsize, 0);
	list_clients[ind][1] = 1;
	for (; ind < number_of_clients && list_clients[ind][0] != -1; ind++);
	if (ind == number_of_clients)
		printf("Osiągnięto maksymalną liczbę klientów\n");
}

void list_handler (struct Message msg)
{
	struct Message list_msg;
	list_msg.mtype = LIST;
	list_msg.mtext[0] = 0;

	for (int i = 0; i < number_of_clients; i++)
	{
		if (list_clients[i][0] == -1)
			continue;
		if (i == msg.cid)
			continue;
		sprintf (list_msg.mtext + strlen (list_msg.mtext), "%d %d\n", i, list_clients[i][1]);
	}

	msgsnd (list_clients[msg.cid][0], &list_msg, msgsize, 0);
}

void stop_handler (struct Message msg)
{
	list_clients[msg.cid][0] = -1;
	if (msg.cid < ind)
		ind = msg.cid;
}

void connect_handler (struct Message msg)
{
	struct Message msg1, msg2;
	msg1.mtype = CONNECT;
	msg2.mtype = CONNECT;
	int cl1, cl2;
	cl1 = msg.cid;
	sscanf (msg.mtext, "%d", &cl2);
	msg1.cid = list_clients[cl2][2];
	msgsnd (list_clients[cl1][0], &msg1, msgsize, 0);
	msg2.cid = list_clients[cl1][2];
	msgsnd (list_clients[cl2][0], &msg2, msgsize, 0);
	list_clients[cl1][1] = 0;
	list_clients[cl2][1] = 0;
}


void message_handler (struct Message msg)
{
	printf("Obsługa wiadomości typu %ld od klienta %d o treści %s\n", msg.mtype, msg.cid, msg.mtext);
	switch (msg.mtype)
	{
		case INIT:
			init_handler (msg);
			break;
		case LIST:
			list_handler (msg);
			break;
		case CONNECT:
			connect_handler (msg);
			break;
		case DISCONNECT:
			list_clients[msg.cid][1] = 1;
			break;
		case STOP:
			stop_handler (msg);
			break;
		default :
			printf("Nie rozpoznano typu wiadomości: %ld\n", msg.mtype);
			break;
	}
}

void int_handler (int signal)
{
	loop = 0;
}

void end_queue ()
{
	if (msgctl (queue_ID, IPC_RMID, NULL) == -1)
		perror ("Nie udało się skasować kolejki");
}


int main (int argc, char** argv)
{
	struct sigaction sig;
	sig.sa_handler = int_handler;
	sigaction (SIGINT, &sig, NULL);

	atexit (end_queue);

	for (int i = 0; i < number_of_clients; i++)
		list_clients[i][0] = -1;

	char* filepath = getenv("HOME");
	key_t key = ftok (filepath, proj);
	queue_ID = msgget (key, IPC_CREAT | IPC_EXCL | 0666);
	if (queue_ID == -1)
	{
		perror ("Nie udało się stworzyć kolejki");
		exit (1);
	}

	struct Message msg;
	while (loop == 1)
	{
		if (msgrcv(queue_ID, &msg, msgsize, STOP, IPC_NOWAIT) != -1)
		{
			message_handler (msg);
			continue;
		}
		if (msgrcv(queue_ID, &msg, msgsize, DISCONNECT, IPC_NOWAIT) != -1)
		{
			message_handler (msg);
			continue;
		}
		if (msgrcv(queue_ID, &msg, msgsize, LIST, IPC_NOWAIT) != -1)
		{
			message_handler (msg);
			continue;
		}
		if (msgrcv(queue_ID, &msg, msgsize, 0, IPC_NOWAIT) != -1)
		{
			message_handler (msg);
			continue;
		}
		sleep (1);
	}

	struct Message stop_msg;
	stop_msg.mtype = STOP;

	for (int i = 0; i < number_of_clients; i++)
	{
		if (list_clients[i][0] == -1)
			continue;
		msgsnd (list_clients[i][0], &stop_msg, msgsize, 0);
		while (1)
		{
			msgrcv(queue_ID, &msg, msgsize, 0, 0);
			if (msg.cid == i && msg.mtype == STOP)
				break;
			if (msg.mtype == STOP)
				stop_handler (msg);
		}
	}

	return 0;
}