#ifndef DATA_H
#define DATA_H

#define SIZE 5
#define PIZZA_TYPE 10


typedef struct pizza {
	int type;
} pizza;

struct Bake {
	int index;
	int busy;
	struct pizza pizzas[SIZE];
} Bake;

struct Table {
	int index;
	int supp;
	int busy;
	struct pizza pizzas[SIZE];
} Table;


#endif