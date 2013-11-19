#ifndef QUEUE
#define QUEUE
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

static int TotalPacketsinQueue;

struct qos_sk_packet {
    struct sk_buff *skb;
    unsigned int timespend;
    unsigned int packetpriority;
    struct qos_sk_packet *next;
    struct qos_sk_packet *pre;
};

struct packet_queue {
    struct qos_sk_packet *head;
    struct qos_sk_packet *tail;
    atomic_t queue_lock;
} qos_packet_queue;

void InsertPacketInQueue(struct sk_buff *skb)
{
    while(1) {
        if(atomic_dec_and_test(&qos_packet_queue.queue_lock)) {
            struct qos_sk_packet *tmp = kmalloc(sizeof(struct qos_sk_packet), GFP_ATOMIC);
            if( tmp == NULL ) {
                atomic_inc(&qos_packet_queue.queue_lock);
                printk("Failed in malloc returning\n");
                continue;
            }
            tmp->skb = skb;
//	tmp->timespend =
            tmp->next = NULL;
            if( qos_packet_queue.tail != NULL ) {
                qos_packet_queue.tail->next = tmp;
            }
            qos_packet_queue.tail = tmp;
            if( qos_packet_queue.head == NULL ) {
                qos_packet_queue.head = tmp;
            }
            TotalPacketsinQueue++;
            atomic_inc(&qos_packet_queue.queue_lock);
            break;
        }
        printk("increamenting queue\n");
        atomic_inc(&qos_packet_queue.queue_lock);
        //	break;//continue;
    }
}

struct sk_buff *PopPacketFromQueue()
{
    while(1) {
        if(atomic_dec_and_test(&qos_packet_queue.queue_lock)) {
            struct qos_sk_packet *tmp = qos_packet_queue.head;
            if( tmp == NULL ) {
                atomic_inc(&qos_packet_queue.queue_lock);
                return NULL;
            }
            struct sk_buff *skb = tmp->skb;
            qos_packet_queue.head =  qos_packet_queue.head->next;
            if( qos_packet_queue.head != NULL ) {
                qos_packet_queue.head->pre = NULL;
                if( qos_packet_queue.head->next == NULL) {
                    qos_packet_queue.tail->pre = NULL;
                }
            }
            else {
                qos_packet_queue.tail = NULL;
            }
            kfree (tmp);
            TotalPacketsinQueue--;
            printk("Packets in queue = %d\n", TotalPacketsinQueue);
            atomic_inc(&qos_packet_queue.queue_lock);
            return skb;
        }
        atomic_inc(&qos_packet_queue.queue_lock);
        //	break;
        //continue;
    }
}

void qos_initialize()
{
//	qos_packet_queue.queue_lock(1);
    atomic_set(&qos_packet_queue.queue_lock,1);
    qos_packet_queue.head = NULL;
    qos_packet_queue.tail = NULL;
}

#endif



