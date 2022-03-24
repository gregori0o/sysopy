#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int count = 0;

void handler (int signal)
{
	printf("Obsługa sygnału %d\n", signal);
	
}

void handler_nodefer (int signal)
{
	count ++;
	printf("Obsługa sygnału %d\n", signal);
	printf("Wysyłam sygnał do siebie i po uśpieniu na 5 sekund wypisuje komunikat\n");
	if (count >= 3)
	{
		printf("Koniec wywoływań w pętli\n");
	}
	else
	{
		raise (signal);
		sleep (5);
	}
	printf("Koniec obsługi sygnału\n");	
}

void info_handler (int signal, siginfo_t* info, void* handl)
{
	printf("Obsługa sygnału %d\n", signal);
	printf("Informacje, które uzyskujemy dzięki fladze SA_SIGINFO\n");
	printf("Sygnał pochodzi z procesu %d\n", (int)info->si_pid);
	printf("Status zakończenia procesu, który wysłał sygnał %d - ważne dla sygnału SGCHLD\n", (int)info->si_status);
}

int main (int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Błędne argumenty\n");
		exit (1);
	}
	struct sigaction sig1, sig2;


	sigemptyset(&sig1.sa_mask);
	sigaddset(&sig1.sa_mask, SIGUSR1);
	sigaddset(&sig1.sa_mask, SIGUSR2);
	sigaddset(&sig1.sa_mask, SIGCHLD);
	sig2.sa_mask = sig1.sa_mask;

	if (strcmp(argv[1], "siginfo") == 0)
	{
		printf("Flaga SA_SIGINFO\n");
		printf("Obsługa trzech sygnałów: 2x SIGUSR1 oraz SIGCHLD pochodzącego z kończącego się procesu potomnego poprzez wywołanie funkcji exit(18)\n");
		printf("Oczekujemy poszerzoną informacje na temat sygnału\n");
		sig1.sa_flags = SA_SIGINFO;
		sig2.sa_flags = SA_SIGINFO;
		sig1.sa_sigaction = info_handler;
		sig2.sa_sigaction = info_handler;
	}
	else if (strcmp(argv[1], "resethand") == 0)
	{
		printf("Flaga SA_RESETHAND\n");
		printf("Obsługa trzech sygnałów: 2x SIGUSR1 oraz SIGCHLD pochodzącego z kończącego się procesu potomnego poprzez wywołanie funkcji exit(18)\n");
		printf("Proces obsłuży tylko jeden sygnał SIGCHLD i jeden SIGUSR1, drugi SIGUSR1 zakończy proces\n");
		sig1.sa_flags = SA_RESETHAND;
		sig2.sa_flags = SA_RESETHAND;
		sig1.sa_handler = handler;
		sig2.sa_handler = handler;
	}
	else if (strcmp(argv[1], "nodefer") == 0)
	{
		printf("Flaga SA_NODEFER\n");
		printf("Usuwam z maski sygnały SIGUSR1 oraz SIGUSR2\n");
		printf("Dla obydwu tych sygnałów ustawiam handler, który wywołuje ten sam sygnał, usypia się na 5 sekund i wypisuje komunikat\n");
		printf("Aby nie wejść w nieskończoną pętlę zabezpieczę się zmiennymi globalnymi\n");
		printf("Oczekujemu dla sygnału SIGUSR1 bez flagi SA_NODEFER że obsługa sygnału wykona się do końca i dopiero wtedy kolejny sygnał\n");
		printf("Dla sygnału SIGUSR2 z ustawioną flagą wraz z pojawieniem się nowego sygnału od razu wywoła się następny handler\n");
		sigdelset(&sig1.sa_mask, SIGUSR1);
		sigdelset(&sig1.sa_mask, SIGUSR2);
		sigdelset(&sig2.sa_mask, SIGUSR1);
		sigdelset(&sig2.sa_mask, SIGUSR2);
		sig2.sa_flags = SA_NODEFER;
		sig1.sa_handler = handler_nodefer;
		sig2.sa_handler = handler_nodefer;

		sigaction(SIGUSR1, &sig1, NULL);
		sigaction(SIGUSR2, &sig2, NULL);

		raise (SIGUSR1);

		count = 0;

		raise (SIGUSR2);

		return 0;
	}
	else
	{
		printf("Obsługa bez flag, handler rozszerzony dla sygnału SIGCHLD i zwykły dla SIGUSR1\n");
		printf("Obsługa trzech sygnałów: 2x SIGUSR1 oraz SIGCHLD pochodzącego z kończącego się procesu potomnego poprzez wywołanie funkcji exit(18)\n");
		printf("Dane ze struktury siginfo_t będą przypadkowe, obsługa błędów podstawowa\n");
		sig1.sa_handler = handler;
		sig2.sa_sigaction = info_handler;
	}

	sigaction(SIGUSR1, &sig1, NULL);
	sigaction(SIGCHLD, &sig2, NULL);


	pid_t child = fork ();
	if (child == 0)
	{
		exit (18);
	}

	sleep (1);
	
	raise (SIGUSR1);

	raise (SIGUSR1);

	return 0;
}

