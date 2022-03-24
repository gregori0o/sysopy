#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>


void print_in_line (char* filepath, int line, char* text)
{
	int out = open (filepath, O_RDWR);
	flock (out, LOCK_EX);
	if (out < 0)
	{
		printf("Nie udało się otworzyć pliku %s, w lini %d nie został zapisany tekst %s\n", filepath, line, text);
		return;
	}
	char c;
	int k = 0;
	while (k < line-1 && read(out, &c, 1) == 1)
	{
		if (c == '\n')
			k++;
	}
	while (k < line-1)
	{
		write (out, "\n", 1);
		k++;
	}
	while (read(out, &c, 1) == 1)
	{
		if (c == '\n')
		{
			lseek (out, -1, SEEK_CUR);
			break;
		}
	}
	char* buffer = calloc (100000, sizeof(char));
	read (out, buffer, 100000);
	int size = strlen(buffer);
	lseek (out, (-1)*size, SEEK_END);
	write (out, text, strlen(text));
	write (out, buffer, size);
	flock (out, LOCK_UN);
	close (out);
	free (buffer);
}


void print_result (char* filepath)
{
	FILE* f;
	f = fopen (filepath, "r");
	if (f == NULL)
	{
		printf("błað\n");
		return;
	}
	char* buffer = calloc (200, sizeof(char));
	while (fgets(buffer, 200, f) != NULL)
	{
		printf("%s\n", buffer);
	}
	printf("********\n********\n");
	fclose (f);
	free (buffer);
}

int main (int argc, char** argv)
{
	if (argc < 4)
	{
		printf("Błąd argumentów wywołania\n");
		exit (1);
	}
	char* pipe = argv[1];
	char* filepath = argv[2];
	int n;
	sscanf(argv[3], "%d", &n);
	int src;
	src = open (pipe, O_RDONLY);
	if (src < 0)
	{
		printf("Nie udało się odczytac danych z pliku %s\n", pipe);
		exit (1);
	}

	char* text = calloc (n+1, sizeof(char));
	char* buffer = calloc (12, sizeof(char));
	while (1)
	{
		char c;
		int k = 0, line, tmp = 1;
		while (tmp != 0)
		{
			tmp = read(src, &c, 1);
			if (tmp == 0) break;
			if (c == 32)
			{
				buffer[k] = 0;
				break;
			}
			buffer[k] = c;
			k++;
		}
		if (tmp == 0)
			break;
		sscanf (buffer, "%d", &line);
		read (src, text, n);
		print_in_line (filepath, line, text);
		//print_result(filepath);
	}


	free (text);
	free (buffer);
	close (src);

	return 0;
}
