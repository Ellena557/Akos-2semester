/*
 ============================================================================
 Name        : AllLab.c
 Author      : Elena Gorskaya
 Version     :
 Copyright   : 
 Description : The 1st Lab with function paste
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

    if (!strcmp(argv[1], "paste")) {

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
