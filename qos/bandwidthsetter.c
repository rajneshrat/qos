#ifndef __bandwidthsetter__

#define __bandwidthsetter__

#include <linux/module.h>
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

#include <linux/kernel.h>
//#include <linux/module.h>
#include <linux/timer.h>
//#include "queue.c"
#include "bandwidthsetter.h"
#include <linux/hrtimer.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

struct sk_buff *my_skb;

int sent = 0;
static struct timer_list my_timer;

static struct iphdr *ip_header;

static struct tcphdr *tcp_header;

unsigned int sport ,
         dport;
uint ban = 10000;
uint  TotalTime = 0;
uint MinuteTimer = 0;
uint TotalSize = 0;

void SetBandwidthKBps(int MaxKBps)
{
    printk("Setting MaxKBps = %d\n",MaxKBps);
    ban = MaxKBps;
}
static overlaps = 0;
static enum hrtimer_restart function_timer(struct hrtimer *);
static struct hrtimer htimer;
static ktime_t kt_periode;

static enum hrtimer_restart function_timer(struct hrtimer * unused)
{
    int size = 0;

    if(MinuteTimer >= 60000) {
        //printk("Timer of 60 seconds hit\n");
        MinuteTimer = 0;
    }
    static int time = 1;
    // printk( "my_timer_callback called (%ld).\n", jiffies );
    struct sk_buff *my_skb = PopPacketFromQueue();
    static int lastban = 0;
    if( lastban != ban )
    {
        overlaps = 0;
        lastban = ban;
    }
    while(my_skb ) {
        struct iphdr *ip_header;
        ip_header = (struct iphdr*)(my_skb->network_header);
        size = size + ip_header->tot_len;
        netif_rx(my_skb);
        if(size > ban) {
            time = (size - ban) / (ban) + 1;
            if(time == 0 ) {
                time = 1;
            }
            my_skb = PopPacketFromQueue();
            MinuteTimer = MinuteTimer + time;
            goto last ;
        }
        overlaps = 0;
        sent = 1;
        my_skb = PopPacketFromQueue();
    }
    if(size) {
        //    printk("total size %d\n",size);
    }

    static int hit  = 0;
    hit++;
    static int t = 1;
    // printk("Timer hit for %d and t %d\n", hit, t);
    t++;
    // kt_periode = ktime_set(t, 00000000l); //seconds,nanoseconds
    //kt_periode = ktime_set(t, t * 100000000l); //seconds,nanoseconds
last :

    // mod_timer( &my_timer, jiffies + msecs_to_jiffies(time) );
    kt_periode = ktime_set(0, time * 1000l); //seconds,nanoseconds
    hrtimer_forward_now(& htimer, kt_periode);
    return HRTIMER_RESTART;
}

void intialize_bsetter(void)
{
    int ret;
    kt_periode = ktime_set(0, 1000l); //seconds,nanoseconds
    hrtimer_init (& htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    htimer.function = function_timer;
    hrtimer_start(& htimer, kt_periode, HRTIMER_MODE_REL);
    // setup_timer( &my_timer, my_timer_callback, 0 );
    // printk( "Starting timer to fire in 200ms (%ld)\n", jiffies );
//  ret = mod_timer( &my_timer, jiffies + msecs_to_jiffies(1) );
//  if (ret) printk("Error in mod_timer\n");

}

void cleanup_bsetter()
{
    int ret;
    ret = del_timer( &my_timer );
    hrtimer_cancel(& htimer);
    if (ret) printk("The timer is still in use...\n");
}

#endif
