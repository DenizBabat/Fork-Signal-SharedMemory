#include "hw.h"



int main(int argc, char const *argv[])
{
	pid_t ch;
	int i = 0;
    
    int flag[6] = {0,0,0,0,0,0}; 
    int choice = 0;
    int random = 0;
    
    if (argc != 1)
    {
        printf("USAGE: ./exe\n");
        exit(-1);
    }


    sigfillset(&sigset);
    sigdelset(&sigset, SIGINT);

    user2.sa_handler = handlerparent;
    user2.sa_flags = 0;
    signal(SIGINT, handlerparent);

    sem_destroy(sp);
    sem_unlink(semname);
    sp = sem_open (semname, O_CREAT | O_EXCL, 0666, 0);
    if (sp == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }
    sem_destroy(sp2);
    sem_unlink(semname2);
    sp2 = sem_open (semname2, O_CREAT | O_EXCL, 0666, 0);
    if (sp2 == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }


    //create a shread memeory
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SHSIZE);

    //create a shread memeory
    shm_fd2 = shm_open(shmname2, O_CREAT | O_RDWR, 0666);
    /* configure the size of the shared memory object */
    ftruncate(shm_fd2, SHSIZE);

    for (i = 0; i < N; ++i)
    {

        //random malzeme belirleniyor.
        do{
            random = 0;
            srand(time(NULL)); 
            random = rand()%6;
        }while(flag[random] != 0);
        flag[random]  = random+1;

        ch = fork();

        if (ch < 0)
        {
            perror("fork");
            exit(1);
        }  
        else if (ch == 0){ // child read
            //burada rastgele chef'lere malzemeler dagitilir.
            if (strcmp(ingredients[random],EGGSSUGAR)==0){
                chef.eggs = true;
                chef.sugar = true;
                chef.flour = false;
                chef.butter = false;
                chef.cake = false;
            }else if (strcmp(ingredients[random],EGGSFLOUR)==0){
                chef.eggs = true;
                chef.sugar = false;
                chef.flour = true;
                chef.butter = false;
                chef.cake = false;
            }else if (strcmp(ingredients[random],EGGSBUTTER)==0){
                chef.eggs = true;
                chef.sugar = false;
                chef.flour = false;
                chef.butter = true;
                chef.cake = false;
            }else if (strcmp(ingredients[random],FLOURBUTTER)==0){
                chef.eggs = false;
                chef.sugar = false;
                chef.flour = true;
                chef.butter = true;
                chef.cake = false;
            }else if (strcmp(ingredients[random],FLOURSUGAR)==0){
                chef.eggs = false;
                chef.sugar = true;
                chef.flour = true;
                chef.butter = false;
                chef.cake = false;
            }else if (strcmp(ingredients[random],BUTTERSUGAR)==0){
                chef.eggs = false;
                chef.sugar = true;
                chef.flour = false;
                chef.butter = true;
                chef.cake = false;
            }

            break;
        }
       
    }

    if (ch == 0){ // child read
        readshmem(i);
        exit(1);
    }else{

        writeshmem();
    }

    wait(0);

    printf("parent normal out\n");

    sem_destroy(sp);
    sem_unlink(semname);

    sem_destroy(sp2);
    sem_unlink(semname2);

    shm_unlink(name);
    shm_unlink(shmname2);

     return 0;
}

static void handlerparent(int signal){
    static int count = 0;

    if (signal == SIGINT)
    {
        printf("PARENT CAUGHT SIGINT(CTRL-C) - Hahahha(^_^)\n");
    }
    sem_destroy(sp);
    sem_unlink(semname);

    sem_destroy(sp2);
    sem_unlink(semname2);

    shm_unlink(name);
    shm_unlink(shmname2);

    wait(0);

    printf("Parent was last seen.\n");

    exit(1);
}
static void handlerchild(int signal){
    static int count = 0;

    if (signal == SIGINT)
    {
        printf("CHILD CAUGHT SIGINT(CTRL-C) - Hahahha (-_-)\n");
    }

    exit(1);
}


