#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    int i;
    char *buffer = malloc(DBSIZE);
   
    char* table_name = "test1";
    dbuse(table_name);

    while (1) {
        memset(buffer, '*', DBSIZE);

        //Интерактивное меню выбора для операции
        puts("———————————————————————————————————————————————");
        printf("Количество записей в базе: %d\n", dbreccount()); 
        printf("Максимальная длина записи: %d\n\n", DBSIZE-1);
        puts("1. ADD.\t\t Append - добавление записи в базу");
        puts("2. UPD.\t\t Update - обновление выбранной записи в базе");
        puts("3. DEL.\t\t Delete - пометить запись на удаление");
        puts("4. READ.\t Read - прочитать запись из базы");
        puts("5. ALLR.\t All records - вывод записей в базе");
        puts("6. EXIT.\t Exit - выйти из программы");    

        puts("\nВведите пункт меню: ");
        scanf("%[^\n]%*c", key);
        //while(getchar() != '\n') {}
        
        if(!strcmp(key, "ADD") || !strcmp(key, "1")) { 
            printf("Введите строку: ");
            scanf("%[^\n]%*c", buffer);
            dbappend(buffer);
        }
        else if(!strcmp(key, "UPDATE") || !strcmp(key, "UPD") || !strcmp(key, "2")) { 
            printf("Введите индекс элемента: ");
            scanf("%d", &i);
            while(getchar() != '\n') {}
            if (!dbgoto(i)) { //Обработка ошибки
                puts("Элемента с таким индексом не существует\n");
                continue;
            }
            puts("\n");
            printf("Введите строку: ");
            scanf("%[^\n]%*c", buffer);
            dbwrite(buffer);
        }
        else if(!strcmp(key, "DELETE") || !strcmp(key, "DEL") || !strcmp(key, "3")) { 
            printf("Введите индекс элемента: ");
            scanf("%d", &i);
            while(getchar() != '\n') {}
            if (!dbgoto(i)) { //Обработка ошибки
                puts("Элемента с таким индексом не существует");
                continue;
            }
            dbdelete(i);
        }
        else if(!strcmp(key, "READ") || !strcmp(key, "4")) { 
            printf("Введите индекс элемента: ");
            scanf("%d", &i);
            while(getchar() != '\n') {}
            if (!dbgoto(i)) { //Обработка ошибки
                puts("Элемента с таким индексом не существует");
                continue;
            }
            puts("");
            dbread(buffer);
            printf("Элемент на %d индексе: %s", i, buffer);
        }
        else if(!strcmp(key, "ALLRECORDS") || !strcmp(key, "ALLR") || !strcmp(key, "5")) { 
            for (i=1;i<=dbreccount();i++) {
                dbgoto(i);
                dbread(buffer);
                printf("Элемент на %d индексе: %s", i, buffer);
            }
        }
        else if(!strcmp(key, "EXIT") || !strcmp(key, "6")) { 
            puts("Выхожу из программы");
            dbclose();
            exit(1);
        }
        printf("\n");
        
        memset(key,0,sizeof(key));
    }
    free(buffer);
}