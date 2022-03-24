#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/select.h>
#include <mqueue.h>
#include <fcntl.h>

#include "client_server.h"


mqd_t server_queue = -1;
mqd_t client_queue = -1;
int client_ID = -1;
int loop = 1;
char name [20];
struct sigevent se;

void message_handler (char* msg);

void disconnect_handler (int cid)
{
	loop = 0;
	char* msg = calloc (50, sizeof(char));
	sprintf (msg, "%d %d", DISCONNECT, client_ID);
	mq_send (server_queue, msg, 50, DISCONNECT);
}

void execute_chat_command (char* option, char* text, int queue)
{
	char* msg = calloc (msgsize, sizeof(char));

	if (strcmp(option, "chat") == 0)
	{
		sprintf (msg, "%d %d %s", CHAT, client_ID, text);
		if (mq_send (queue, msg, msgsize, CHAT) == -1)
			disconnect_handler (0);
	}
	else if (strcmp(option, "disconnect") == 0)
	{
		sprintf (msg, "%d %d", DISCONNECT, client_ID);
		mq_send (queue, msg, msgsize, DISCONNECT);
		disconnect_handler (0);
	}
	else
	{
		printf("Nieprawidłowa opcja czatu\n");
	}
}

void connect_handler (int cid, char* text)
{
	char* tmp_name = calloc (50, sizeof(char));
	sprintf (tmp_name, "/%d", cid);

	int queue = mq_open (tmp_name, O_WRONLY);
	free (tmp_name);

	char* option = calloc (20, sizeof (char));
	char* text_c = calloc (text_size, sizeof (char));

	mq_notify(client_queue, &se);

	while (loop == 1)
	{
		//if (msgrcv(client_queue, &new_msg, msgsize, 0, IPC_NOWAIT) >= 0)
		//	message_handler (new_msg);

		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(STDIN_FILENO, &rfds);
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
		if (FD_ISSET(0, &rfds))
		{
			scanf ("%s", option);
			scanf ("%s", text_c);
			printf("%s\n", "Wczytano komendę.");

			execute_chat_command (option, text_c, queue);
		}
		
	}
	loop = 1;

	mq_close(queue);

	free (option);
	free (text_c);
}

void stop_handler (int cid)
{
	char* msg = calloc (50, sizeof(char));
	sprintf (msg, "%d %d", STOP, client_ID);
	mq_send (server_queue, msg, 50, STOP);

	exit (0);
}

void message_handler (char* msg)
{
	int type, cid;
	char* text = calloc (text_size, sizeof(char));
	sscanf (msg, "%d %d %s", &type, &cid, text);
	switch (type)
	{
		case CONNECT:
			connect_handler (cid, text);
			break;
		case STOP:
			stop_handler (cid);
			break;
		case CHAT:
			printf("%s\n", text);
			break;
		case DISCONNECT:
			disconnect_handler (cid);
			break;
		default :
			printf("Nie rozpoznano typu wiadomości: %d\n", type);
			break;
	}
	free (text);
}

void execute_command (char* option, char* text)
{
	char* msg = calloc (msgsize, sizeof(char));

	if (strcmp(option, "list") == 0)
	{
		sprintf (msg, "%d %d", LIST, client_ID);
		mq_send (server_queue, msg, msgsize, LIST);

		mq_receive(client_queue, msg, msgsize, NULL);
		int k = 0;
		int l = 0;
		while (l<2)
		{
			if (msg[k] == 32)
				l++;
			k++;
		}
		printf ("%s\n", msg+k);
	}
	else if (strcmp(option, "connect") == 0)
	{
		sprintf (msg, "%d %d %s", CONNECT, client_ID, text);
		mq_send (server_queue, msg, msgsize, CONNECT);

		mq_receive(client_queue, msg, msgsize, NULL);
		message_handler (msg);
	}
	else if (strcmp(option, "stop") == 0)
	{
		sprintf (msg, "%d %d", STOP, client_ID);
		mq_send (server_queue, msg, msgsize, STOP);
		exit (0);
	}
	else
	{
		printf("Nieprawidłowa opcja\n");
	}
	free (msg);
}



void int_handler (int signal)
{
	char* msg = calloc (50, sizeof(char));
	sprintf (msg, "%d %d", STOP, client_ID);
	mq_send (server_queue, msg, 50, STOP);

	exit (0);
}

void notify_handler (int signal)
{
	char* msg = calloc (msgsize, sizeof(char));
	mq_receive(client_queue, msg, msgsize, NULL);
	mq_notify(client_queue, &se);
	message_handler (msg);
	free (msg);
}


void end_queue ()
{
	mq_close(client_queue);
	mq_close(server_queue);
	if (mq_unlink (name) == -1)
		perror ("Nie udało się skasować kolejki");
}

int main (int argc, char** argv)
{
	struct sigaction sig, sig_notify;
	sig.sa_handler = int_handler;
	sigaction (SIGINT, &sig, NULL);
	sig_notify.sa_handler = notify_handler;
	sigaction (SIGUSR1, &sig_notify, NULL);

	atexit (end_queue);

	sprintf (name, "/%d", getpid());

	server_queue = mq_open (NAME, O_WRONLY);
	if (server_queue == -1)
	{
		perror ("Nie udało się odnależć kolejki serwera");
		exit (1);
	}

	struct mq_attr attr;
	attr.mq_maxmsg = number_of_msg;
	attr.mq_msgsize = msgsize;

	client_queue = mq_open (name, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);
	if (client_queue == -1)
	{
		perror ("Nie udało się utworzyć kolejki klienta");
		exit (1);
	}

	char* init_msg = calloc (50, sizeof(char));
	char* msg = calloc (msgsize, sizeof(char));

	sprintf (init_msg, "%d %d", INIT, getpid());

	mq_send (server_queue, init_msg, 50, INIT);
	free (init_msg);

	mq_receive(client_queue, msg, msgsize, NULL);
	sscanf (msg, "%*d %d", &client_ID);
	free (msg);

	
	se.sigev_notify = SIGEV_SIGNAL;
	se.sigev_signo = SIGUSR1;

	mq_notify(client_queue, &se);


	char* option = calloc (20, sizeof (char));
	char* text = calloc (text_size, sizeof (char));
	while (1)
	{
		//if (msgrcv(client_queue, &msg, msgsize, 0, IPC_NOWAIT) >= 0)
		//	message_handler (msg);

		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(STDIN_FILENO, &rfds);
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
		if (FD_ISSET(0, &rfds))
		{
			scanf ("%s", option);
			scanf ("%s", text);
			printf("%s\n", "Wczytano komendę.");
			execute_command (option, text);
		}
	}
	free (option);
	free (text);
	return 0;
}