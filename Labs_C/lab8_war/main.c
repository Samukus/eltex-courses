#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

#define N 10
#define GOLD 400

struct mymesg {
long mesg_type;
char mesg_data[2];
pid_t pid;
};

int main()
{
int gold = GOLD;
int mqid;
int msgflg = 0600 | IPC_CREAT;
key_t key = 1234;
long type = 12;
int i;
pid_t childpid[N];

if( (mqid = msgget(key,msgflg)) >= 0 )
    printf("mqid = %d\n",mqid);
else
    return -1;

for(i=0;i<N;i++){
    childpid[i] = fork();
    if(childpid[i] == 0)
    {
        printf("Unit %d starts working\n",getpid());
        struct mymesg buf;
        strcpy(buf.mesg_data,"10");
        buf.mesg_type = type;
        buf.pid = getpid();
        while(1){
            srand(clock());
            sleep(rand()%3+1);
            msgsnd(mqid, &buf,sizeof(buf),0);
        }
    }
}

struct mymesg buf_rcv;
while(gold){
    msgrcv(mqid,&buf_rcv,sizeof(buf_rcv),type,0);
    gold -= atoi(buf_rcv.mesg_data);
    printf("Unit %d mined %d, %d gold in mine.\n",buf_rcv.pid,atoi(buf_rcv.mesg_data),gold);
}

for(i=0;i<N;i++){
    kill(childpid[i],SIGTERM);
}

int status;
if( (status = msgctl(mqid, IPC_RMID, NULL)) >= 0 )
    printf("status = %d\n",status);
else
    return -1;

exit(0);
}
