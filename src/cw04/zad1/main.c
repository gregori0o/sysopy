#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


void handler (int sign)
{
	printf("Obsługa sygnału %d\n", sign);
}


int main (int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Błąd argumentu wywołań\n");
		return 0;
	}

	char* filepath = "/home/glegeza/Programming/sysopy/lab4/zad1/main";

	//argv[1] -> ignore, handler, mask, pending

	int option = 0;
	if (strcmp(argv[1], "ignore") == 0)
	{
		option = 1;
		printf("Test ignorowania sygnału\n");
		signal (SIGUSR1, SIG_IGN);
	}
	else if (strcmp(argv[1], "handler") == 0)
	{
		option = 2;
		printf("Test obsługi sygnału przez handler\n");
		signal (SIGUSR1, handler);
	}
	else if (strcmp(argv[1], "mask") == 0)
	{
		option = 3;
		printf("Test maskowania sygnału\n");
		sigset_t mask_sig;
		sigemptyset(&mask_sig);
		sigaddset(&mask_sig, SIGUSR1);
		sigprocmask(SIG_BLOCK, &mask_sig, NULL);

	}
	else if (strcmp(argv[1], "pending") == 0)
	{
		option = 4;
		printf("Sprawdzanie czy sygnał wisi podczas maskowania\n");
		sigset_t mask_sig;
		sigemptyset(&mask_sig);
		sigaddset(&mask_sig, SIGUSR1);
		sigprocmask(SIG_BLOCK, &mask_sig, NULL);
	}
	else if (strcmp(argv[1], "exec") == 0)
	{
		sscanf (argv[2], "%d", &option);
		if (option == 4)
		{
			sigset_t pend_sig;
			sigpending(&pend_sig);
			if (sigismember(&pend_sig, SIGUSR1))
				printf("Sygnał wiszący jest widoczny w procesie wywołanym funkcją exec\n");
			else
				printf("Sygnał nie jest widoczny w procesie wywołanym funkcją exec\n");
			return 0;
		}
		printf("%s\n", "Wysyłam sygnał SIGUSR1 w procesie z exec i czekam 10 sekund na reakcję");
		raise(SIGUSR1);
		sleep (10);
		if (option == 1)
		{
			printf("Sygnał został zignorowany\n");
		}
		else if (option == 3)
		{
			printf("Sygnał blokowany przez maskę\n");
		}
		return 0;
	}
	else
	{
		printf("Opcja '%s' nie istnieje\n", argv[1]);
		return 0;
	}
	printf("Wysyłam sygnał SIGUSR1 w procesie macierzystym i czekam 10 sekund na reakcję\n");
	raise(SIGUSR1);
	sleep (10);
	if (option == 1)
	{
		printf("Sygnał został zignorowany\n");
	}
	else if (option == 2)
	{
		printf("Nastąpiło wywołanie handlera\n");
	}
	else if (option == 3)
	{
		printf("Sygnał blokowany przez maskę\n");
	}
	else if (option == 4)
	{
		sigset_t pend_sig;
		sigpending(&pend_sig);
		if (sigismember(&pend_sig, SIGUSR1))
			printf("Sygnał wiszący jest widoczny w procesie macierzystym\n");
		else
			printf("Sygnał nie jest widoczny w procesie macierzystym\n");
	}

	pid_t child = fork ();
	if (child == 0)
	{
		if (option == 4)
		{
			sigset_t pend_sig;
			sigpending(&pend_sig);
			if (sigismember(&pend_sig, SIGUSR1))
				printf("Sygnał wiszący jest widoczny w procesie potomnym\n");
			else
				printf("Sygnał nie jest widoczny w procesie potomnym\n");
			return 0;
		}
		printf("%s\n", "Wysyłam sygnał SIGUSR1 w procesie potomnym i czekam 10 sekund na reakcję");
		raise(SIGUSR1);
		sleep (10);
		if (option == 1)
		{
			printf("Sygnał został zignorowany\n");
		}
		else if (option == 2)
		{
			printf("Nastąpiło wywołanie handlera\n");
		}
		else if (option == 3)
		{
			printf("Sygnał blokowany przez maskę\n");
		}
		return 0;
	}
	sleep (12);
	if (option != 2)
	{
		char* buff = calloc (12, sizeof(char));
		sprintf(buff, "%d", option);
		execl (filepath, "main", "exec", buff, NULL);
	}
	sleep (12);
	return 0;
}