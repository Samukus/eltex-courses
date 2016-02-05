#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define NETLINK_USER 31
MODULE_LICENSE("GPL");

/*------Реализация Рандома------*/
static int constant = 65539;
static int m = 2147483648;
static unsigned long seed = 1; //  Начальное значение

static unsigned long rand(void) {
	seed = ( constant * seed) % m;
	return seed;
}
/*-------------------------------*/

struct sock *nl_sk = NULL;
static struct netlink_kernel_cfg cfg;

static void nl_recv_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlh; //заголовок сообщения
    int pid;
    struct sk_buff *skb_out; //содержимое пакета
    int msg_size;
    char msg_buf[256] = "";
    char *msg="";
    int res;

    printk(KERN_INFO "[nl_mod]: Entering: %s\n", __FUNCTION__);

    nlh=(struct nlmsghdr*)skb->data;

	if( !strcmp((char*)nlmsg_data(nlh),"GetRandom") )
		sprintf(msg_buf, "%lu", rand() );
	else 
		sprintf(msg_buf, "Send \"GetRandom\" to receive random int");
	
	msg=msg_buf;
    msg_size=strlen(msg);

    printk(KERN_INFO "[nl_mod]: Netlink received msg payload: %s\n",(char*)nlmsg_data(nlh));
    
	pid = nlh->nlmsg_pid; /*pid of sending process */
    skb_out = nlmsg_new(msg_size,0);

    if(!skb_out)
    {
        printk(KERN_ERR "[nl_mod]: Failed to allocate new skb\n");
        return;
    } 
    nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);

    NETLINK_CB(skb_out).dst_group = 0; 
    strncpy(nlmsg_data(nlh),msg,msg_size);
    
    res=nlmsg_unicast(nl_sk,skb_out,pid);
    
    if(res<0)
        printk(KERN_INFO "[nl_mod]: Error while sending bak to user\n");
}

static int __init init(void)
{
    printk("[nl_mod]: Entering: %s\n",__FUNCTION__);
    cfg.input = nl_recv_msg;
	nl_sk=netlink_kernel_create(&init_net, NETLINK_USER, &cfg );
    if( !nl_sk )
    {
        printk("[nl_mod]: Error creating socket: %d\n", nl_sk);
        return -1;
    }
	printk("[nl_mod]: NetLink module successfuly added\n");
    return 0;
}

static void __exit exit(void){
	printk("[nl_mod]: Entering: %s\n",__FUNCTION__);
    netlink_kernel_release(nl_sk);
    printk(KERN_INFO "[nl_mod]: NetLink module successfuly removed\n");
}

module_init(init);
module_exit(exit);


