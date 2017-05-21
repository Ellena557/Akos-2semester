#include <stdio.h>
#include "client_util_f_message_format_struct.h"

int sp_log_write_msg_to_history( char* message, size_t len ) {
    FILE* f = fopen( "hst.lst", "a" );
    if (f == NULL) {
        return 2;
    }
    fwrite( intToBytes(5 + len), 1, 4, f );
    fwrite( message, 1, len, f );
    fprintf( f, "\n" );
    fclose( f );
    return 0;
}

int sp_log_read_msg_history( char* message, size_t* len, long int* pos ) {
    FILE* f = fopen( "hst.lst", "r" );
    if (f == NULL) {
        return 2;
    }
    fseek(f, *pos, SEEK_SET);
    char buff[4];
    fread( buff, 1, 4, f );
    *len = bytesToInt(buff);
    fread( message, 1, *len, f );
    *pos += *len;
    *len -= 5;
    fclose( f );
    return 0;
}
