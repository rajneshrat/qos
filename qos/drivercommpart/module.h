#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>
 
typedef struct
{
    int status, dignity, ego;
} query_arg_t;
 
#define QUERY_GET_VARIABLES 4 //_IOR('q', 1, query_arg_t *)
#define QUERY_CLR_VARIABLES 5 //_IO('q', 2)
#define QUERY_SET_VARIABLES 6 //_IOW('q', 3, query_arg_t *)
 
#endif
