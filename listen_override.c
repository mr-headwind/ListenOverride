#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <dlfcn.h>
#include <stdio.h>
 
// To compile: gcc -shared -fPIC listen_override.c -o listen_override.so -ldl
// To run:     LD_PRELOAD=./listen_override.so prog_name

typedef int (*orig_listen_type)(int sockfd, int backlog);
 
int listen(int sockfd, int backlog)
{
    if (backlog < 8)
    	backlog = 8;

    orig_listen_type orig_listen;

    printf("Override listen(int sockfd, int backlog)\n");
    orig_listen = (orig_listen_type) dlsym(RTLD_NEXT, "listen");

    return orig_listen(sockfd, backlog);
}
