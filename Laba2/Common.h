#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "server_util_f_user_side.h"
#include "server_util_mem.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "server_til_f_message_reciving_struct.h"
#include <netinet/in.h>
#include <signal.h>
#include "client_util_f_message_format_struct.h"
#include "server_util_f_user_side.h"
#include "server_util_mem.h"
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#define DEF_PORT_CD 8080
#define DEFAULT_LEHGTH 1000
#define _STR_MAIN_PASSWORD "kiss"

void error( const char* msg );
void* newThread( void* );
