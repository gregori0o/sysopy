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


union addr {
  struct sockaddr_un uni;
  struct sockaddr_in web;
};


char *socket_path;
int pings[SIZE];
int socket_local;
int socket_net;
int epoll;
int idx = 0;
union addr **clients;
int socks[SIZE];

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
			if (clients[i] == NULL)
				continue;
			if (pings[i] == 0)
			{
				int opp;
				if (i % 2 == 0)
					opp = i+1;
				else
					opp = i-1;
				int m = 12;
				sendto (socks[opp], &m, sizeof(int), 0, (struct sockaddr*) clients[opp], sizeof(clients[opp]));
				if (clients[opp] == NULL)
					idx ++;
				else
					free (clients[opp]);
				free (clients[i]);
				clients[i] = NULL;
				clients[opp] = NULL;
			}
			else
			{
				pings[i] = 0;
				sendto (socks[i], &msg, sizeof(int), 0, (struct sockaddr*) clients[i], sizeof(clients[i]));
			}
		}
		pthread_mutex_unlock(&mutex);
	}
}


void end ()
{
	pthread_mutex_destroy (&mutex);
	pthread_cond_destroy (&ping_cond);
	close (socket_local);
	close (socket_net);
	close (epoll);
	for (int i = 0; i < SIZE; i++)
		if (clients != NULL)
			free (clients[i]);
	free (clients);
	unlink(socket_path);
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

	clients = calloc (SIZE, sizeof(union addr*));

	for (int i = 0; i < SIZE; i++)
	{
		pings[i] = 0;
		clients[i] = NULL;

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

	socket_local = socket (AF_UNIX, SOCK_DGRAM, 0);
	bind (socket_local, (struct sockaddr*) &loc_addr, sizeof(loc_addr));
	
	socket_net = socket (AF_INET, SOCK_DGRAM, 0);
	bind (socket_net, (struct sockaddr*) &net_addr, sizeof(net_addr));


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

	char msg[20];


	while (1)
	{
		int k = epoll_wait(epoll, events, 16, -1);
		for (int i = 0; i < k; i++)
		{
			int so = events[i].data.fd;
			union addr addr;
			socklen_t len = sizeof (addr);
			recvfrom(so, msg, 19, 0, (struct sockaddr*) &addr, &len);
			//printf("%s\n", msg);
			pthread_mutex_lock(&mutex);
			int fd = -1;
			for (int j = 0; j < SIZE; j++)
			{
				if (clients[j] == NULL) continue;
				if (memcmp(clients[j], &addr, len) == 0)
				{
					fd = j;
					break;
				}
			}
			if (fd == -1)
			{
				//printf("New client\n");
				if (idx == SIZE)
				{
					printf("Serwer pełny\n");
					int msg = 13;
					sendto (so, &msg, sizeof(int), 0, (struct sockaddr*) &addr, len);
				}
				int in = idx;
				socks[in] = so;
				clients[in] = calloc (1, sizeof(union addr));
				memcpy(clients[in], &addr, len);
				pings[in] = 1;
				while (idx < SIZE && clients[idx] != NULL) idx++;
				if (idx == SIZE)
				{
					idx = 0;
					while (idx < SIZE && clients[idx] != NULL) idx++;
				}
				int move;
				if (in % 2 == 0)
					move = 10;
				else
					move = 11;
				sendto (so, &move, sizeof(int), 0, (struct sockaddr*) &addr, len);
			}
			else
			{
				//printf("Old client\n");
				int move = atoi (msg);
				int opp;
				if (fd % 2 == 0)
					opp = fd+1;
				else
					opp = fd-1;
				len = sizeof (clients[opp]);
				if (move == 0)
					pings[fd] = 1;
				else if (move == 10)
				{
					int msg = 12;
					sendto (socks[opp], &msg, sizeof(int), 0, (struct sockaddr*) clients[opp], len);
					if (clients[opp] == NULL)
						idx ++;
					else
						free (clients[opp]);
					free (clients[fd]);
					clients[fd] = NULL;
					clients[opp] = NULL;
				}
				else if (move == 11)
				{
					free (clients[fd]);
					clients[fd] = NULL;
				}
				else
				{
					sendto (socks[opp], &move, sizeof(int), 0, (struct sockaddr*) clients[opp], len);
					int msg = 11;
					sendto (socks[opp], &msg, sizeof(int), 0, (struct sockaddr*) clients[opp], len);
				}
					
			}
			pthread_mutex_unlock(&mutex);
		}
	}

	return 0;
}
