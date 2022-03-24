#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int wait = 1;
int count = 0;
int sigq = -1;
int si = -1;

union sigval val = {.sival_ptr = NULL};

void hand_sigaction (int signal, siginfo_t* info, void* handl)
{
	wait = 0;
	sigq = info->si_value.sival_int;
}

void counts (int signal, siginfo_t* info, void* handl)
{
	count ++;
	if (si < info->si_value.sival_int)
		si = info->si_value.sival_int;
}


int main (int argc, char** argv)
{
	if (argc < 4)
	{
		printf("Błąd argumentu wywołań\n");
		return 0;
	}
	int SIG1, SIG2;
	SIG1 = SIGUSR1;
	SIG2 = SIGUSR2;
	if (strcmp(argv[3],"SIGRT")==0)
	{
		SIG1 = SIGRTMIN;
		SIG2 = SIGRTMIN+1;
	}
	int catcher, number;
	sscanf (argv[1], "%d", &catcher);
	sscanf (argv[2], "%d", &number);

	struct sigaction sig1, sig2;
	sig1.sa_sigaction = counts;
	sig1.sa_flags = SA_SIGINFO;
	sigaction(SIG1, &sig1, NULL);
	sig2.sa_sigaction = hand_sigaction;
	sig2.sa_flags = SA_SIGINFO;
	sigaction(SIG2, &sig2, NULL);

	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, SIG1);
	sigdelset(&mask, SIG2);

	if (strcmp(argv[3],"KILL")==0 || strcmp(argv[3],"SIGRT")==0)
	{
		for (int i=0; i<number; i++)
		{
			kill(catcher, SIG1);
		}
		kill(catcher, SIG2);
	}
	else if (strcmp(argv[3],"SIGQUEUE")==0)
	{
		for (int i=0; i<number; i++)
		{
			sigqueue(catcher, SIG1, (union sigval){.sival_int = i});
		}
		sigqueue(catcher, SIG2, (union sigval){.sival_int = number});
	}
	else
		printf("Tryb nieprawidłowy\n");
	count = 0;
	while (wait == 1)
		sigsuspend(&mask);

	if (strcmp(argv[3],"SIGQUEUE")==0)
		printf("Sender prawidłowo wysłał %d sygnałów i otrzymał informację o %d\n", sigq, si);

	printf ("Sender otrzymał %d sygnałów a powinien otrzymać ich %d\n", count, number);

	return 0;
}