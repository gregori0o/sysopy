#ifndef merge_text
#define merge_text

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct block {
	int size;
	char** lines;
} block;

typedef struct table {
	int size;
	int free_index;
	block** blocks;
} table;


table* create_table (int );

int set_seq (char* , char** );

char* merge_files (char* , char* , int );

int make_structure (char* , table* , int );

int length (int , table* );

void remove_block (int , table* );

void remove_row (int , int , table* );

void print_result (table* );

void remove_all (table* );

#endif