#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//użycie grepa -> <pattern> <text>
//użycie ls -a i posortowanie w kolejności malejącej -> <filepath>


int main (int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Błąd argumentów wywołania\n");
		exit (1);
	}
	if (argc == 2)
	{
		char* command = calloc (100, sizeof(char));
		strcpy(command, "ls -a ");
		strcat (command, argv[1]);
		FILE* ls;
		FILE* sort;
		ls = popen(command, "r");
		free (command);
		if (ls == NULL)
		{
			printf("Nie udało się wykonać polecenia ls -a\n");
			exit (1);
		}
		sort = popen("sort -r", "w");
		if (sort == NULL)
		{
			printf("Nie udało się wykonać polecenia sort -r\n");
			pclose(ls);
			exit (1);
		}
		char* buffer = calloc (100, sizeof(char));
		while (fgets(buffer, 100, ls) != NULL)
			fputs (buffer, sort);
		pclose (ls);
		pclose (sort);
	}
	else
	{
		char* command = calloc (100, sizeof(char));
		strcpy(command, "grep ");
		strcat (command, argv[1]);
		FILE* grep = popen(command, "w");
		free (command);
		if (grep == NULL)
		{
			printf("Nie udało się wykonać polecenia grep\n");
			exit (1);
		}
		fputs (argv[2], grep);
		pclose (grep);
	}

	
	return 0;
}
