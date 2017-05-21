#pragma once
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct CSharedData {
    size_t online;
    char* onlineUsers[100];
};

int utl_write_struct_sh_mem( struct CSharedData* data );

int utl_read_struct_sh_mem( struct CSharedData* data );
