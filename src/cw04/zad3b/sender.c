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

int SIG1, SIG2;
int I;
int type = 0;
int catcher = -1;

union sigval val = {.sival_ptr = NULL};

void handler (int signal)
{
	if (I > 0)
	{
		I--;
		if (type == 1)
			sigqueue(catcher, SIG1, (union sigval){.sival_int = I});
		else
			kill (catcher, SIG1);
	}
	else
	{
		if (type == 1)
			sigqueue(catcher, SIG2, (union sigval){.sival_int = I});
		else
			kill (catcher, SIG2);
	}
}

void hand_sigaction (int signal, siginfo_t* info, void* handl)
{
	sigq = info->si_value.sival_int;
	wait = 0;
}

void counts (int signal, siginfo_t* info, void* handl)
{
	count ++;
	if (si < info->si_value.sival_int)
		si = info->si_value.sival_int;
}


int main (int argc, char** argv)
{
	sigset_t mask_tmp;
	sigfillset(&mask_tmp);
	sigprocmask (SIG_BLOCK, &mask_tmp, NULL);

	if (argc < 4)
	{
		printf("Błąd argumentu wywołań\n");
		return 0;
	}
	SIG1 = SIGUSR1;
	SIG2 = SIGUSR2;
	if (strcmp(argv[3],"SIGRT")==0)
	{
		SIG1 = SIGRTMIN;
		SIG2 = SIGRTMIN+1;
	}
	int number;
	sscanf (argv[1], "%d", &catcher);
	sscanf (argv[2], "%d", &number);
	I = number-1;


	struct sigaction sig1, sig2;
	sig1.sa_handler = handler;
	sigaction (SIGUSR1, &sig1, NULL);
	sig2.sa_sigaction = hand_sigaction;
	sigaction (SIGUSR2, &sig2, NULL);
	
	sigset_t mask, mask_c;
	sigfillset(&mask);
	sigdelset(&mask, SIGUSR1);
	sigdelset(&mask, SIGUSR2);

	sigfillset(&mask_c);
	sigdelset(&mask_c, SIG1);
	sigdelset(&mask_c, SIG2);
	sigdelset(&mask_c, SIGUSR1);
	sigdelset(&mask_c, SIGUSR2);

	if (strcmp(argv[3],"KILL")==0 || strcmp(argv[3],"SIGRT")==0)
	{
		kill(catcher, SIG1);
	}
	else if (strcmp(argv[3],"SIGQUEUE")==0)
	{
		type = 1;
		sigqueue(catcher, SIG1, (union sigval){.sival_int = number});
	}
	else
		printf("Tryb nieprawidłowy\n");
	count = 0;

	while (wait == 1)
	{
		//usleep(1);
		sigsuspend(&mask);
	}


	sig1.sa_sigaction = counts;
	sig1.sa_flags = SA_SIGINFO;
	sigaction(SIG1, &sig1, NULL);
	sig2.sa_sigaction = hand_sigaction;
	sig2.sa_flags = SA_SIGINFO;
	sigaction(SIG2, &sig2, NULL);

	wait = 1;

	while (wait == 1)
	{
		//usleep(1);
		sigsuspend(&mask_c);
	}

	if (strcmp(argv[3],"SIGQUEUE")==0)
		printf("Sender prawidłowo wysłał %d sygnałów i otrzymał informację o %d\n", sigq, si);

	printf ("Sender otrzymał %d sygnałów a powinien otrzymać ich %d\n", count, number);

	return 0;
}