#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>

#include "client_server.h"


int list_clients [number_of_clients][3];
int ind = 0;

int loop = 1;
mqd_t queue_ID = -1;


void init_handler (int cid)
{
	char* name = calloc (20, sizeof (char));
	sprintf (name, "/%d", cid);
	list_clients[ind][0] = mq_open (name, O_WRONLY);
	list_clients[ind][2] = cid;
	free (name);


	char* init_msg = calloc (50, sizeof(char));
	sprintf (init_msg, "%d %d", INIT, ind);
	mq_send (list_clients[ind][0], init_msg, 50, INIT);
	free (init_msg);
	list_clients[ind][1] = 1;
	for (; ind < number_of_clients && list_clients[ind][0] != -1; ind++);
	if (ind == number_of_clients)
		printf("Osiągnięto maksymalną liczbę klientów\n");
}

void list_handler (int cid)
{
	char* msg = calloc (text_size, sizeof(char));
	sprintf (msg, "%d %d ", LIST, 0);

	for (int i = 0; i < number_of_clients; i++)
	{
		if (list_clients[i][0] == -1)
			continue;
		if (i == cid)
			continue;
		sprintf (msg + strlen (msg), "%d %d\n", i, list_clients[i][1]);
	}

	mq_send (list_clients[cid][0], msg, msgsize, LIST);
}

void stop_handler (int cid)
{
	mq_close(list_clients[cid][0]);
	list_clients[cid][0] = -1;
	if (cid < ind)
		ind = cid;
}

void connect_handler (int cid, char* text)
{
	char* msg1 = calloc (50, sizeof(char));
	char* msg2 = calloc (50, sizeof(char));
	int cl1, cl2;
	cl1 = cid;
	sscanf (text, "%d", &cl2);
	sprintf (msg1, "%d %d", CONNECT, list_clients[cl2][2]);
	sprintf (msg2, "%d %d", CONNECT, list_clients[cl1][2]);
	mq_send (list_clients[cl1][0], msg1, 50, CONNECT);
	mq_send (list_clients[cl2][0], msg2, 50, CONNECT);
	list_clients[cl1][1] = 0;
	list_clients[cl2][1] = 0;
	free (msg1);
	free (msg2);
}


void message_handler (char* msg)
{
	int type, cid;
	char* text = calloc (text_size, sizeof(char));
	sscanf (msg, "%d %d %s", &type, &cid, text);
	printf("Obsługa wiadomości typu %d od klienta %d o treści %s\n", type, cid, text);
	switch (type)
	{
		case INIT:
			init_handler (cid);
			break;
		case LIST:
			list_handler (cid);
			break;
		case CONNECT:
			connect_handler (cid, text);
			break;
		case DISCONNECT:
			list_clients[cid][1] = 1;
			break;
		case STOP:
			stop_handler (cid);
			break;
		default :
			printf("Nie rozpoznano typu wiadomości: %d\n", type);
			break;
	}
	free (text);
}

void int_handler (int signal)
{
	loop = 0;
}

void end_queue ()
{
	mq_close(queue_ID);
	if (mq_unlink (NAME) == -1)
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

	struct mq_attr attr;
	attr.mq_maxmsg = number_of_msg;
	attr.mq_msgsize = msgsize;

	queue_ID = mq_open (NAME, O_RDONLY | O_CREAT | O_EXCL |  O_NONBLOCK, 0666, &attr);
	if (queue_ID == -1)
	{
		perror ("Nie udało się utworzyć kolejki");
		exit (1);
	}


	char* msg = calloc (msgsize, sizeof(char));
	while (loop == 1)
	{
		if (mq_receive(queue_ID, msg, msgsize, NULL) > 0)
		{
			message_handler (msg);
			continue;
		}
		sleep (1);
	}

	char* stop_msg = calloc (50, sizeof(char));
	sprintf (stop_msg, "%d %d", STOP, 0);

	for (int i = 0; i < number_of_clients; i++)
	{
		if (list_clients[i][0] == -1)
			continue;
		mq_send (list_clients[i][0], stop_msg, 50, STOP);
		while (1)
		{
			if (mq_receive(queue_ID, msg, msgsize, NULL) > 0)
			{
				int type, cid;
				char* text = calloc (text_size, sizeof(char));
				sscanf (msg, "%d %d %s", &type, &cid, text);
				if (cid == i && type == STOP)
					break;
				if (type == STOP)
					stop_handler (cid);
			}
		}
		mq_close(list_clients[i][0]);
	}
	free (msg);
	free (stop_msg);

	return 0;
}