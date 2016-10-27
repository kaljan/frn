#include "cfname.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

// Variables --------------------------------------------------------
/*
 --------------------------------------------------------------------
 Массив для дранслитерации с кирилицы латиницу
 
 а - a, б - b, в - v, г - g, д - d, е - e , ё - io, ж - zh, з - z,
 и - i, й - j, к - k, л - l, м - m, н - n, о - o, п - p, р - r, с - s
 т - t, у - u, ф - f, х - h, ц - c, ч - ch, ш - sh, щ - shch, ы - y
 э - e, ю - iu, я - ia
 -------------------------------------------------------------------- 
 */
const char translit_array[33][5] = {
	"a\0",  "b\0",    "v\0", "g\0", "d\0", "e\0", "zh\0", "z\0", 
	"i\0",  "j\0",    "k\0", "l\0", "m\0", "n\0", "o\0",  "p\0",
	"r\0",  "s\0",    "t\0", "u\0", "f\0", "h\0", "c\0",  "ch\0", 
	"sh\0", "shch\0", "\0",  "y\0", "\0",  "e\0", "iu\0", "ia\0", "io\0"
};

/*
 --------------------------------------------------------------------
 Эта функция выдаёт номер ячейки в массиве для транслитерации

 --------------------------------------------------------------------
 */
cfname_error get_cyrrinic_num(char * str)
{
	uint8_t *strptr;
	uint16_t chn;
	
	if (str == 0) {
		return CFN_BAD_PINTER;
	}
	strptr = (uint8_t*)str;
	if ((*strptr == 0xD0) || (*strptr == 0xD1)) {

		chn = ((uint16_t)(*strptr));
		chn = (chn << 8) | ((uint16_t)(*(strptr + 1) & 0xFF));

		if (chn >= 0xD090 && chn < 0xD0B0) {
			chn -= 0xD090;
		} else if (chn >= 0xD0B0 && chn <= 0xD0BF) {
			chn -= 0xD0B0;
		} else if (chn >= 0xD180 && chn <= 0xD18F) {
			chn -= 0xD180;
			chn += 16;
		} else if (chn == 0xD001 || chn == 0xD191) {
			chn = 32;
		}
		return ((int)chn);

	}
	
	return CFN_BAD_STRING;
}

/*
 --------------------------------------------------------------------
 Эта функция проверяет нужна ли транслитерация

 В случае если нужна транслитерация, фукнция вернёт новый размер
 имени.

 Если транслитерация не нужна, функция вернёт 0

 И если произошли какая-то ошибка функция вернёт отрицательное
 значение

 --------------------------------------------------------------------
 */
int is_translit_need(char *fname)
{
	char *strptr;
	int newsize = 0, nc = 0, c;

	if (fname == 0) {
		return (int)CFN_BAD_PINTER;
	}

	if (strlen(fname) < 1) {
		return (int)CFN_BAD_STRING;
	}

	// вычисляем новый размер строки, и за одно проверяем есть ли в ней кирилица
	strptr = fname;

	while (*strptr != 0) {
		c = get_cyrrinic_num(strptr);

		if ((c >= 0) && (c <= 32)) {
			strptr++;
			newsize += strlen(&translit_array[c][0]);
			nc++;
		} else {
			newsize++;
		}

		strptr++;
	}

	if (nc == 0) {
		return (int)CFN_NOERROR;
	}

	return newsize + 1;
}

/*
 --------------------------------------------------------------------
 Транслитерация имени файла

 --------------------------------------------------------------------
 */
cfname_error translit_utf8(char *fname, char *trfname)
{
	char *fnameptr, *trfnameptr;
	int c;

	if (fname == 0) {
		return CFN_BAD_PINTER;
	}

	if (strlen(fname) < 1) {
		return CFN_BAD_STRING;
	}

	if (trfname == 0) {
		return CFN_BAD_PINTER;
	}

	fnameptr = fname;
	trfnameptr = trfname;
	while (*fnameptr != 0) {
		c = get_cyrrinic_num(fnameptr);
		if ((c >= 0) && (c <= 32)) {
			fnameptr++;
			if (strlen(&translit_array[c][0]) == 0) {
				fnameptr++;
				continue;
			}
			sprintf(trfnameptr, "%s", &translit_array[c][0]);
			trfnameptr += strlen(&translit_array[c][0]);
		} else {
			*trfnameptr = *fnameptr;
			trfnameptr++;
		}
		fnameptr++;
	}
	return CFN_NOERROR;
}

/*
 --------------------------------------------------------------------
 Почистим имя файла от лишних символов

 Определим правило именования файлов. В имени файлов должны быть только
 латинские строчные буквы a-z; цифры 0-9; и символы '_'. Точка должна
 быть только одна для разделения расширения. В конце имени файла может
 быть только одно нижнее подчёркивание, в начале не больше двух.
 
 --------------------------------------------------------------------
 */
