#ifndef __NAMED_SEMAPHORE_PROCESS__
#define __NAMED_SEMAPHORE_PROCESS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SHMSZ 32

char SEM_NAME[] = "test_for_sema";

#endif
