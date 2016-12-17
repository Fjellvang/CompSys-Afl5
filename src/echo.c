/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"

void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;

    printf("ECHO START\n");
    Rio_readinitb(&rio, connfd);
    printf("READ INIT\n");
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
    	printf("server received %d bytes\n", (int)n);
    	Rio_writen(connfd, buf, n);
    }
}

