#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <errno.h>

#define SIZE 10
#define PING 10


int clients[SIZE];
int pings[SIZE];
int socket_local;
int socket_net;
int epoll;
int idx = 0;
char *socket_path;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ping_cond = PTHREAD_COND_INITIALIZER;


void *ping_clients (void *_)
{
	int msg = 0;
	while (1)
	{
		sleep (PING);
		pthread_mutex_lock(&mutex);
		for (int i = 0; i < SIZE; i++)
		{
			if (clients[i] == -1)
				continue;
			if (pings[i] == 0)
			{
				int opp;
				if (i % 2 == 0)
					opp = i+1;
				else
					opp = i-1;
				int m = 12;
				write (clients[opp], &m, sizeof(int));
				close (clients[i]);
				if (clients[opp] == -1)
					idx ++;
				close (clients[opp]);
				clients[i] = -1;
				clients[opp] = -1;
			}
			else
			{
				pings[i] = 0;
				write (clients[i], &msg, sizeof(int));
			}
		}
		pthread_mutex_unlock(&mutex);
	}
}


void end ()
{
	pthread_mutex_destroy (&mutex);
	pthread_cond_destroy (&ping_cond);
	shutdown (socket_local, SHUT_RDWR);
	shutdown (socket_net, SHUT_RDWR);
	close (socket_local);
	close (socket_net);
	close (epoll);
	unlink (socket_path);
	printf("Koniec!\n");
}


int main (int argc, char** argv)
{
	atexit(end);

	if (argc < 3)
	{
		printf("Błąd argumentów\n");
		exit (1);
	}

	for (int i = 0; i < SIZE; i++)
	{
		clients[i] = -1;
		pings[i] = 0;
	}

	int port = atoi (argv[1]);
	socket_path = argv[2];

	epoll = epoll_create1 (0);

	struct sockaddr_in net_addr;
	net_addr.sin_family = AF_INET;
	net_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	net_addr.sin_port = htons(port);

	struct sockaddr_un loc_addr;
	loc_addr.sun_family = AF_UNIX;
	strcpy (loc_addr.sun_path, socket_path);

	socket_local = socket (AF_UNIX, SOCK_STREAM, 0);
	bind (socket_local, (struct sockaddr*) &loc_addr, sizeof(loc_addr));
	listen (socket_local, SIZE);
	
	socket_net = socket (AF_INET, SOCK_STREAM, 0);
	bind (socket_net, (struct sockaddr*) &net_addr, sizeof(net_addr));
	listen (socket_net, SIZE);


	struct epoll_event event1, event2;
	event1.events = EPOLLIN | EPOLLPRI;
	event2.events = EPOLLIN | EPOLLPRI;
	event1.data.fd = socket_local;
	event2.data.fd = socket_net;
	
	epoll_ctl (epoll, EPOLL_CTL_ADD, socket_local, &event1);

	epoll_ctl (epoll, EPOLL_CTL_ADD, socket_net, &event2);

	pthread_t ping_thread;
	pthread_create(&ping_thread, NULL, ping_clients, NULL);

	printf("Start listening\n");

	struct epoll_event events[16];
	while (1)
	{
		int k = epoll_wait(epoll, events, 16, -1);
		for (int i = 0; i < k; i++)
		{
			int fd = events[i].data.fd;
			pthread_mutex_lock(&mutex);
			if (fd == socket_local || fd == socket_net)
			{
				int client_fd = accept (fd, NULL, NULL);
				char name[10];
				read (client_fd, &name, 9);
				if (idx == SIZE)
				{
					printf("Serwer pełny\n");
					int msg = 13;
					write (client_fd, &msg, sizeof(int));
					close (client_fd);
				}
				int in = idx;
				clients[in] = client_fd;
				pings[in] = 1;
				while (idx < SIZE && clients[idx] != -1) idx++;
				if (idx == SIZE)
				{
					idx = 0;
					while (idx < SIZE && clients[idx] != -1) idx++;
				}
				struct epoll_event event;
				event.events = EPOLLIN | EPOLLET | EPOLLHUP;
				event.data.fd = (-1)*in;
				epoll_ctl (epoll, EPOLL_CTL_ADD, client_fd, &event);
				int move;
				if (in % 2 == 0)
					move = 10;
				else
					move = 11;
				write (client_fd, &move, sizeof(int));
			}
			else
			{
				fd *= (-1);
				int move;
				read (clients[fd], &move, sizeof(int));
				int opp;
				if (fd % 2 == 0)
					opp = fd+1;
				else
					opp = fd-1;
				if (move == 0)
					pings[fd] = 1;
				else if (move == 10)
				{
					int msg = 12;
					write (clients[opp], &msg, sizeof(int));
					if (clients[opp] == -1)
						idx ++;
					close (clients[fd]);
					close (clients[opp]);
					clients[fd] = -1;
					clients[opp] = -1;
				}
				else if (move == 11)
				{
					close (clients[fd]);
					clients[fd] = -1;
				}
				else
				{
					write (clients[opp], &move, sizeof(int));
					int msg = 11;
					write (clients[opp], &msg, sizeof(int));
				}
					
			}
			pthread_mutex_unlock(&mutex);
		}
	}

	return 0;
}