/*
 ============================================================================
 Name        : AllLab.c
 Author      : Elena Gorskaya
 Version     :
 Copyright   : 
 Description : The 1st Lab with function mkdir
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


int main(int argc, char **argv) {
    if (argc <= 1) {
        puts("No arguments passed");
        return EXIT_SUCCESS;
    }

    if (!strcmp(argv[1], "mkdir")) {
        _mkdirPrimary(argc, argv);
    } 
    return EXIT_SUCCESS;
}
