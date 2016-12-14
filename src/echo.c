/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
int isStringCommand(char *c);

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

int isStringCommand(char *c){
    char s[2] = " ";
    char *token;
    // array of 4 strings. max 4 cmds
    char strings[4][MAXLINE];
    token = strtok(c, s);
    int tkcnt = 0;    
    while ( token != NULL) {
        if((tkcnt < 4)){
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
    if(!strcmp(strings[0], "/join")){ /* We use !strcmp because strcmp returns 0 on equal string  */
        printf("We got JOIN!\n");
        return 0;
    } else if(!strcmp(strings[0], "/lookup")){
        printf("We got LOOKUP\n");
        return 1;
    } else if(!strcmp(strings[0], "/logout")) {
        printf("we goot logout!");
        return 2;
    } else if(!strcmp(strings[0], "/exit")) {
        printf("we got exit");
        return 3;
    } else {
        printf("unrecognized");
        return 4;
    }
}
