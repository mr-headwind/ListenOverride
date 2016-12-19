#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <dlfcn.h>
#include <stdio.h>

// To compile: gcc -Wall -fPIC -DPIC -c listen_override2.c
//             ld -shared -o listen_override.so listen_override2.o -ldl
// To run:     LD_PRELOAD=./listen_override.so prog_name

 
int (*orig_listen)(int sockfd, int backlog);
 
int listen(int sockfd, int backlog)
{
    if (backlog < 8)
    	backlog = 8;

    return orig_listen(sockfd, backlog);
}
 
void
_init(void)
{
    printf("Override listen(int sockfd, int backlog)\n");
    orig_listen = dlsym(RTLD_NEXT, "listen");
}
