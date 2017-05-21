#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "client_util_f_message_format_struct.h"
#include "Common.h"
#include "client.h"


int main( int argc, char* argv[] ) {
    if (argc < 2) {        printf( "Run with server IP!\n" );        exit( 0 );    }
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = terminate;
    sigemptyset( &sigIntHandler.sa_mask );
    sigIntHandler.sa_flags = 0;
    sigaction( SIGINT, &sigIntHandler, NULL);
    struct sockaddr_in serv_addr;
    struct hostent* server;
    i_socket_cd = socket( AF_INET, SOCK_STREAM, 0 );
    if (i_socket_cd < 0)
        error( "ERROR opening socket" );
    server = gethostbyname( argv[1] );
    if (server == NULL) {
        fprintf( stderr, "ERROR, no such SERVER =%c\n",argv[1] );
        exit( 0 );
    }
    bzero((char*) &serv_addr, sizeof( serv_addr ));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length );
    serv_addr.sin_port = htons( DEF_PORT_CD );
    if (connect( i_socket_cd, (struct sockaddr*) &serv_addr, sizeof( serv_addr )) < 0)
        error( "ERROR connecting" );
    ssize_t n;
    printf( "Hi\n" );
    char type = 'i'; // login
    char messBody[100];
    char login[32];
    char password[32];
    printf( "\nLogin: " );
    bzero( login, 32 );
    fgets( login, 32, stdin );
    login[strlen( login ) - 1] = 0;    printf( "\nPassword: " );
    bzero( password, 32 );
    fgets( password, 32, stdin );
    password[strlen( password ) - 1] = 0; 
    sprintf( messBody, "%s\n%s", login, password );
    char* message = (char*) malloc( DEFAULT_LEHGTH );
    size_t messSize = formMessage( message, type, messBody, strlen( messBody ));
    n = write( i_socket_cd, message, messSize );
    if (n < 0)
        error( "\nERROR writing to socket" );
    bzero( message, DEFAULT_LEHGTH );
    n = read( i_socket_cd, message, DEFAULT_LEHGTH );
    if (n < 0)
        error( "\nERROR reading from socket" );
    recognizeMessage( message, &messSize, &type, messBody );
    if (type == 's') {
        switch (messBody[0]) {
            case '0':
                printf( "OK!\n" );
                break;
            case '3':
                printf( "Error %c", type );
                exit( 0 );
            case '4':
                printf( "Connect passowrd or login error:%c\n", type );
                exit( 0 );
            default:
                printf( "Server not responding !!!  ->%c<-\n", type );
                exit( 0 );
        }
    } else {
        printf( "Server not responding !!!  ->%c<-\n", type );
        exit( 0 );
    }
    printf( "U in! simple commands :\n" );
    printf( "%%r message - simple chat or mesage\n" );
    printf( "%%o - logout\n" );
    printf( "%%h count - history\n" );
    printf( "%%l - list\n" );
    printf( "Example: 'r HiHi' - send message\n" );
    pthread_t thread;
    pthread_create( &thread, NULL, receiver, (void*) &type );
    while (type != 'o') {
        char buffer[DEFAULT_LEHGTH];
        bzero( buffer, DEFAULT_LEHGTH );
        fgets( buffer, 3, stdin ); 
        type = buffer[0];
        size_t bufferLen = 0;
        if (type == 'o' || type == 'l') {
            bzero( buffer, DEFAULT_LEHGTH );
        } else {
            fgets( buffer, DEFAULT_LEHGTH, stdin );
            bufferLen = strlen( buffer );
            buffer[--bufferLen] = 0; 
        }

        char res[DEFAULT_LEHGTH];
        size_t resSize = formMessage( res, type, buffer, bufferLen );
        n = write( i_socket_cd, res, resSize );
        if (n < 0)
            error( "ERROR writing to socket" );
    }
    sleep( 2 );
    close( i_socket_cd );
    return 0;
}

void* receiver( void* type ) {   //receiving msg
    char buffer[DEFAULT_LEHGTH];
    while (*(char*) type != 'o') {
        bzero( buffer, DEFAULT_LEHGTH );
        printf( "\t reciving\n" );
        ssize_t n = read( i_socket_cd, buffer, DEFAULT_LEHGTH );
        if (n < 0)
            error( "ERROR reading from socket" );

        size_t buff_size = 0;
        char typeGet = 0;
        char messageBody[DEFAULT_LEHGTH];
        recognizeMessage( buffer, &buff_size, &typeGet, messageBody );
        switch (typeGet) {
            case 'k':
                printf( "U was refused from chat: %s\n", messageBody );
                exit( 0 );
            case 'r':
                printf( "%s\n", messageBody );
                break;
            default:
                printf( "\tERROR: Not responding Server %c\n", *(char*) typeGet );
        }
    }
    printf( "goodbye!\n" );
}

void error( const char* msg ) { // printing an error appearance
    perror( msg );
    exit( 0 );
}

void terminate( int param ) {
    close( i_socket_cd );
    printf( "terminate %d\n", param );
    exit( 0 );
}
