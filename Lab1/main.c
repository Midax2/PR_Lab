#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "double_list.h"

int main() {
    double_list* head = NULL;
    int index;
    char* data = NULL;

    while (1) {
        printf("Menu:\n");
        printf("1. Wstaw napis do listy\n");
        printf("2. Usun napis z listy po indeksie\n");
        printf("3. Wyswietl zawartosc listy\n");
        printf("4. Koniec programu\n");
        printf("Wybierz opcje: ");

        int option;
        if (scanf("%d", &option) != 1) {
            printf("Nieprawidlowa opcja, sprobuj ponownie.\n\n");
            while (getchar() != '\n');
            continue;
        }
        int default_size = 10;
        switch (option) {
        case 1:
            printf("\nPodaj napis do wstawienia: ");
            data = (char*)malloc(default_size * sizeof(char));
            int i = 0;
            char c = getchar();
            while ((c = getchar()) != '\n') {
                data[i++] = c;
                if (i == default_size) {
                    default_size *= 10;
                    data = (char*)realloc(data, default_size * sizeof(char));
                }
            }
            data[i] = '\0';
            dinsert(&head, _strdup(data));
            free(data);
            break;
        case 2:
            printf("\nPodaj indeks napisu do usuniecia: ");
            scanf("%d", &index);
            dremove(&head, index);
            break;
        case 3:
            printf("\nZawartosc listy:\n");
            dlist(head);
            break;
        case 4:
            dclear(&head);
            printf("\nKoniec programu.\n");
            dclear(&head);
            return 0;
        default:
            printf("\nNieprawidlowa opcja, sprobuj ponownie.\n");
            break;
        }
    }
    return 0;
}
