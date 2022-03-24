#include "merge_text.h"


table* create_table (int size)
{
	table* new_table = calloc (1, sizeof(table));
	if (new_table == NULL) exit (1);
	new_table -> size = size;
	new_table -> free_index = 0;
	new_table -> blocks = calloc (size, sizeof(block*));
	if (new_table -> blocks == NULL) exit (1);
	return new_table;
}


int set_seq (char* input, char** list_sequence)
{
	int i = 0;
	int size = strlen(input);
	int number = 0;
	while (i < size)
	{
		int f1 = 0, f2 = 0;
		while (input[i+f1] != 58)
		{
			f1++;
			if (i+f1 >= size)
				break;
		}
		while (input[i+1+f1+f2] != 32)
		{
			f2++;
			if (i+1+f1+f2 >= size)
				break;
		}
		if (f1>0 && f2>0)
		{
			list_sequence[number] = calloc(f1+1, sizeof(char));
			list_sequence[number+1] = calloc(f2+1, sizeof(char));
			for (int j=0; j<f1; j++)
				list_sequence[number][j] = input[i++];
			list_sequence[number][f1]=0;
			i++;
			for (int j=0; j<f2; j++)
				list_sequence[number+1][j] = input[i++];
			list_sequence[number+1][f2]=0;
			i++;
			number+=2;
		}
	}
	return number/2;
}

char* merge_files (char* first_path, char* second_path, int i)
{
	FILE *first;
	FILE *second;
	first = fopen (first_path, "r");
	if (first == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", first_path);
		return NULL;
	}
	second = fopen (second_path, "r");
	if (second==NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", second_path);
		fclose (first);
		return NULL;
	}
	char *buffer = calloc (255, sizeof(char));
	int num = i/2+1;
	char buff[12];
	sprintf (buff, "%d", num);
	num = i/2;
	char file_path [30] = "tmp_";
	strcat (file_path, buff);
	char tmp[] = ".txt";
	strcat (file_path, tmp);
	int leng = strlen(file_path);
	char* path = calloc (leng+1, sizeof(char));
	strcpy(path, file_path);
	FILE *result;
	result = fopen (file_path, "w");
	
	int k=1, j=1;
	while (k>0 || j>0)
	{
		if (k>0)
		{
			if (fgets(buffer, 255, first) != NULL)
				fputs (buffer, result);
			else
				k = 0;
		}
		if (j>0)
		{
			if (fgets(buffer, 255, second) != NULL)
				fputs (buffer, result);
			else
				j = 0;
		}
	}
	fclose(first);
	fclose(second);
	fclose(result);
	free (buffer);	
	return path;
}

int make_structure (char *file_path, table* table, int index)
{
	FILE *fp;
	fp = fopen (file_path, "r");
	if (fp == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany1\n", file_path);
		return -1;
	}
	int size = 0;
	char *buffer = calloc (255, sizeof(char));
	while (fgets(buffer, 255, fp) != NULL)
		size++;
	fclose (fp);
	fp = fopen (file_path, "r");
	if (fp == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany2\n", file_path);
		free(buffer);
		return -1;
	}

	table -> blocks[index] = calloc (1, sizeof(block));

	table -> blocks[index] -> size = size;
	table -> blocks[index] -> lines = calloc(size, sizeof(char*));

	
	for (int i=0; i<size; i++)
	{
		fgets(buffer, 255, fp);
		int len = strlen (buffer);
		len++;
		table -> blocks[index]->lines[i] = calloc(len+1, sizeof(char));
		strcpy (table -> blocks[index]->lines[i], buffer);
	}
	fclose (fp);
	remove (file_path);
	free (buffer);

	return index;
}

int length (int index, table* table)
{
	if (index >= table -> size || index <0)
		return 0;
	if (table -> blocks[index] == NULL)
		return 0;
	return table -> blocks[index] -> size;
}

void remove_block (int index, table* table)
{
	if (index >= table -> size || index <0)
		return;
	if (table -> blocks[index] == NULL)
		return;
	for (int i=0; i< table->blocks[index]->size; i++)
	{
		if (table->blocks[index]->lines[i] != NULL)
			free (table->blocks[index]->lines[i]);
	}
	free (table->blocks[index]);
	table->blocks[index] = NULL;
	if (table -> free_index > index)
		table -> free_index = index;
}

void remove_row (int index, int row, table* table)
{
	if (index >= table -> size || index <0)
		return;
	if (table -> blocks[index] == NULL)
		return;
	if (index >= table -> blocks[index] -> size || index <0)
		return;
	if (table -> blocks[index] -> lines[row] == NULL)
		return;
	free (table -> blocks[index] -> lines[row]);
	table -> blocks[index] -> lines[row] = NULL;
}

void print_result (table* table)
{
	for (int i=0; i<table->size; i++)
	{
		if (table->blocks[i] == NULL)
			continue;
		for (int j=0; j<table->blocks[i]->size; j++)
		{
			if (table->blocks[i]->lines[j] == NULL)
				continue;
			printf("%s", table->blocks[i]->lines[j]);
		}
		printf("\n");
	}
}

void remove_all (table* table)
{
	for (int i=0; i<table->size; i++)
		remove_block(i, table);
	free (table);
}


