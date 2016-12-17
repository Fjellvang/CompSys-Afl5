/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#include "connectionlist.h"
#define MAXUSERS (256)

void echo(int connfd);
void *thread(void *vargp);

typedef struct {
    char usrname[MAXLINE];
    char pswrd[MAXLINE];
} user_t;

user_t registered_users[MAXUSERS];
int login(int connfd);
int checkLogin(char *username, char *password);

int main(int argc, char **argv) 
{
    //add a user
    strcpy(registered_users[0].usrname, "user");
    strcpy(registered_users[0].pswrd, "password");
    //init list for connections
    init_connect_list();
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    //thread table for 20 connections
    pthread_t tid; //[MAXCONNECTIONS]; 
    //int tindex = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    for (;;) {
    	clientlen = sizeof(struct sockaddr_storage); 
        connfdp = Malloc(sizeof(int));
    	*connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        //if(login(*connfdp)) {
            // læs fra fd, se om login / pass passer
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        add_to_connect_list(client_hostname, client_port);	
        print_connected_list();
        Pthread_create(&tid/*&tid[tindex]*/, NULL, thread, (void *)connfdp);
        /*} else {
            char* msg = "Connection refused";
            int fd = *((int*)connfdp);
            Rio_writen(fd, msg, sizeof(msg));
        }*/
    }
    exit(0);
}

void *thread(void *vargp){
    
    int fd = *((int*)vargp);
    // detach thread så den kan reapes af kernen når tråden exitter
    Pthread_detach(Pthread_self());
    Free(vargp);
    printf("FD is: %i\n", fd);
    int loggedin = 0;
    char* msg = "You are currently not logged in. Please login or exit\n";
    Rio_writen(fd, msg, strlen(msg));
    while(loggedin == 0) {
        if(login(fd)){
            loggedin=1;
            char *msg = "login success\n";
            Rio_writen(fd, msg, strlen(msg)); 
        }else {
            char *msg = "login refused\n";
            Rio_writen(fd, msg, strlen(msg)); 
        }
    }
    echo(fd);
    // should remove peer from connected list here.
    Close(fd);
    return NULL;
}

int login(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    cmd_t command;
    rio_t rio;
    Rio_readinitb(&rio, connfd);
    if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
        if(getCmd(buf, &command) == join_command) {
            // we have login... once again
            if(checkLogin(command.strings[1], command.strings[2])){
                printf("Login success!\n");
                return 1;
            }
            else{ 
                printf("Login refused");
                return 0;
            }
        }
    }
    return 0;
}

int checkLogin(char *username, char *password){
    for(int i = 0; i < MAXUSERS; i++) {
        if(!strcmp(username, registered_users[i].usrname) &&
           !strcmp(password, registered_users[i].pswrd)){
            //We have a match! 
            return 1;
        }
    }
    return 0;
}
/* $end echoserverimain */
