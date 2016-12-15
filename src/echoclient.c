/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
#define NROFCOMMANDS (4)

int getCmd(char *c);
int handlecmd(char *c);
const char* getcmdstring(int i);
int getcmdint(char *s);

typedef struct {
    int loggedin;
    char my_hostname[MAXLINE];
    char my_port[MAXLINE];  
} client;

char strings[5][MAXLINE];
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
                     printf("SUCCESS");
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
}

int getCmd(char *c){
    char s[2] = " ";
    char *token;
    // array of 4 strings. max 4 cmds
    token = strtok(c, s);
    int tkcnt = 0;    
    while ( token != NULL) {
        if((tkcnt < 5)){
            strcpy(strings[tkcnt], token);
        } else {
            printf("incorrect command\n");
            return -1;
        } 
        token = strtok(NULL, s);
        tkcnt++;
    }
    
    for(int i=0; i < tkcnt; i++){
        printf("index: %i string: %s\n", i, strings[i]);
    }
    return getcmdint(strings[0]);
}
int getcmdint(char *s){
    for(int i = 0; i < NROFCOMMANDS; i++){
        if(!strcmp(s, getcmdstring(i))){
            // DE matcher.
            return i;
        }
    }
    return -1;
}

const char* getcmdstring(int i){
    switch(i){
        case 0:
            return "/join";
            break;
        case 1:
            return "/lookup";
            break;
        case 2:
            return "/logout";
            break;
        case 3:
            return "/exit";
            break;
        default:
            return NULL;
            break;
    }
}

int handlecmd(char *c){
    char tmp[MAXLINE];
    strcpy(tmp, c);
    int cmd = getCmd(tmp);
    if(cmd == 0) return 1;
    else return 0;
}
/* $end echoclientmain */
