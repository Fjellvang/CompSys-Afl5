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


void linestoread(int x, rio_t *rio);
int main() 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    host = "127.0.0.1"; 
    port = "1337";
    rio_t rio;
    //cmd_t command;
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
    Rio_readlineb(&rio, buf, MAXLINE);
    Fputs(buf, stdout);
    while(Fgets(buf, MAXLINE, stdin) != NULL) {
        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);

        int toread = atoi(buf);
        linestoread(toread, &rio);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
    return 0;
}
void linestoread(int x, rio_t *rio){
    char buf[MAXLINE];
    for(int i = 0; i < x; i++){
        Rio_readlineb(rio, buf, MAXLINE);
        Fputs(buf, stdout);
    } 

}
/* $end echoclientmain */
