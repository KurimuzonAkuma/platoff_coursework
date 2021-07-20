#include <stdio.h>

#define DBSIZE 128
char key[10]; // максимальная длина аргумента

struct dbinfo {
    char name[127];
    int rec;
};

int dbuse(char* fname);
int dbreccount(void);
int dbgoto(int number);
int dbwrite(void*src);
int dbread(void*dest);
void dbclose(void);
int dbappend(void*src);
int dbdelete(int);
int dbcompress(void);