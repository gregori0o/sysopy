#include "merge_text.h"
#include <stdio.h>
#include <sys/times.h>
#include <dlfcn.h>

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
	void *handle = dlopen("libmerge_text.so", RTLD_LAZY);
	if (!handle)
	{
		printf("%s\n", "Błąd odczytu biblioteki");
		exit(1);
	}
	table* (*create_table) (int);
	create_table = (table* (*) ())dlsym(handle,"create_table");
	int (*set_seq) (char*, char**);
	set_seq = (int (*) ())dlsym(handle,"set_seq");
	char** (*merge_seq) (char**, int);
	merge_seq = (char** (*) ())dlsym(handle,"merge_seq");
	int (*make_structure) (char*, table*);
	make_structure = (int (*) ())dlsym(handle,"make_structure");
	int (*length) (int, table*);
	length = (int (*) ())dlsym(handle,"length");
	void (*remove_block) (int, table*);
	remove_block = (void (*) ())dlsym(handle,"remove_block");
	void (*remove_row) (int, int, table*);
	remove_row = (void (*) ())dlsym(handle,"remove_row");
	void (*print_result) (table*);
	print_result = (void (*) ())dlsym(handle,"print_result");
	void (*remove_all) (table*);
	remove_all = (void (*) ())dlsym(handle,"remove_all");
	if (dlerror() != NULL)
	{
		printf("%s\n", "Błąd odczytu biblioteki");
		exit(1);
	}


	table* main_table = NULL;
	char **list_sequence = NULL;
	char **paths = NULL;
	int number = 0;
	int size_ls = 0;
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
		else if (strcmp(option, "merge_files") == 0)
		{
			list_sequence = calloc (size_ls, sizeof(char*));
			char *input = argv[i++];
			c_start = times(&start);
			int tmp = set_seq (input, list_sequence);
			if (tmp < number) number = tmp;
			paths = merge_seq (list_sequence, number);
			c_end = times(&end);
			save_time ("Zmergowanie plików do plików tymczasowych");
		}
		else if (strcmp(option, "save_merge") == 0)
		{
			c_start = times(&start);
			for (int j=0; j<number; j++)
				make_structure (paths[j], main_table);
			c_end = times(&end);
			save_time ("Zapis wyniku zmergowania do bloków pamięci");
		}
		else if (strcmp(option, "print") == 0)
		{
			print_result (main_table);
		}
		else if (strcmp(option, "remove_block") == 0)
		{
			c_start = times(&start);
			int index;
			sscanf(argv[i++], "%d", &index);
			remove_block(index, main_table);
			c_end = times(&end);
			save_time ("Usunięcie bloku");
		}
		else if (strcmp(option, "remove_and_add_blocks") == 0)
		{
			int *index = calloc (number, sizeof(int));
			int few;
			sscanf(argv[i++], "%d", &few);
			c_start = times(&start);
			while (few>0)
			{
				for (int j=0; j<number; j++)
					index[j] = make_structure (paths[j], main_table);
				for (int j=0; j<number; j++)
					remove_block(index[j], main_table);
				few--;
			}
			c_end = times(&end);
			save_time ("Kilkukrotne zapisanie i usunięcie bloków pamięci");
			free (index);
		}
		else if (strcmp(option, "remove_row") == 0)
		{
			int index;
			int row;
			sscanf(argv[i++], "%d", &index);
			sscanf(argv[i++], "%d", &row);
			remove_row (index, row, main_table);
		}
		else if (strcmp(option, "block_length") == 0)
		{
			int index;
			sscanf(argv[i++], "%d", &index);
			printf("%d\n", length(index, main_table));
		}
		else if (strcmp(option, "remove_all") == 0)
		{
			c_start = times(&start);
			remove_all(main_table);
			main_table = NULL;
			c_end = times(&end);
			save_time ("Czyszczenie tablicy z wszystkich bloków");
		}
		else
			printf("%s %s\n", "Nieprawidłowe polecenie! -> ", option);
	}
	dlclose (handle);


	//czyszczenie pamięci
	if (main_table != NULL)
		remove_all(main_table);
	if (paths != NULL)
	{
		for (int i=0; i<number; i++)
		{
			remove (paths[i]);
			free (paths[i]);
		}
		free (paths);
	}
	if (list_sequence != NULL)
	{
		for (int i=0; i<size_ls; i++)
			free (list_sequence[i]);
		free (list_sequence);
	}

	
	return 0;
}