cfname_error replace_symbols(char *fname)
{
	char *tmpfname, *fnameptr, *fnameext;
		
	if (fname == 0) {
		return CFN_BAD_PINTER;
	}
	
	if (strlen(fname) == 0) {
		return CFN_BAD_STRING;
	}
	
	// Создадим новую строку в которой и будем работать
	tmpfname = malloc(strlen(fname) + 1);
	if (tmpfname == 0) {
		return CFN_ALLOC_ERROR;
	}
	strcpy(tmpfname, fname);
	
	// Сохраним отдельно расширение
	fnameptr = strrchr(tmpfname, '.');
	fnameext = malloc(strlen(fnameptr) + 1);
	if (fnameext == 0) {
		free(tmpfname);
		return CFN_ALLOC_ERROR;
	}
	strcpy(fnameext, fnameptr);
	*fnameptr = 0;	// временно завалим расширение
	
	// Заменим все левые символы на '_'
	fnameptr = tmpfname;
	while (*fnameptr != 0) {
		if (isalnum(*fnameptr) == 0) {
			*fnameptr = '_';
		}
		*fnameptr = tolower(*fnameptr);
		fnameptr++;
	}
	
	// Теперь уберём все симолы '_' следующие один за одним и в конце
	fnameptr = strchr(tmpfname, '_');
	
	while (fnameptr != 0) {
		if (*(fnameptr + 1) == '_') {
			memmove(fnameptr, fnameptr + 1, strlen(fnameptr + 1) + 1);
		} else if (*(fnameptr + 1) == '\0') {
			*fnameptr = 0;
		} else {
			fnameptr++;
		}
		
		fnameptr = strchr(fnameptr, '_');
	}
	
	// Сформируем новое имя файла
	if ((strlen(tmpfname) + strlen(fnameext)) > strlen(fname)) {
		free(tmpfname);
		free(fnameext);
		return CFN_BAD_STRING;
	}
	
	sprintf(fname, "%s%s", tmpfname, fnameext);
	
	free(tmpfname);
	free(fnameext);	
	
	return CFN_NOERROR;
}

/*
 --------------------------------------------------------------------
 Подготовка нового имени файла

 --------------------------------------------------------------------
 */
cfname_error prepare_new_file_name(char *fname, char ** nfname)
{
	char *newfname = 0;
	int len;
	cfname_error err;

	if (fname == 0) {
		return CFN_BAD_PINTER;
	}

	if (strlen(fname) == 0) {
		return CFN_BAD_STRING;
	}

	if (nfname == 0) {
		return CFN_BAD_PINTER;
	}

	if (*nfname != 0) {
		return CFN_BAD_PINTER;
	}

	len = is_translit_need(fname);
	if (len < 0) {
		return len;
	}

	if (len > 0) {
		newfname = malloc(len);
		if (newfname == 0) {
			return CFN_ALLOC_ERROR;
		}

		err = translit_utf8(fname, newfname);
		if (err != CFN_NOERROR) {
			return err;
		}
	} else {
		newfname = malloc(strlen(fname) + 1);
		if (newfname == 0) {
			return CFN_ALLOC_ERROR;
		}

		strcpy(newfname, fname);
	}

	err = replace_symbols(newfname);
	if (err != CFN_NOERROR) {
		if (newfname != 0) {
			free(newfname);
		}
		return err;
	}

	*nfname = malloc(strlen(newfname) + 1);

	if (*nfname == 0) {
		free(newfname);
		return CFN_ALLOC_ERROR;
	}

	strcpy(*nfname, newfname);
	free(newfname);

	return CFN_NOERROR;
}

/*
 --------------------------------------------------------------------
 +---+------+ +---+------+ +---+------+ +---+------+	
 | А | D090 | | Р | D0A0 | | а | D0B0 | | р | D180 | 	
 | Б | D091 | | С | D0A1 | | б | D0B1 | | с | D181 | 	
 | В | D092 | | Т | D0A2 | | в | D0B2 | | т | D182 | 	
 | Г | D093 | | У | D0A3 | | г | D0B3 | | у | D183 | 	
 | Д | D094 | | Ф | D0A4 | | д | D0B4 | | ф | D184 | 	
 | Е | D095 | | Х | D0A5 | | е | D0B5 | | х | D185 | 	
 | Ж | D096 | | Ц | D0A6 | | ж | D0B6 | | ц | D186 | 	
 | З | D097 | | Ч | D0A7 | | з | D0B7 | | ч | D187 | 	
 | И | D098 | | Ш | D0A8 | | и | D0B8 | | ш | D188 | 	
 | Й | D099 | | Щ | D0A9 | | й | D0B9 | | щ | D189 | 	
 | К | D09A | | Ъ | D0AA | | к | D0BA | | ъ | D18A | 	
 | Л | D09B | | Ы | D0AB | | л | D0BB | | ы | D18B | 	
 | М | D09C | | Ь | D0AC | | м | D0BC | | ь | D18C | 	
 | Н | D09D | | Э | D0AD | | н | D0BD | | э | D18D | 	
 | О | D09E | | Ю | D0AE | | о | D0BE | | ю | D18E | 	
 | П | D09F | | Я | D0AF | | п | D0BF | | я | D18F |
 +---+------+ +---+------+ +---+------+ +---+------+ 	

 | Ё | D001 | JO
 | ё | D191 |

 0xD001, 0xD090..0xD0BF, 0xD180..0xD18F, 0xD191

 0xD0AA, 0xD0AC, 0xD18A, 0xD18C

 --------------------------------------------------------------------
 */
