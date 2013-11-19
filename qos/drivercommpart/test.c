#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "qos_types.h"
 
#include "module.h"

struct global_qos UserLandQos;
 
void GetTotalUsers(int fd)
{
    int TotalUsers = 0;
 
    if (ioctl(fd, 0, &TotalUsers) == -1)
    {
        perror("query_apps ioctl get");
    }
    else
    {
        printf("TotalUSers : %d\n", TotalUsers);
    }
}

void SetGlobalBandwidthLimit(int fd, int AllowedKBps)
{
    if (ioctl(fd, 1, &AllowedKBps) == -1)
    {
        perror("Failed in setting Global Qos");
    }
}

int main(int argc, char *argv[])
{
    char *file_name = "/dev/qos_handle";
    int fd;
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("query_apps open");
        return 2;
    }
    GetTotalUsers(fd);
    int val = atoi(argv[1]); 
    SetGlobalBandwidthLimit(fd, val);
    close (fd);
    return 0;
}
