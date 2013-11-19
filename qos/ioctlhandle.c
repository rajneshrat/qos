#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include "qos_types.h"
#include "bandwidthsetter.h"

#define FIRST_MINOR 0
#define MINOR_CNT 1

MODULE_LICENSE("GPL");

struct global_qos QosValues;

static struct class *device_class;

static dev_t dev;
static struct cdev c_dev;

static int qos_open(struct inode *i, struct file *f)
{
    return 0;
}
static int qos_close(struct inode *i, struct file *f)
{
    return 0;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int qos_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long qos_ioctl(struct file *f, unsigned int command, unsigned long arg)
#endif
{
    int status = 0;
    switch (command) {
    case GET_TOTAL_USERS:
        printk("0 is sent\n");
        int test = 9;
        if (copy_to_user((int *)arg, &test, sizeof(int)))
        {
            return -EACCES;
        }
        break;
    case 1:
        printk("1 is sent\n");
        int test2;
        if (copy_from_user(&test2, (int *)arg, sizeof(int)))
        {
            return -EACCES;
        }
        SetBandwidthKBps(test2);
        printk("Test2 = %d\n", test2);
        break;
    case 2:
        printk("2 is sent\n");
        break;
    }
    return status;
}

static struct file_operations qos_file_operations =
{
    .owner = THIS_MODULE,
    .open = qos_open,
    .release = qos_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = qos_ioctl
#else
    .unlocked_ioctl = qos_ioctl
#endif
};

int ioctl_init(void)
{
    int status = 0;
    if( (status = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "qos_ioctl")) < 0 ) {
        printk("Failed to allocate char device region\n");
        return status;
    }
    else {
        printk("Successfully allocated char device region\n");
    }
    cdev_init(&c_dev, &qos_file_operations);
    if ((status = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return status;
    }

//to create the device file in /dev
    if (IS_ERR(device_class = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        printk("Failed at creating class of qos_ioctl\n");
        return -1;
    }
    if (IS_ERR(device_create(device_class, NULL, dev, NULL, "qos_handle")))
    {
        class_destroy(device_class);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        printk("Failed at creating device file in /dev\n");
        return -1;
    }
    return status;
}

void ioctl_exit(void)
{
    device_destroy(device_class, dev);
    class_destroy(device_class);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}


