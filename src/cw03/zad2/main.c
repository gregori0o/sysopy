#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "merge_text.h"
#include <sys/times.h>

static struct tms start, end;
static clock_t c_start, c_end;


void save_time (char *function)
{
	printf ("%s\n", function);
	printf ("Czas rzeczywisty: %ld\n", c_end - c_start);
	printf ("Czas użytkownika: %ld\n", end.tms_utime - start.tms_utime);
	printf ("Czas systemowy:   %ld\n", end.tms_stime - start.tms_stime);
}

int main (int argc, char **argv)
{
	table* main_table = NULL;
	char **list_sequence;
	int number;
	int size_ls;
	int i = 1;
	while (i < argc)
	{
		char *option = argv[i++];
		if (strcmp(option, "create_table") == 0)
		{
			sscanf(argv[i++], "%d", &number);
			size_ls = 2*number;
			main_table = create_table (number);
		}
		else if (strcmp(option, "merge_files_one") == 0)
		{
			list_sequence = calloc (size_ls, sizeof(char*));
			char *input = argv[i++];
			int tmp = set_seq (input, list_sequence);
			if (tmp < number) number = tmp;
			
			c_start = times(&start);
			for (int i=0; i<2*number; i+=2)
			{
				char* path = merge_files (list_sequence[i], list_sequence[i+1], i);
				if (path == NULL) continue;
				make_structure (path, main_table, i/2);
			}
			c_end = times(&end);
			
			save_time ("Zmergowanie plików w jednym procesie");
		}
		else if (strcmp(option, "merge_files_many") == 0)
		{
			list_sequence = calloc (size_ls, sizeof(char*));
			char *input = argv[i++];
			int tmp = set_seq (input, list_sequence);
			if (tmp < number) number = tmp;
			
			c_start = times(&start);
			for (int i=0; i<2*number; i+=2)
			{
				pid_t child = fork ();
				if (child == 0)
				{
					char* path = merge_files (list_sequence[i], list_sequence[i+1], i);
					if (path == NULL) continue;
					make_structure (path, main_table, i/2);
					exit (0);
				}
				
			}
			for (int i=0; i<number; i++)
				wait();
			c_end = times(&end);

			save_time ("Zmergowanie plików w wielu procesach");
		}
		else if (strcmp(option, "print") == 0)
		{
			print_result (main_table);
		}
		else if (strcmp(option, "remove_all") == 0)
		{
			remove_all(main_table);
		}
		else
			printf("%s %s\n", "Nieprawidłowe polecenie! -> ", option);
	}

	//czyszczenie pamięci
	for (int i=0; i<size_ls; i++)
	{
		free (list_sequence[i]);
	}
	free (list_sequence);
	
	return 0;
}
