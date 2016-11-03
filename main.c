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

#include "cfname.h"

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

int dir_work(char *dirname) 
{
	DIR *dp;
	int dfd, ret;
	struct dirent * de;
	char fname[256], *nfname;
	
	nfname = 0;
	
	if (dirname == 0) {
		printf("Bad directory name\n");
		return -1;
	}
	
	printf("\nWork with directories!\n\n");
	
	printf("Working directory = %s\n", dirname);
	
	dp = opendir(dirname);
	
	if (dp == 0) {
		printf("Error opening dir: %s\n", strerror(errno));
		return -1;	
	}
	
	de = readdir(dp);
	
	while (de != 0) {
		if (strcmp(de->d_name, ".") == 0) {
			de = readdir(dp);
			continue;
		}
		
		if (strcmp(de->d_name, "..") == 0) {
			de = readdir(dp);
			continue;
		}
		
		strcpy(fname, de->d_name);		
		printf("Old name: %s\n", fname);
		ret = prepare_new_file_name(fname, &nfname);
		if (ret != 0) {
			printf("Error getting new file name: %d\n", ret);
			break;
		}
		
		if (nfname == 0) {
			printf("Error getting new file name\n");
			break;
		}
		
		printf("New name: %s\n\n", nfname);
		
		free(nfname);
		
		nfname = 0;
		de = readdir(dp);
		
	}
	
	dfd = closedir(dp);
	if (dfd < 0) {
		printf("Error close dir: %s\n", strerror(errno));
		return -1;
	}
	
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
//	char * fname;
//	char * fname2 = 0;
//	int ret;
	
	
	
	if (argc > 1) {
		dir_work(*(argv + 1));
//		printf("Full path: %s\n", *(argv + 1));
//		fname = strrchr(*(argv + 1), '/');
//		if (fname != 0) {
//			fname++;
			
//			if (*fname == 0) {
//				fname = *(argv + 1);
//				dir_work(fname);
//				return 0;
//			}
			
//			printf("\nOld file name: %s\n", fname);
//			gettimeofday(&t1, NULL);
//			ret = prepare_new_file_name(fname, &fname2);
//			gettimeofday(&t2, NULL);
//			elapsed_time = get_elapsed_time(&t1,&t2);
//			if (elapsed_time > 1.000) {
//				printf("Elapsed time: %6.3f ms\n", elapsed_time);
//			} else {
//				printf("Elapsed time: %6.3f us\n", elapsed_time * 1000);
//			}

//			if (ret != 0) {
//				printf("prepare_new_file_name(fname, &fname2) failed with error %d\n", ret);
//			}
//			printf("New file name: %s\n", fname2);
//		}
	}
	return 0;
}

///*
// --------------------------------------------------------------------
// Фильтруем строку
 
// Определим правило именования файлов. В имени файлов должны быть только
// латинские строчные буквы a-z; цифры 0-9; и символы '_'. Точка должна
// быть только одна для разделения расширения. В конце имени файла может
// быть только одно нижнее подчёркивание, в начале не больше двух.
 
// --------------------------------------------------------------------
// */
//int str_filt(char *str)
//{
//	char *s, *se;
//	int l;

//	if (str == 0) {
//		return -1;
//	}
	
//	if (strlen(str) < 1) {
//		return -1;
//	}
	
//	// Меняем всякие левые символы на '_'
//	s = str;
//	while ((*s) != 0) {
//		if ((isalnum(*s) == 0) && 
//			(*s != '.') && (*s != '_')) {
//			*s = '_';
//		}
//		*s = tolower(*s);
//		s++;
//	}
	
//	/* Заменим все точки символом нижнего подчёркивания. кроме последней, 
//	 * которая разделяет расширение и имя файла.
//	 */
//	s = strchr(str, '.');
//	se = strrchr(str, '.');

//	while (s != 0 && s != se) {
//		if (s == 0) break;
//		*s = '_';
//		s = strchr(s, '.');
//	}
//	printf("%s\n", str);
//	// Уберём все следующие друг за другом символы нижнего подчёркивания
//	s = strchr(str, '_');
//	while (s != 0) {
//		if (*(s + 1) == 0) {
//			break;
//		}
//		if ((*(s + 1) == '_') || (*(s + 1) == '.')) {
//			l = strlen(s + 1);
//			memmove(s, s + 1, l + 1);
//		}
//		s = strchr(s + 1, '_');
//	}

//	return 0;
//}
//	char *cwd;
	
//	printf("Get current work dir name\n");
//	cwd = getcwd(NULL, 0);
	
//	if (cwd == 0) {
//		perror("getcwd");
//		return -1;
//	}
	
//	printf("cwd = %s\n", cwd);

//dfd = dirfd(dp);

//if (dfd < 0) {
//	printf("Error getting dir desc: %s\n", strerror(errno));
//	return 0;
//}

//printf("Dir descriptor: %d\n", dfd);
