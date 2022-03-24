#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main (int argc, char** argv)
{
	if (argc < 5)
	{
		printf("Błąd argumentów wywołania\n");
		exit (1);
	}
	char* pipe = argv[1];
	int k, n;
	sscanf(argv[2], "%d", &k);
	char* filepath = argv[3];
	sscanf(argv[4], "%d", &n);
	int src, out;
	src = open (filepath, O_RDONLY);
	if (src < 0)
	{
		printf("Nie udało się odczytac danych z pliku %s\n", filepath);
		exit (1);
	}
	out = open (pipe, O_WRONLY);
	if (out < 0)
	{
		printf("Nie udało się odczytac danych z pliku %s\n", pipe);
		close (src);
		exit (1);
	}

	char* buffer = calloc (n+13, sizeof(char));
	strcpy (buffer, argv[2]);
	int shift = strlen(buffer);
	buffer[shift] = 32;
	shift++;

	while (read (src, buffer+shift, n) == n)
	{
		sleep (1);
		write (out, buffer, strlen(buffer));
	}
	//sleep(1);
	//write (out, buffer, strlen(buffer));

	free (buffer);
	close (out);
	close (src);

	return 0;
}
