#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "double_list.h"

void dlist(double_list*head) {
	double_list* p;
	
	for (p = head; p != NULL; p = p->next) {
		printf("%s\n", p->data);
	}
	printf("\n");
}

void dinsert(double_list** phead, char* data) {
	double_list* my_list = *phead;
	double_list* new_list = (double_list*)malloc(sizeof(double_list));
	new_list->data = data;
	new_list->next = NULL;
	new_list->prev = NULL;
	if (my_list == NULL) {
		*phead = new_list;
		return;
	}
	while (strcmp(my_list->data, data) > 0 && my_list->next != NULL) {
		my_list = my_list->next;
	}
	if (strcmp(my_list->data, data) <= 0) {
		if (my_list->prev == NULL) {
			*phead = new_list;
		}
		else { 
			new_list->prev = my_list->prev;
			my_list->prev->next = new_list;
		}
		new_list->next = my_list;
		my_list->prev = new_list;
	}
	else {
		new_list->prev = my_list;
		my_list->next = new_list;
	}
}



void dremove(double_list** phead, int index) {
	double_list* my_list = *phead;
	if (my_list == NULL) {
		printf("Lista jest pusta\n");
		return;
	}
	int i = 0;
	while (my_list != NULL) {
		if (i == index) {
			if (my_list->prev == NULL && my_list->next == NULL) {
				free((*phead)->data);
				free(*phead);
				*phead = NULL;
				printf("Element zostal usuniety z listy\n");
				return;
			}
			else if (my_list->prev == NULL && my_list->next != NULL) {
				*phead = my_list->next;
				my_list->next->prev = NULL;
				free(my_list->data);
				free(my_list);
				printf("Element zostal usuniety z listy\n");
				return;
			}
			else if (my_list->prev != NULL && my_list->next == NULL) {
				my_list->prev->next = NULL;
				free(my_list->data);
				free(my_list);
				printf("Element zostal usuniety z listy\n");
				return;
			}
			else {
				my_list->prev->next = my_list->next;
				my_list->next->prev = my_list->prev;
				free(my_list->data);
				free(my_list);
				printf("Element zostal usuniety z listy\n");
				return;
			}
		}
		i++;
		my_list = my_list->next;
	}
	printf("\nPodano index poza zakresem\n");
}

void dclear(double_list** phead) {
	double_list* my_list = *phead;
	if (my_list == NULL) {
		return;
	}
	while (my_list->next != NULL) {
		my_list = my_list->next;
		if (my_list->prev != NULL) {
			free(my_list->prev->data);
			free(my_list->prev);
		}
	}
	free(my_list->data);
	free(my_list);
	*phead = NULL;
	return;
}