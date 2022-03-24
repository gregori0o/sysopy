#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int sender = -1;
int wait = 1;
int count = 0;


void hand_sigaction (int signal, siginfo_t* info, void* handl)
{
	if (sender < 0)
		sender = (int)info->si_pid;
	count++;
}

void handler (int signal)
{
	wait = 0;
}


int main (int argc, char** argv)
{
	int SIG1, SIG2;
	SIG1 = SIGUSR1;
	SIG2 = SIGUSR2;
	if (argc > 1 && strcmp(argv[1],"SIGRT")==0)
	{
		SIG1 = SIGRTMIN;
		SIG2 = SIGRTMIN+1;
	}
	printf("%d\n", (int)getpid());
	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, SIG1);
	sigdelset(&mask, SIG2);
	struct sigaction sig1, sig2;
	sig1.sa_sigaction = hand_sigaction;
	sig1.sa_flags = SA_SIGINFO;
	sigaction(SIG1, &sig1, NULL);
	sig2.sa_handler = handler;
	sigaction(SIG2, &sig2, NULL);

	while (wait == 1)
		sigsuspend(&mask);
	if (argc > 1 && strcmp(argv[1],"SIGQUEUE")==0)
	{
		for (int i=0; i<count; i++)
			sigqueue(sender, SIG1, (union sigval){.sival_int = i+1});
		sigqueue(sender, SIG2, (union sigval){.sival_int = count});
	}
	else
	{
		for (int i=0; i<count; i++)
			kill(sender, SIG1);
		kill(sender, SIG2);	
	}
	
	printf("Catcher odebrał %d sygnałów\n", count);	

	return 0;
}