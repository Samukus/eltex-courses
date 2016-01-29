#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define N 5
#define GOLD 400

static pthread_mutex_t shared_mutex  = PTHREAD_MUTEX_INITIALIZER;


void *thread(void *arg){
    int *gold = (int *)arg;
    while(1){
        pthread_mutex_lock(&shared_mutex);
        if( *gold > 0){
            *gold -= 10;
            printf ("Unit %x mined 10 gold, %d gold in mine\n",pthread_self(),*gold);
            pthread_mutex_unlock(&shared_mutex);
            sleep(1);
        }
        else{
            pthread_mutex_unlock(&shared_mutex);
            return(*gold);
        }
    }
};

 void wait_for_threads(pthread_t *tid,int n){
    long unsigned res;
    int i;
    for(i=0;i<n;i++){
        pthread_join(tid[i],&res);
        printf("Thread[%d] finished with return %d\n",i,res);
    }
};

void start_threads(pthread_t *tid,int n,int *gold){
    int i;
    for(i=0;i<N;i++){
        printf("Starting %dth thread\n",i);
        pthread_create(&tid[i], NULL, thread, gold);
    }
};

int main(){

int gold = GOLD;
pthread_t tid[N];
start_threads(&tid,N,&gold);
wait_for_threads(&tid,N);
exit(0);

}
