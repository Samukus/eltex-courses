#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define N 3
int main(int argc, char **argv)
{

    int n=5;
    int gold = 200;


    pid_t *pid;
    pid = malloc(n*sizeof(pid_t));
    FILE *mine;
    mine = fopen("mine.txt","w+");
    fprintf(mine, "%d\n", gold);
    //fprintf(mine, "%s", argv[1]);
    fclose(mine);

    int i;
    for(i=0;i<n;i++){
        pid[i] = fork();
        if(pid[i] == 0){
            int ID = i;
            printf("Unit %d start work\n",getpid());
            sleep(1);
            char tmp[10];
            int gold;
            int *mine;
            while( 1 ){
                sleep(rand()%5);
                mine = open("mine.txt",O_RDWR);
                if(mine == 0){
                    continue;
                }

                if(lockf(mine, F_LOCK, 0) <0){
                    continue;
                }

                read (mine, tmp, sizeof(tmp));
                gold = atoi(tmp);
                if( gold <= 0 ) return 0;
                gold -= 10;
                sprintf(tmp,"%d",gold);
                lseek(mine,0,SEEK_SET);
                write(mine, tmp, sizeof(tmp));
                lockf(mine, F_ULOCK, 0);
                close(mine);
                printf("Unit %d mine 10 gold. %d gold in mine\n",ID,gold);
            }
        }
    }

    pid_t pidclose;
    for(i=0;i<n;i++)
    {
        pidclose = wait(EXIT_SUCCESS);
        printf("%d process successfuly closed\n",pidclose);
    }

    return 0;
}

