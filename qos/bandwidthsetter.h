
#ifndef __bandwidthsetterh__

#define __bandwidthsetterh__
void intialize_bsetter();
void cleanup_bsetter();
void SetBandwidthKBps(int MaxKBps);

extern struct sk_buff *my_skb;
extern int sent ;

#include "queue.h"
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

#endif
