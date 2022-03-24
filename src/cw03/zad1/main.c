#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	int N = 0;
	if (argc <= 1)
	{
		printf("%s\n", "Nieprawidłowe argumenty");
		exit (1);
	}
	sscanf (argv[1], "%d", &N);
	for (int i = 0; i < N; i++)
	{
		pid_t child = fork ();
		if (child == 0)
		{
			printf("Wywołanie procesu potomnego o numerze %d, PID wynosi: %d\n", i+1, (int)getpid());
			break;
		}
	}
	
	return 0;
}
