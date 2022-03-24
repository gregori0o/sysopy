#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int interpret_line (char* filepath, int n, int fd)
{
	FILE* f;
	f = fopen (filepath, "r");
	if (f == NULL)
	{
		printf("Nie odnaleziono pliku dla lini %d\n", n);
		return fd;
	}
	char* buffer = calloc (350, sizeof(char));
	for (int i = 0; i < n; i++)
		if (fgets(buffer, 350, f) == NULL)
		{
			printf("Nie odnaleziono lini numer %d\n", n);
			fclose (f);
			free (buffer);
			return fd;
		}
	char** commands = calloc (7, sizeof(char));
	char* command = strtok(buffer, "|=");
	command = strtok(NULL, "|=");
	int k = 0;
	while (command != NULL)
	{
		commands[k] = command;
		command = strtok(NULL, "|=");
		k++;
	}
	if (k < 7)
		commands[k] = NULL;

	char** av = calloc (10, sizeof(char*));
	for (int i = 0; i < 7 && commands[i] != NULL; i++)
	{
		char* arguments = strtok(commands[i], " \n");
		int k = 0;
		while (arguments != NULL)
		{
			av[k] = arguments;
			arguments = strtok(NULL, " \n");
			k++;
		}
		av[k] = NULL;

		int pip[2];
		pipe(pip);
		pid_t pid = fork ();
		if (pid == 0)
		{
			close (pip[0]);
			dup2 (pip[1], STDOUT_FILENO);
			if (fd != -1)
			{
				dup2 (fd, STDIN_FILENO);
			}
			execvp(av[0], av);
		}

		close (pip[1]);
		fd = pip[0];

		waitpid(pid);
	}

	free (buffer);
	free (commands);
	free (command);
	free (av);

	return fd;
}


int main (int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Błąd argumentów wywołania\n");
		exit (1);
	}

	char* filepath = argv[1];

	int fd = -1;

	for (int i = 2; i < argc; i+=2)
	{
		int n;
		sscanf(argv[i]+8, "%d", &n);
		fd = interpret_line (filepath, n, fd);
	}

	char* result = calloc (1000, sizeof(char));
	read (fd, result, 1000);
	printf("%s\n", result);
	free (result);

	return 0;
}