void readshmem(int chefsquence){
        

    bool e=false, f = false, b = false, s = false, c = false; //eggs,flour,butter,sugar,cake;
    char *cake = "cake";

  
    //iki semaphore da burda açıyorum ki çalıssın.
    sp = sem_open (semname,  O_EXCL, 0666, 0);
    sp2 = sem_open (semname2,  O_EXCL, 0666, 0);
   
    user2.sa_handler = handlerchild;
    user2.sa_flags = 0;

    while(1){
         signal(SIGINT, handlerchild);
        if (chef.eggs == true && chef.sugar == true)
        {
            printf("chef%d is waiting for flour and butter\n",chefsquence+1 );
        }else if (chef.eggs == true && chef.flour == true ){
             printf("chef%d is waiting for butter and sugar\n",chefsquence+1 );
        }
        else if (chef.eggs == true && chef.butter == true ){
             printf("chef%d is waiting for flour and sugar\n",chefsquence+1 );
        }
        else if (chef.flour == true && chef.butter == true ){
             printf("chef%d is waiting for eggs and sugar\n",chefsquence+1 );
        }
        else if (chef.flour == true && chef.sugar == true ){
             printf("chef%d is waiting for eggs and butter\n",chefsquence+1 );
        }
        else if (chef.butter == true && chef.sugar == true ){
             printf("chef%d is waiting for flour and eggs\n",chefsquence+1 );
        }

        sem_wait(sp);//semaphore
        //crittical section-----------------------------------------------  

        e = chef.eggs;
        f = chef.flour;
        b = chef.butter;
        s = chef.sugar;
        c = chef.cake;


        //1. shared memory okunur.
        ptr = mmap(0, SHSIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        if (ptrcake == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }

        //burada share memoryde okunan malzemeler gecici olarak aliyor. 
        //eger cake yapabiliyorsak aliriz, cake yapamazsak siradaki chef geliyor.
        if (strcmp(EGGSSUGAR, ptr) == 0)
        {
            e = true;
            s = true;
        }
        else if (strcmp(EGGSFLOUR, ptr) == 0){
            e = true;
            f = true;
        }
        else if (strcmp(EGGSBUTTER, ptr) == 0){
            e = true;
            b = true;
        }
        else if (strcmp(FLOURBUTTER, ptr) == 0){
            f = true;
            b = true;
        }else if (strcmp(FLOURSUGAR, ptr) == 0){
            f = true;
            s = true;
        }else if (strcmp(BUTTERSUGAR, ptr) == 0){
            b = true;
            s = true;
        }else{
            printf("wrong thing was written to shared memory\n");
            exit(1);
        }

        if (e == true && f == true && b == true && s == true)
        {
            c = true; // cake basari ile pisirildi.
        }else{
            c = false;
        }

        if (c == true) //burad 2. shared memory e yazilir.
        {
            printf("chef%d is preparing the dessert\n",chefsquence+1);
            ptrcake = mmap(0, SHSIZE, PROT_WRITE, MAP_SHARED, shm_fd2, 0);
            if (ptrcake == MAP_FAILED)
            {
                perror("mmap");
                exit(1);
            }
            sprintf(ptrcake,"%s",cake);
            sem_post(sp2);
        }else
        {
            sem_post(sp);//semaphore
        }

    }
 
}
void writeshmem(){
   // printf("parent girdi\n");
    int count = 0;
    int random = 0;
    time_t t;

    //iki semaphore da burda açıyorum ki çalıssın.
    sp = sem_open (semname,  O_EXCL, 0666, 0);
    sp2 = sem_open (semname2,  O_EXCL, 0666, 0);


    while(1){
        //her defasında share memory'inin icini temeizliyorum.
        //ptr = mmap(0, SHSIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
       
        /* Intializes random number generator */
   		srand((unsigned) time(&t));
        random = rand()%6;

        //degerin -1 olmasına karsi alinan onlem.
        while(random<0){
            random += 6; //random bir index hesaplanır.
        }
        printf("wholesaler delivers %s\n", ingredients[random]);
        
        //her defasında share memory'inin icini temeizliyorum.
        ptr = mmap(0, SHSIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }
        //burada shared memory e yazilir.
        sprintf(ptr, "%s", ingredients[random]);
        ptr += strlen(ingredients[random]);

        sem_post(sp);
        printf("wholesaler is waiting for the dessert\n");
        sem_wait(sp2);

        //burada ikinci shared memory okunur.
        ptrcake = mmap(0, SHSIZE, PROT_READ, MAP_SHARED, shm_fd2, 0);
        if (ptrcake == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }

        if (strcmp("cake",(char*)ptrcake) == 0)
        {
            printf("wholesaler has obtained the dessert and left to sell it\n");
        }else{
            printf("wholesaler is exit program. Because There is something wrong. \n");
            exit(1);
        }

    }
}

