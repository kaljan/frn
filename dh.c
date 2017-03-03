#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

extern int scvstr(char **str);

int sdir(char *path)
{
	int ret;
	struct stat st;
	DIR *dr;
	struct dirent *de;
	char *fname = 0;

	if ((ret = stat(path, &st)) != 0) {
		printf("[%d] %s\n", ret, strerror(errno));
		return -1;
	}

	if ((st.st_mode & S_IFMT) != S_IFDIR) {
		printf("%s is not directory\n", path);
		return -1;
	}

	dr = opendir(path);
	if (dr == 0) {
		printf("Open directory failed: %s\n", strerror(errno));
		return -1;
	}

	errno = 0;
	while ((de = readdir(dr)) != 0) {
		if (de->d_type == DT_DIR) {
			printf("[%3li] %s/\n", strlen(de->d_name), de->d_name);
			fname = &de->d_name[0];
			scvstr(&fname);
			if (fname != 0) {
				printf("\t%s/\n", fname);
			}
		} else {
			printf("[%3li] %s\n", strlen(de->d_name), de->d_name);
			fname = &de->d_name[0];
			scvstr(&fname);
			if (fname != 0) {
				printf("\t%s\n", fname);
			}

		}
	}

	printf("\n%s\n", strerror(errno));

	closedir(dr);

	return 0;
}
