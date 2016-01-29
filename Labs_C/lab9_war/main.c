#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define N 3
#define GOLD 100

int sem_create(char *path){
    union semun {
    int val;   /* значение для SETVAL */
    struct semid_ds *buf;/*буферы для IPC_STAT, IPC_SET */
    unsigned short *array;/* массивы для GETALL, SETALL */
    struct seminfo *__buf; /* буфер для IPC_INFO */
    };

    key_t key;
    int semid;
    key = ftok(path, getpid());
    semid = semget(key, 1, 0666 | IPC_CREAT);
    printf("Sem create: %d\n",semid);
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    return semid;
};

void *shm_init(char *path,size_t size,int *shmid){
    key_t key;
    key = ftok(path, getpid());
    *shmid = shmget(key,size,0666 | IPC_CREAT);
    printf("Shm create: %d\n",*shmid);
    void *ptr = shmat(*shmid,NULL, 0);
    return ptr;
};

void mined_10(int semid,int *gold){

    struct sembuf lock_res = {0, -1, 0};
    struct sembuf ulock_res = {0, 1, 0};
    semop(semid,&lock_res,1);
        if( *gold > 0 )
            *gold -= 10;
        else{
            semop(semid,&ulock_res,1);
            return;
        }
        printf("Unit %d mined 10 gold, %d gold in mine\n",getpid(),*gold);
        //sleep(1);
    semop(semid,&ulock_res,1);
};

void wait_for_childs(int n){
    pid_t pidclose;
    int i;
    //for(i=0;i<n;i++){
     //   kill(childpid[i],SIGTERM);
    //}
    for(i=0;i<n;i++)
    {
        pidclose = wait(EXIT_SUCCESS);
        printf("%d process successfuly closed\n",pidclose);
    }
};

void deinit(int shmid,int semid){
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
};

int main()
{
pid_t childpid[N];
int i;
int shmid;

int semid = sem_create("/home/alex/sem/sem.o");
int *gold = shm_init("/home/alex/sem/sem.o",sizeof(int),&shmid);
*gold = GOLD;

for(i=0;i<N;i++){
    childpid[i] = fork();
    if(childpid[i] == 0)
    {
        while( (*gold) > 0 ){
            mined_10(semid,gold);
        }
        return 0;
    }
}

wait_for_childs(N);
deinit(shmid,semid);

exit(0);
}
