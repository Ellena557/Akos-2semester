#include "server_util_f_user_side.h"
#include <string.h>
#include <regex.h>
#include <dirent.h>

char* hash( char* str ) {
    return str;
}

int isCorrect( char* login, char* password ) {
    return 1;
}

int isUserExist( char* login ) {
    DIR* dir = opendir( "users" );
    if (dir) {
        struct dirent* ent;
        while ((ent = readdir( dir )) != NULL) {
            if (!strcmp( ent->d_name, login )) {
                return 1;
            }
        }
        return 0; // Doesn't exist
    } else {
        return 6;
    }
}

int readUserData( char* login, char* getPassword, int* getIsKicked ) {
    char filePath[100];
    sprintf( filePath, "users/%s", login );
    FILE* f = fopen( filePath, "r" );
    if (f == NULL) {
        // файл не удалось открыть
        return 1;
    }
    char buff[100];
    fscanf( f, "%s%s%d", buff, getPassword, getIsKicked );
    fclose( f );
    return 0;
}

int authentication( char* login, char* password ) {
    int res = isUserExist( login );
    if (res == 1) {
        char getPassword[100];
        int getIsKicked;
        readUserData( login, getPassword, &getIsKicked );
        if (!strcmp( getPassword, password )) {
            return 0;
        } else {
            return 2;
        }
    } else {
        if (res == 0) {
            return 1;
        } else {
            return res;
        }
    }
}

int createUser( char* login, char* password, int isKicked ) {
    if (!isCorrect( login, password )) {
        return 1;
    }

    char filePath[100];
    sprintf( filePath, "users/%s", login );
    FILE* f = fopen( filePath, "w" );
    if (f == NULL) {
        return 2;
    }
    fprintf( f, "%s\n%s\n%d\n", login, hash( password ), isKicked );
    fclose( f );
    return 0;
}

int kick( char* login ) {
    int res = isUserExist( login );
    if (res == 1) {
        char getPassword[100];
        int getIsKicked;
        readUserData( login, getPassword, &getIsKicked );
        if (getIsKicked == 1) {
            return 2;
        } else {
            createUser(login, getPassword, 1);
            return 0;
        }
    } else {
        if (res == 0) {
            return 1;
        } else {
            return res;
        }
    }
}
