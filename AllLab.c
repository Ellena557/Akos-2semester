/*
 ============================================================================
 Name        : Lab1.c
 Author      : Elena Gorskaya
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>
#include <err.h>

static int _mkdir_p(const char *path) {
	const size_t len = strlen(path);
	char _path[PATH_MAX];
	char *p;
	errno = 0;
	if (len > sizeof(_path) - 1) {
		return errno;
	}
	strcpy(_path, path);

	for (p = _path + 1; *p; p++) {
		if (*p == '/') {
			*p = '\0';
			if (mkdir(_path, S_IRWXU) != 0) {
				if (errno != EEXIST) {
					return errno;
				}
			}
			*p = '/';
		}
	}

	if (mkdir(_path, S_IRWXU) != 0) {
		return errno;
	}

	return 0;
}

int _mkdir(char* dir) {
	errno = 0;
	if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 && errno == 0) {
		return 0;
	} else if (errno == EEXIST) {
		return EEXIST;
	} else {
		return errno;
	}
}

void _error(int errorCodem, char* dir) {
	switch (errorCodem) {
	case 0:
		printf("Lab1 mkdir: Directory '%s' created successfully\n", dir);
		break;
	case EEXIST:
		printf("Lab1 mkdir: cannot create directory ‘%s’: File exists\n", dir);
		break;
	default:
		printf("Error: %s\n", strerror(errno));
		break;

	}
}

void _mkdirPrimary(int argc, char **argv) {
	if (argc == 2) {
		puts("Enter directory name");
	} else if (!strcmp(argv[2], "-p") && argc == 3) {
		puts("mkdir: missing operand");
	} else if (!strcmp(argv[2], "-p") && argc > 3) {
		while (--argc >= 3) {
			_error(_mkdir_p(argv[argc]), argv[argc]);
		}
	} else {
		while (--argc >= 2) {
			_error(_mkdir(argv[argc]), argv[argc]);
		}
	}
}

void _w() { //not working final-- it will be done later!
	printf("USER\tTTY\tFROM\tLOGIN\tIDLE\tJCPU\tPCPU\tWHAT\n");
	int fd;
	struct utmp record;
	int reclen = sizeof(struct utmp);

	fd = open(UTMP_FILE, O_RDONLY);
	if (fd == -1) {
		perror("oops");
		exit(1);
	}

	while (read(fd, &record, reclen) == reclen)
		if(!strcmp(record.ut_id,":0"))
		 printf("%s\t%s\t%s\t%d\n", record.ut_user, record.ut_line, record.ut_id, record.ut_tv.tv_usec);


	close(fd);
}

void _paste(int filesCount, char **files, char* delimiter) {
	FILE *fd[filesCount];
	int linesmax = 0;
	int ch;
	for (int i = 0; i < filesCount; i++) {
		int lines = 0;
		if ((fd[i] = fopen(files[i], "r")) == NULL) {
			printf("Error opening source-file %s\n", files[i]);
			return;
		}
		while ((ch = fgetc(fd[i])) != EOF) {
			if (ch == '\n') {
				lines++;
			}
		}
		if (lines > linesmax) {
			linesmax = lines;
		}
		rewind(fd[i]);
	}

	char buf[1024];
	for (int i = 0; i < linesmax; i++) {
		for (int j = 0; j < filesCount; j++) {
			if (fgets(buf, sizeof(buf), fd[j]) != NULL) {
				buf[strlen(buf) - 1] = '\0';
				printf("%s%s", buf, j < filesCount - 1 ? delimiter : "");
			} else {
				printf("%s", j < filesCount - 1 ? delimiter : "");
			}
		}
		printf("\n");
	}

	for (int i = 0; i < filesCount; i++) {
		fclose(fd[i]);
	}

}

char **copy(int count, int from, char **copyFrom) {
	char **copyTo = malloc(sizeof(char*) * (count - from));
	for (int i = 0; copyFrom[i + from] != NULL; i++) {
		copyTo[i] = malloc(strlen(copyFrom[i + from] + 1));
		strcpy(copyTo[i], copyFrom[i + from]);
	}
	return copyTo;
}

int startsWith(const char *pre, const char *str) {
	size_t lenpre = strlen(pre), lenstr = strlen(str);
	return lenstr < lenpre ? 0 : (strncmp(pre, str, lenpre) == 0 ? 1 : 0);
}

char* subString(const char* input, int offset) {
	char* dest = malloc(strlen(input) - offset);
	strncpy(dest, input + offset, strlen(input) - offset);
	return dest;
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		puts("No arguments passed");
		return EXIT_SUCCESS;
	}

	if (!strcmp(argv[1], "mkdir")) {
		_mkdirPrimary(argc, argv);
	} else if (!strcmp(argv[1], "w")) {
		_w();
	} else if (!strcmp(argv[1], "paste")) {

		if (argc <= 2) {
			puts("paste: Enter files names");
			return EXIT_SUCCESS;
		}
		if (startsWith("-d", argv[2])) {
			_paste(argc - 3, copy(argc, 3, argv), subString(argv[2], 2));
		} else {
			_paste(argc - 2, copy(argc, 2, argv), "\t");
		}
	}
	return EXIT_SUCCESS;
}


