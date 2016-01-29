#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define N 5

#define GOLD 400

pid_t Start_Unit(int wr);
int Mining_Gold(int rd,int gold,pid_t child);

int main()
{
srand(clock(0));
int our_pipe[N][2];
pid_t childpid[N];
int gold = GOLD;
int i;

for(i=0;i<N;i++){
    pipe(our_pipe[i]);
    childpid[i] = Start_Unit(our_pipe[i][1]);
}

while(1){
    i = rand()%N;
    if( !(gold = Mining_Gold(our_pipe[i][0],gold,childpid[i])) )
        break;
}

for(i=0;i<N;i++){
    kill(childpid[i],SIGTERM);
    close(our_pipe[i][0]);
    close(our_pipe[i][1]);
}

}

int Mining_Gold(int rd,int gold,pid_t child){
    char buf[2];
    int mined;
    if(read(rd,buf,sizeof(buf))){
        mined = atoi(buf);
        mined = (gold < mined) ? gold : mined;
        gold -= mined;
        printf("Unit %.2d mined %d gold, %d gold in mine\n",child,mined,gold);
    }
    return gold;
};

pid_t Start_Unit(int wr){
    pid_t child;
    child = fork();
    if(child == 0)
    {
        printf("Unit %d starts working\n",getpid());
        char wrbuff[2];
        while(1){
            sprintf(wrbuff,"%d",10);
            write(wr,wrbuff,sizeof(wrbuff));
            sleep(1);
        }
    }
    return child;
};



