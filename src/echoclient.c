/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"


typedef struct {
    int loggedin;
    char my_hostname[MAXLINE];
    char my_port[MAXLINE];  
} client;

client c;

int main() 
{
    
    client c;
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;
    c.loggedin = 0;

    while(c.loggedin == 0) {
        if(Fgets(buf, MAXLINE, stdin) != NULL) {
            if(buf[0] == '/') {
                // smid string i tmp fil
                if(handlecmd(buf)){
                     printf("SUCCESS\n");
                     c.loggedin = 1;
                } 
            }
        }
    }

    host = "127.0.0.1"; 
    port = "1337";

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        if(buf[0] == '/') {
            // smid string i tmp fil
            handlecmd(buf);
        }
    	Rio_writen(clientfd, buf, strlen(buf));
    	Rio_readlineb(&rio, buf, MAXLINE);
    	Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
    return 0;
}
/* $end echoclientmain */
