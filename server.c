#include <stdio.h>
#include <stdlib.h>
#include "client_util_f_message_format_struct.h"
#include "server_util_f_user_side.h"
#include "server_util_mem.h"
#include <dirent.h>
#include "server_til_f_message_reciving_struct.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include "Common.h"


int main() {
    DIR* dir = opendir( "userlist" );
    if (!dir) {
        fprintf( stderr, "Error opening directory with registered users\n" );
        return 1;
    }
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = terminate;
    sigemptyset( &sigIntHandler.sa_mask );
    sigIntHandler.sa_flags = 0;
    sigaction( SIGINT, &sigIntHandler, NULL);
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    i_socket_cd = socket( AF_INET, SOCK_STREAM, 0 );
    printf("sokkket: %d \n", i_socket_cd);
    if (i_socket_cd < 0)
        error( "ERROR opening socket" );
    bzero((char*) &serv_addr, sizeof( serv_addr ));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( DEF_PORT_CD );

    if (bind( i_socket_cd, (struct sockaddr*) &serv_addr, sizeof( serv_addr )) < 0)
        error( "ERROR on binding sokket" );
    listen( i_socket_cd, 100 );
    clilen = sizeof( cli_addr );
    printf( "Starting listening......................\n" );

    while (1) {
        newi_socket_cd = accept( i_socket_cd, (struct sockaddr*) &cli_addr, &clilen );
        if (newi_socket_cd < 0)
            continue;

        pthread_t thread;
        pthread_create( &thread, NULL, newThread, (void*) &newi_socket_cd );
    }
    return 0;
}

void terminate( int param ) {
    close( newi_socket_cd );
    close( i_socket_cd );
    printf( "term %d\n", param );
    exit( 0 );
}

void error( const char* msg ) {
    perror( msg );
    exit( 1 );
}