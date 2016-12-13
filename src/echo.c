/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
void isStringCommand(const char *c);

void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
    	printf("server received %d bytes\n", (int)n);
        if(buf[0] == '/') {
            // smid string i tmp fil
            char tmp[MAXLINE];
            strcpy(tmp, buf);
            isStringCommand(tmp);
        }
    	Rio_writen(connfd, buf, n);
    }
}

void isStringCommand(const char *c){
    const char s[2] = " ";
    char *token;
    token = strtok(c, s);
    int tkcnt = 0;    
    while ( token != NULL) {
        printf(" %s\n", token);
        token = strtok(NULL, s);
        tkcnt++;
    }
    printf("nr of tokens: %i\n", tkcnt);
}
/* $end echo */

