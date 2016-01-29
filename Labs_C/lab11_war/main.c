#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define N 5
#define GOLD 500
#define PORT 25000

int stop = 0;

size_t socket_init_server(int *sock,int port){

    if ((*sock=socket(AF_INET,SOCK_DGRAM,0))<0){
        error("Opening socket");
        return -1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(*sock,(struct sockaddr *)&server, sizeof(server))){
        error("binding");
        return -2;
    }
    return sizeof(server);
};

void recieving(int mysocket,int gold){
    char buf[64];
    int n;
    int mined = 0;
    pid_t pid;
    printf("start recieving:\n");
    while(gold > 0){
        n = recvfrom(mysocket,buf, sizeof(buf),NULL,NULL,NULL);
        if(n == -1){
            perror("Recive error");
            continue;
        }
        buf[n] = '\0';
        pid = str_to_pid(buf,&mined);
        if(mined > gold) mined = gold;
        gold -= mined;
        printf("Unit %d mined: %d, In Mine: %d\n",pid,mined,gold);
    }
    close(mysocket);
};

size_t socket_init_client(int *sock,struct sockaddr_in *server,int port){

    // Шаг 1 - создание сокета
	*sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (*sock < 0) perror("socket");

	// Шаг 2 - связывание

	server->sin_family = AF_INET; // указание адресации
	inet_aton("127.0.0.1", &server->sin_addr);
    server->sin_port = htons(port); // извлечение порта

	return sizeof(server);
};

void sig_act(int sig){
    if(sig == SIGUSR1)
        stop = 1;
};

void start_sending(int sock,struct sockaddr_in server){
    sleep(1);
    printf("Unit: #%d starts working!\n",getpid());
    signal(SIGUSR1,sig_act);
    char buffer[64];
    int n;
    while(1){
        srand(clock(0));
        if( stop == 1 ) break;
        sleep(rand()%3+1);
        //printf("Pid:%d\n",getpid());
        sprintf(buffer,"%d  %d",rand()%50+25,getpid());
        n = sendto(sock,&buffer[0],strlen(buffer),0,(const struct sockaddr *)&server,sizeof(server));
        if(!n)  perror("Sending error");
    }
    printf("Unit: #%d is finished work\n",getpid());
    close(sock);
    return;
};

pid_t str_to_pid(char *buf,int *gold){
    int i,j;
    *gold = atoi(buf);
    char tmp[64];
    for(i=0;i<strlen(buf);i++)
        if( buf[i] == ' '  ){
            for(j=0;i<strlen(buf);j++,i++){
                tmp[j]=buf[i];
            }
            return (pid_t)atoi(tmp);
        }
    return -1;
};

void exit_forks(pid_t *childs,int n){
    int i;
    for(i=0;i<n;i++){
        kill(childs[i],SIGUSR1);
        wait(EXIT_SUCCESS);
    }
};




int main(){
    int i;
    pid_t units[N];

    int socket_rcv;
    int socket_snd;   // дескрипторы
    struct sockaddr_in server; // структуры адреса сервера

    socket_init_server(&socket_rcv,PORT);
	socket_init_client(&socket_snd,&server,PORT);

	for(i=0;i<N;i++){
        units[i] = fork();
        if(units[i] == 0){
            start_sending(socket_snd,server);
            return 0;
        }
	}

    recieving(socket_rcv,GOLD);
    exit_forks(units,N);

    return(0);
}
