/*#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>

#include <linux/skbuff.h>
#include <linux/string.h>
#include <linux/inet.h>
*/
#include "bandwidthsetter.h"
#include "ioctlhandle.h"
//#include "queue.c"



MODULE_LICENSE("GPL");


static struct nf_hook_ops netfilter_ops_in;/* IP PRE ROUTING */
static struct nf_hook_ops netfilter_ops_out; /* NF_IP_POST_ROUTING */
struct sk_buff *sock_buff;
struct iphdr *ip_header;
struct net_device *dev;
char *in_face = "eth0";
char *out_face = "eth1";

unsigned int main_hook(unsigned int hooknum,
                       const struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int(*okfn)(struct sk_buff*))
{
    struct icmphdr* icmp;
    if((skb->priority == 0) ) { // && (tcp_header->ece !=1)){ //||  dport == 8080 || tcp_header->dest == 21){
        my_skb = skb_get ( skb);//, GFP_ATOMIC);
        my_skb->priority = 1;
        InsertPacketInQueue(my_skb);
        sent = 1;
        return NF_DROP;
    }
    return NF_ACCEPT;
}

int init_module(void)
{
    qos_initialize();
    intialize_bsetter();
    ioctl_init();
    netfilter_ops_in.hook       = main_hook;
    netfilter_ops_in.pf         = PF_INET;
    netfilter_ops_in.hooknum    = NF_INET_PRE_ROUTING; /*NF_INET_PRE_ROUTING;*/
    netfilter_ops_in.priority   = NF_IP_PRI_FIRST;

    nf_register_hook(&netfilter_ops_in);


    printk(KERN_INFO "qos initialized called\n");
    return 0;
}

void cleanup_module(void)
{
    cleanup_bsetter();
    ioctl_exit();
    printk(KERN_INFO "ending qos\n");
    nf_unregister_hook(&netfilter_ops_in);
    printk(KERN_INFO "unregistering\n");
}
