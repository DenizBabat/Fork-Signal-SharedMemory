#ifndef __HW4__
#define __HW4__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>

#define EGGSSUGAR "eggs and sugar"
#define EGGSFLOUR "eggs and flour"
#define EGGSBUTTER "egg and butter"
#define FLOURBUTTER "flour and butter"
#define FLOURSUGAR "flour and sugar"
#define BUTTERSUGAR "butter and sugar"

char *ingredients[] = {EGGSSUGAR,EGGSFLOUR,
                        EGGSBUTTER,FLOURBUTTER,
                        FLOURSUGAR,BUTTERSUGAR};


#define SHSIZE 30//shared memory SHSIZE
#define N 6	//process number

//structure chef
typedef struct Chef
{
	bool eggs;
	bool flour;
	bool butter;
	bool sugar;
	bool cake;

}Chef;

//Whosalesaler chef
typedef struct Wholesaler
{
	bool eggs;
	bool flour;
	bool butter;
	bool sugar;
	bool cake;

}Wholesaler;

//chef and whosaler varaible
Chef chef;
Wholesaler saler;


/* shared memory file descriptor */
int shm_fd, shm_fd2;

/* pointer to shared memory obect */
void* ptr, *ptrcake;

/*semaphore descriptor*/
sem_t *sp, *sp2;

/* name of the shared memory object */
const char* name = "shmem";
const char* shmname2 = "shmem";
const char* semname = "sema";
const char* semname2 = "sema2";


struct sigaction user1, user2;
sigset_t sigset;

static void handlerparent(int signal);
static void handlerchild(int signal);


void readshmem(int chefsquence);
void writeshmem();



#endif //__HW4__