#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>

/*
 --------------------------------------------------------------------
 Массив для дранслитерации с кирилицы латиницу

 а - a, б - b, в - v, г - g, д - d, е - e , ё - io, ж - zh, з - z,
 и - i, й - j, к - k, л - l, м - m, н - n, о - o, п - p, р - r, с - s
 т - t, у - u, ф - f, х - h, ц - c, ч - ch, ш - sh, щ - shch, ы - y
 э - e, ю - iu, я - ia
 --------------------------------------------------------------------
 */
static const char translit_array[33][5] = {
	"a\0",  "b\0",    "v\0", "g\0", "d\0", "e\0", "zh\0", "z\0",
	"i\0",  "j\0",    "k\0", "l\0", "m\0", "n\0", "o\0",  "p\0",
	"r\0",  "s\0",    "t\0", "u\0", "f\0", "h\0", "c\0",  "ch\0",
	"sh\0", "shch\0", "\0",  "y\0", "\0",  "e\0", "iu\0", "ia\0", "io\0"
};


char hstr[256];
static int wrngc;
static char *strp;
static uint8_t *ustrp;

static inline void chktr(void)
{
	while (*ustrp != 0) {

		if (*ustrp == 0xD0) {
			ustrp++;
			if (((*ustrp >= 0x90) && (*ustrp <= 0xBF)) ||
				(*ustrp == 0x01)) {
				wrngc++;
			}
		}

		if (*ustrp == 0xD1) {
			ustrp++;
			if (((*ustrp >= 0x80) && (*ustrp <= 0x8F)) ||
				(*ustrp == 0x91)) {
				wrngc++;
			}
		}

		ustrp++;
	}
}

const char *tr(uint16_t c)
{
	if (c == 0) {
		return NULL;
	}

	if (c >= 0xD090 && c < 0xD0B0) {
		return &translit_array[c - 0xD090][0];
	} else if (c >= 0xD0B0 && c <= 0xD0BF) {
		return &translit_array[c - 0xD0B0][0];
	} else if (c >= 0xD180 && c <= 0xD18F) {
		return &translit_array[((c - 0xD180) + 16)][0];
	} else if (c == 0xD001 || c == 0xD191 || c == 0xD081) {
		/* опять же проблемы стандартов. 'Ё' прописная
		 * имеет код 0xD081
		 */
		return &translit_array[32][0];
	}

	return NULL;
}

static inline void trstr(void)
{
	uint16_t trc;
	const char *tc;
	char *tmpptr;
	tmpptr = hstr;
	int n = 0, l;

	while (*ustrp != 0) {
		if (*ustrp == 0xD0 ||
			*ustrp == 0xD1) {

			trc = *ustrp;
			trc <<= 8;
			ustrp++;
			trc |= *ustrp;

			tc = tr(trc);

			if (tc == 0) {
				ustrp++;
				continue;
			}

			l = strlen(tc);

			n += l;

			if (n >= 255) {
				break;
			}

			sprintf(tmpptr, "%s", tc);
			tmpptr += l;
		} else {
			*tmpptr = *ustrp;
			tmpptr++;
		}
		ustrp++;
	}
}

static inline void chkstr(void)
{
	while (*strp != 0) {
		if ((isalnum(*strp) == 0) &&
			((*strp != '_') || (*strp != '.'))) {
			wrngc++;
		}
		strp++;
	}
}

static inline void fltstr(void)
{
	while (*strp != 0) {
		if ((isalnum(*strp) == 0) &&
			((*strp != '_') || (*strp != '.'))) {
			*strp = '_';
		} else {
			*strp = tolower(*strp);
		}
		strp++;
	}

	if ((strp = strchr(hstr, '_')) != NULL) {
		while (*strp != 0) {
			if (*(strp + 1) == '_') {
				memmove(strp, strp + 1, strlen(strp));
				continue;
			}
			if (*(strp + 1) == '\0') {
				*strp = '\0';
				break;
			}
			strp++;
			strp = strchr(strp, '_');
		}
	}
}

int scvstr(char **str)
{
	if ((str == NULL) || (*str == NULL) ||
		(strlen(*str) < 1) || (strlen(*str) > 255)) {
		return -1;
	}

	wrngc = 0;
	ustrp = (uint8_t *)(*str);

	chktr();

	if (wrngc > 0) {
		ustrp = (uint8_t *)(*str);
		trstr();
	}

	printf("%s\n", hstr);

	wrngc = 0;
	strp = hstr;

	chkstr();

	if (wrngc > 0) {
		strp = hstr;
		fltstr();
	}
	*str = hstr;

	return 0;
}
