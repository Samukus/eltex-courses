#include <sys/socket.h>
#include <linux/netlink.h>
#include <string.h>

#define NETLINK_USER 31
#define MAX_PAYLOAD 1024  /* maximum payload size*/

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = 0;
struct iovec iov;
int sock_fd;
struct msghdr msg;
char MSG[64] = "My first message to the kernel";

int main(int argc,char *argv[]) {
	if(argc<2){
		printf("Usage: argv[1] = \"Message to the kernel\" \n");
		printf("Default: \"My first message to the kernel\" \n");			
	}
	else
		strcpy(MSG,argv[1]);
    
	sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if(sock_fd<0) return -1;

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();  /* self pid */
    /* interested in group 1<<0 */
    bind(sock_fd, (struct sockaddr*)&src_addr,
        sizeof(src_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;   /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */
    
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    strcpy(NLMSG_DATA(nlh), MSG);
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    //printf("Sending message to kernel\n");
    sendmsg(sock_fd,&msg,0);
    //printf("Waiting for message from kernel\n");

    /* Read message from kernel */
    recvmsg(sock_fd, &msg, 0);
    printf("Message from Kernel Space:\n%s\n", NLMSG_DATA(nlh));
    close(sock_fd);
    return 0;
}
