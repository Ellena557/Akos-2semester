#include "server_til_f_message_reciving_struct.h"
#include "server_util_f_user_side.h"
#include "Common.h"
#include "client_util_f_message_format_struct.h"

int switchMessType( char type, char* messBody, size_t messBSize, int newi_socket_cd ) {
    printf( "t: %c, m: %s\n", type, messBody );
    char message[DEFAULT_LEHGTH];
    switch (type) {
        case 'i':
            type = 's';
            char login[32];
            char password[32];
            bzero( login, 32 );
            bzero( password, 32 );
            int j = 0;
            int i = 0;
            for (i = 0; i < messBSize; ++i) {
                if (messBody[i] == '\n')
                    break;
                login[j++] = messBody[i];
            }
            j = 0;
            for (int k = i + 1; k < messBSize; ++k) {
                password[j++] = messBody[k];
            }
            int userStatus = isUserExist( login );
            if (userStatus == 0) {
                int crStatus = createUser( login, password, 0 );
                if (crStatus == 0) {
                    bzero( messBody, DEFAULT_LEHGTH );
                    messBody[0] = '0';
                } else {
                    bzero( messBody, DEFAULT_LEHGTH );
                    messBody[0] = '4';
                }
            } else if (userStatus == 1) {
                int auStatus = authentication( login, password );
                if (auStatus == 0) {
                    bzero( messBody, DEFAULT_LEHGTH );
                    messBody[0] = '0';
                } else {
                    bzero( messBody, DEFAULT_LEHGTH );
                    messBody[0] = '3';
                }
            }
            break;
        case 'r':
            break;
        default:
            printf( "Client side error type=: %d\n", type );
            break;
    }

    size_t messSizeTT = formMessage( message, type, messBody, messBSize );
    ssize_t nTT = write( newi_socket_cd, message, messSizeTT );
    if (nTT < 0) {
        error( "INTERNAL ERROR SCT:0997" );
    }
}

int activate_connection_For_User( int newi_socket_cd ) {
    char buffer[DEFAULT_LEHGTH];
    ssize_t n;
    printf( "Started talking\n" );
    char type = '0';
    while (type != 'o') {
        bzero( buffer, DEFAULT_LEHGTH );
        n = read( newi_socket_cd, buffer, DEFAULT_LEHGTH );
        if (buffer[0] == 0) break;
        if (n < 0)
            error( "ERROR reading from socket" );
        size_t messBSize = 0;
        char messBody[DEFAULT_LEHGTH];
        recognizeMessage( buffer, &messBSize, &type, messBody );
        switchMessType( type, messBody, messBSize, newi_socket_cd );
    }
    printf( "Stopped talking\n" );
    return 0;
}

void* newThread( void* newi_socket_cd ) {
    activate_connection_For_User( *(int*) newi_socket_cd );
}