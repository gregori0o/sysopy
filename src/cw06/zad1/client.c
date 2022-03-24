#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/select.h>

#include "client_server.h"


int server_queue = -1;
int client_queue = -1;
int client_ID = -1;
int loop = 1;

void message_handler (struct Message msg);

void disconnect_handler (struct Message msg)
{
	loop = 0;
	struct Message disconnect_message;
	disconnect_message.mtype = DISCONNECT;
	disconnect_message.cid = client_ID;
	msgsnd (server_queue, &disconnect_message, msgsize, 0);
}

void execute_chat_command (char* option, char* text, int queue)
{
	struct Message msg;
	msg.cid = client_ID;

	if (strcmp(option, "chat") == 0)
	{
		msg.mtype = CHAT;
		strcpy (msg.mtext, text);
		if (msgsnd (queue, &msg, msgsize, 0) == -1)
			disconnect_handler (msg);
	}
	else if (strcmp(option, "disconnect") == 0)
	{
		msg.mtype = DISCONNECT;
		msgsnd (queue, &msg, msgsize, 0);
		disconnect_handler (msg);
	}
	else
	{
		printf("Nieprawidłowa opcja czatu\n");
	}
}

void connect_handler (struct Message msg)
{
	int queue = msgget (msg.cid, 0);

	Message new_msg;
	char* option = calloc (20, sizeof (char));
	char* text = calloc (text_size, sizeof (char));
	while (loop == 1)
	{
		if (msgrcv(client_queue, &new_msg, msgsize, 0, IPC_NOWAIT) >= 0)
			message_handler (new_msg);

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

			execute_chat_command (option, text, queue);
		}
		
	}
	loop = 1;
}

void stop_handler (struct Message msg)
{
	struct Message stop_msg;
	stop_msg.cid = client_ID;
	stop_msg.mtype = STOP;
	msgsnd (server_queue, &stop_msg, msgsize, 0);

	exit (0);
}

void message_handler (struct Message msg)
{
	switch (msg.mtype)
	{
		case CONNECT:
			connect_handler (msg);
			break;
		case STOP:
			stop_handler (msg);
			break;
		case CHAT:
			printf("%s\n", msg.mtext);
			break;
		case DISCONNECT:
			disconnect_handler (msg);
			break;
		default :
			printf("Nie rozpoznano typu wiadomości: %ld\n", msg.mtype);
			break;
	}
}

void execute_command (char* option, char* text)
{
	struct Message msg;
	msg.cid = client_ID;

	if (strcmp(option, "list") == 0)
	{
		msg.mtype = LIST;
		msgsnd (server_queue, &msg, msgsize, 0);

		msgrcv(client_queue, &msg, msgsize, 0, 0);
		printf ("%s\n", msg.mtext);
	}
	else if (strcmp(option, "connect") == 0)
	{
		msg.mtype = CONNECT;
		strcpy (msg.mtext, text);
		msgsnd (server_queue, &msg, msgsize, 0);

		msgrcv(client_queue, &msg, msgsize, 0, 0);
		connect_handler (msg);
	}
	else if (strcmp(option, "stop") == 0)
	{
		msg.mtype = STOP;
		msgsnd (server_queue, &msg, msgsize, 0);
		exit (0);
	}
	else
	{
		printf("Nieprawidłowa opcja\n");
	}
}

void int_handler (int signal)
{
	struct Message stop_msg;
	stop_msg.cid = client_ID;
	stop_msg.mtype = STOP;
	msgsnd (server_queue, &stop_msg, msgsize, 0);

	exit (0);
}

void end_queue ()
{
	if (msgctl (client_queue, IPC_RMID, NULL) == -1)
		perror ("Nie udało się skasować kolejki");
}


int main (int argc, char** argv)
{
	struct sigaction sig;
	sig.sa_handler = int_handler;
	sigaction (SIGINT, &sig, NULL);

	atexit (end_queue);

	char* filepath = getenv("HOME");
	key_t key = ftok (filepath, proj);
	server_queue = msgget (key, 0);
	if (server_queue == -1)
	{
		perror ("Nie udało się odnależć kolejki serwera");
		exit (1);
	}

	key_t c_key = ftok (filepath, getpid());
	client_queue = msgget (c_key, IPC_CREAT | IPC_EXCL | 0666);
	if (client_queue == -1)
	{
		perror ("Nie udało się utworzyć kolejki klienta");
		exit (1);
	}

	struct Message init_msg, msg;
	init_msg.mtype = INIT;
	init_msg.cid = c_key;

	msgsnd (server_queue, &init_msg, msgsize, 0);
	msgrcv(client_queue, &msg, msgsize, 0, 0);
	client_ID = msg.cid;

	char* option = calloc (20, sizeof (char));
	char* text = calloc (text_size, sizeof (char));
	while (1)
	{
		if (msgrcv(client_queue, &msg, msgsize, 0, IPC_NOWAIT) >= 0)
			message_handler (msg);

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

	return 0;
}