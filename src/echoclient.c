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
    host = "127.0.0.1"; 
    port = "1337";
    rio_t rio;
    c.loggedin = 0;
    cmd_t command;
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    Rio_readlineb(&rio, buf, MAXLINE);
    Fputs(buf, stdout);
    while(Fgets(buf, MAXLINE, stdin) != NULL) {
        char tmp[MAXLINE];
        strcpy(tmp, buf);

        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
        /*if(Fgets(buf, MAXLINE, stdin) != NULL) {
            if(buf[0] == '/') {
                command = handlecmd(buf);
                printf("SUCCESS\n");
                c.loggedin = 1;
                printf("cmd1:%s\n", command.strings[1]);
                
                //send username
                Rio_writen(clientfd, buf, strlen(command.strings[1]));
                printf("%s written to server\n", buf);
                //send password
            }
        }*/
        /*while(c.loggedin == 1){
            if(Fgets(buf, MAXLINE, stdin) != NULL){
                if(buf[0] == '/') {
                    // smid string i tmp fil
                    handlecmd(buf);
                }
                Rio_writen(clientfd, buf, strlen(buf));
                Rio_readlineb(&rio, buf, MAXLINE);
                Fputs(buf, stdout);
            }
        }*/
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
    return 0;
}
/* $end echoclientmain */
