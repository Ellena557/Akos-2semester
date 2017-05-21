#include "server_util_mem.h"

key_t key = 4096;

int utl_write_struct_sh_mem( struct CSharedData* data ) {
    int shmid;
    struct CSharedData* shm;

    // ........segment.
    if ((shmid = shmget( key, sizeof( struct CSharedData ), IPC_CREAT | 0666 )) < 0) {
        perror( "shmget" );
        return 1;
    }

    if ((shm = shmat( shmid, NULL, 0 )) == (struct CSharedData*) -1) {
        perror( "shmat" );
        return 1;
    }

    *shm = *data;

    return 0;
}

int utl_read_struct_sh_mem( struct CSharedData* data ) {
    int shmid;
    struct CSharedData* shm;

    // Creation of a segment.
    if ((shmid = shmget( key, sizeof( struct CSharedData ), IPC_CREAT | 0666 )) < 0) {
        perror( "shmget" );
        return 1;
    }

    if ((shm = shmat( shmid, NULL, 0 )) == (struct CSharedData*) -1) {
        perror( "shmat" );
        return 1;
    }

    *data = *shm;

    return 0;
}