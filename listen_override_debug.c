#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
 
// To compile: gcc -shared -fPIC listen_override.c -o listen_override.so -ldl
// To run:     LD_PRELOAD=./listen_override.so prog_name

// Debug version

typedef int (*orig_listen_type)(int sockfd, int backlog);

static FILE *dbug = NULL;
 
int listen(int sockfd, int backlog)
{
    int r;
    orig_listen_type orig_listen;

    //dbug = fopen("/tmp/listen_preload.dbg", "a");
    dbug = fopen("/tmp/listen_preload.dbg", "w+");

    if (sockfd != 0)
	{fprintf(dbug, "Listen preload: socket %d, backlog %d\n", sockfd, backlog); fflush(dbug);}
    else
	fprintf(dbug, "Err: Socket is 0\n"); fflush(dbug);
    
    if (backlog < 8)
    	backlog = 8;

    orig_listen = (orig_listen_type) dlsym(RTLD_NEXT, "listen");

    fprintf(dbug, "Calling original...\n"); fflush(dbug);
    r = orig_listen(sockfd, backlog);

    if (r < 0)
	{fprintf(dbug, "Returned error - %d %s\n", r, strerror(errno)); fflush(dbug);}
    else
	fprintf(dbug, "Returned success...\n"); fflush(dbug);

    fclose(dbug);
    return r;
}
