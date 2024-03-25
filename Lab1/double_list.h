#include <stdio.h>
typedef struct double_list double_list;
struct double_list {
	char* data;
	double_list* next;
	double_list* prev;
};

void dinsert(double_list** phead, char *data);

void dlist(double_list*head);

void dremove(double_list** phead, int index);

void dclear(double_list** phead);