/*
 --------------------------------------------------------------------
 Программа для наведения порядка на дисках.

 Что сделать:
 1. Фильтр имени файлов от левых символов (сделано, не тестировано)

 2. Транслитерация (сделано, не тестировано)

 3. Просмотр файлов и папок на файловой системе

 4. Переименование файлов

 5. Филтр файлов с одинаковыми именами

 --------------------------------------------------------------------
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>

extern int scvstr(char **str);

// Variables --------------------------------------------------------
struct timeval t1;
struct timeval t2;
double elapsed_time;

/*
 --------------------------------------------------------------------
 Подсчёт прошедшего времени

 --------------------------------------------------------------------
 */
double get_elapsed_time(struct timeval *tstart, struct timeval *tstop)
{
	struct timeval et;
	et.tv_sec = tstop->tv_sec - tstart->tv_sec;
	et.tv_usec = tstop->tv_usec - tstart->tv_usec;
	return (((double)et.tv_sec * (double)1000) + (((double)et.tv_usec) / (double) 1000));
}

/*
 --------------------------------------------------------------------
 Преобразуем строку в хексдамп и выведем на экран

 --------------------------------------------------------------------
 */
int str_to_hexdump(char *str)
{
	int l, i;

	if (str == 0) {
		return -1;
	}

	l = strlen(str);
	if (l < 1) {
		return -1;
	}

	for (i = 0; i < l; i++) {
		if (((i % 8) == 0) && (i != 0)) {
			printf(" \n");
		}
		printf ("%02X ", (unsigned char)(*(str + i)));
	}
	printf(" \n");

	return 0;
}


/*
 --------------------------------------------------------------------
 Нужно сделать простенькую програмку, которая бы переименовывала файлы
 и папки. Что нужно делать с названиями:
 - Заменять пробелы на нижнее подчёркивание
 - Заменять скобки
 - Заменять двоеточия
 - Транслитерация с кирилицы
 - Заменять всякие не нужные символы
 - Убирать лишние точки
 - убирать все строчные символы
 Грубо говоря нужно переименовывать файлы так, что бы у них оставались
 только маленькие буквы латинского алфавита, цифры от 0 до 9, одна точка
 и нижнее подчёркивание.

 Но для начала:
 1. Сначала просто выведем имя файла.

 --------------------------------------------------------------------
 */
int main (int argc, char **argv)
{
	char *str;
	struct stat st;
	int ret;

	if (argc > 1) {
		argv++;
		str = *argv;

		if ((ret = stat(str, &st)) != 0) {
			printf("Stat file failed: %s\n", strerror(errno));
			return 0;
		}



		return 0;

		printf("input : %s\n", str);
		scvstr(&str);
		printf("output: %s\n", str);
	}

	return 0;
}
