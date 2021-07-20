#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "base.h"

static struct dbinfo db;

int dbuse(char *fname) {
    /* 
    Создание/использование файла базы данных
    */
    FILE * fp;
    db.rec = fileno(fp);
    if ((db.rec = open(fname, O_RDWR)) == -1)
        db.rec = open(fname, O_RDWR | O_CREAT);
    if (db.rec == -1)
        return 0;
    strcpy(db.name, fname);
    return 1;
}

int dbreccount(void) {
    /* 
    Подсчет количества записей в базе путем разделения
    общего количества байт на размер длины записи.

    Возвращает:
    >>> Количество записей в базе
    */
    int count, now, end;
    now = lseek(db.rec, 0, SEEK_CUR);
    end = lseek(db.rec, 0, SEEK_END);
    end /= DBSIZE;
    count = end;
    lseek(db.rec, now, SEEK_SET);
    return count;
}

int dbgoto(int number) {
    /* 
    Перемещение указателя на начало записи number
    */
    if (number > dbreccount() || number < 0)
        return 0;
    if (number == 0) {
        lseek(db.rec, 0, SEEK_END);
        return 1;
    }
    lseek(db.rec, DBSIZE * (number - 1), SEEK_SET);
    return 1;
}


int dbwrite(void *src) {
    if (strlen(src) > DBSIZE-1) return 0;
    if (write(db.rec, src, DBSIZE-1)) {
        write(db.rec, "\n", 1);
        return 1;
    }
    return 0;
}



int dbread(void *dest) {
    if (!read(db.rec, dest, DBSIZE))
        return 0;
    return 1;
}

int dbappend(void *src) {
    dbgoto(0);
    return dbwrite(src);
}


int dbdelete(int number){
    dbgoto(number);
	if(write(db.rec, "#", 1)) return 1;
	return 0;
}

void dbclose(void) {
    dbcompress();
    
    if (!dbreccount()) {
        remove(db.name);
        return;
    }
    close(db.rec);
    return;
}


int dbcompress(void) {
    FILE * fp = tmpfile();
    int tmp = fileno(fp);
    char buffer[DBSIZE];

    for(int i=1; i<=dbreccount(); i++) {
        dbgoto(i);
        dbread(buffer);
        if(*buffer != '#') write(tmp, buffer, DBSIZE);
    }
    close(db.rec);
    db.rec = open(db.name, O_WRONLY | O_CREAT | O_TRUNC);
    lseek(tmp, 0, SEEK_SET);

    while(read(tmp, buffer, DBSIZE))
        write(db.rec, buffer, DBSIZE);
    close(tmp);
}
    