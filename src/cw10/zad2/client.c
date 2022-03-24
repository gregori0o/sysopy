#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/un.h>


#define X 2
#define O 1


int type;
int sock;
int epoll;
char mess[12];

void disconnect (int msg)
{
	sprintf(mess, "%d", msg);
	write (sock, &mess, 12);
	exit (0);
}


void handler (int signal)
{
	disconnect (10);
}


void end ()
{
	close (epoll);
	close (sock);
	printf("Koniec!\n");
}


int same (int* arr, int i, int j, int k)
{
	if (arr[i] == arr[j] && arr[j] == arr[k])
		return arr[i];
	return 0;
}


int win (int *arr)
{
	if (same(arr, 0, 1, 2) == X)
		return 1;
	if (same(arr, 3, 4, 5) == X)
		return 1;
	if (same(arr, 6, 7, 8) == X)
		return 1;
	if (same(arr, 0, 3, 6) == X)
		return 1;
	if (same(arr, 1, 4, 7) == X)
		return 1;
	if (same(arr, 2, 5, 8) == X)
		return 1;
	if (same(arr, 0, 4, 8) == X)
		return 1;
	if (same(arr, 2, 4, 6) == X)
		return 1;
	return 0;
}


int lose (int *arr)
{
	if (same(arr, 0, 1, 2) == O)
		return 1;
	if (same(arr, 3, 4, 5) == O)
		return 1;
	if (same(arr, 6, 7, 8) == O)
		return 1;
	if (same(arr, 0, 3, 6) == O)
		return 1;
	if (same(arr, 1, 4, 7) == O)
		return 1;
	if (same(arr, 2, 5, 8) == O)
		return 1;
	if (same(arr, 0, 4, 8) == O)
		return 1;
	if (same(arr, 2, 4, 6) == O)
		return 1;
	return 0;
}


int draw (int *arr)
{
	for (int i = 0; i < 9; i++)
		if (arr[i] == 0)
			return 0;
	return 1;
}


void do_move(int *arr, int move, int type)
{
	arr[move-1] = type;
	for (int i = 0; i < 9; i++)
	{
		if (arr[i] == X)
			printf(" X ");
		else if (arr[i] == O)
			printf(" O ");
		else
			printf(" _ ");
		if (i % 3 == 2)
			printf("\n\n");
		else
			printf("||");
	}
	printf("\n");
	if (win(arr))
	{
		printf("Wygrałeś, nastąpi rozłączenie\n");
		disconnect(11);
	}
	if (lose(arr))
	{
		printf("Przegrałeś, nastąpi rozłączenie\n");
		disconnect(11);
	}
	if (draw(arr))
	{
		printf("REMIS, nastąpi rozłączenie\n");
		disconnect(11);
	}
}


void print (int *arr)
{
	for (int i = 0; i < 9; i++)
	{
		if (arr[i] == X)
			printf(" X ");
		else if (arr[i] == O)
			printf(" O ");
		else
			printf(" %d ", i+1);
		if (i % 3 == 2)
			printf("\n\n");
		else
			printf("||");
	}
	printf("\n");
}


int main (int argc, char** argv)
{
	atexit(end);

	if (argc < 4)
	{
		printf("Błąd argumentów\n");
		exit (1);
	}

	char *name = argv[1];
	if (strcmp(argv[2], "net") == 0)
		type = AF_INET;
	else if (strcmp(argv[2], "unix") == 0)
		type = AF_UNIX;
	else
	{
		printf("Błąd argumentów\n");
		exit (1);
	}
	if (type == AF_INET && argc < 5)
	{
		printf("Błąd argumentów\n");
		exit (1);
	}
	char *address = argv[3];
	int port;
	if (type == AF_INET)
		port = atoi(argv[4]);

	signal(SIGINT, handler);

	sock = socket(type, SOCK_DGRAM, 0);

	if (type == AF_INET)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		inet_pton(AF_INET, address, &addr.sin_addr);
		connect(sock, (struct sockaddr*) &addr, sizeof(addr));
	}
	else if (type == AF_UNIX)
	{
		struct sockaddr_un addr;
		addr.sun_family = AF_UNIX;
		strcpy (addr.sun_path, address);
		bind (sock, (struct sockaddr*) &addr, sizeof(sa_family_t));
		connect(sock, (struct sockaddr*) &addr, sizeof(addr));
	}

	epoll = epoll_create1 (0);

	struct epoll_event event1;
	event1.events = EPOLLIN | EPOLLPRI;
	event1.data.fd = STDIN_FILENO;
	epoll_ctl(epoll, EPOLL_CTL_ADD, STDIN_FILENO, &event1);

	struct epoll_event event2;
	event2.events = EPOLLIN | EPOLLPRI | EPOLLHUP;
	event2.data.fd = sock;
	epoll_ctl(epoll, EPOLL_CTL_ADD, sock, &event2);

	write(sock, name, strlen(name));

	int arr[9];
	for (int i = 0; i < 9; i++)
		arr[i] = 0;

	int your_move = 0;
	struct epoll_event events[4];
	while (1)
	{
		int k = epoll_wait(epoll, events, 4, 1);
		for (int i = 0; i < k; i++)
		{
			int move;
			int fd = events[i].data.fd;
			if (fd == STDIN_FILENO)
			{
				scanf("%d", &move);
				if (your_move == 0)
				{
					printf("Teraz nie twoja kolej oszuście\n");
					continue;
				}
				if (arr[move-1] != 0)
				{
					printf("Pole zajęte\n");
					continue;
				}
				sprintf(mess, "%d", move);
				write (sock, &mess, 12);
				do_move(arr, move, X);
				your_move = 0;
			}
			else
			{
				recvfrom(sock, &move, sizeof (int), 0, NULL, NULL);
				//printf("%d\n", move);
				if (move == 0)
				{
					sprintf(mess, "%d", move);
					write (sock, &mess, 12);
				}
				else if (move == 10)
					printf("Czekaj na przeciwnika\n");
				else if (move == 11)
				{
					print (arr);
					printf("Twój ruch\n");
					your_move = 1;
				}
				else if (move == 12)
				{
					printf("Wygrałeś, nastąpi rozłączenie\n");
					disconnect(11);
				}
				else if (move == 13)
				{
					printf("Serwer pełny\n");
					exit (0);
				}
				else
				{
					do_move(arr, move, O);
				}
			}
		}
	}

	return 0;
}
