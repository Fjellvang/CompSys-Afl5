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
int login(char *c);
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
    int exited = 0;
    int loggedin = 0;
    rio_t rio;
    rio_readinitb(&rio, fd);
    size_t n; 
    char buf[MAXLINE]; 
    char* msg = "You are currently not logged in. Please login or exit\n";
    Rio_writen(fd, msg, strlen(msg));
    while(exited == 0) {
        if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
            if(buf[0] == '/') {
                cmd_t command;
                int cmd_i = getCmd(buf, &command);
                if(cmd_i == join_command){
                    if(checkLogin(command.strings[1], command.strings[2])) {
                        loggedin=1;
                        char *msg = "1\nLogin success\n";
                        Rio_writen(fd, msg, strlen(msg)); 
                    } else {
                        char *msg = "1\nWrong username/password\n";
                        Rio_writen(fd, msg, strlen(msg));
                    }
                } else if(cmd_i == exit_command) {
                    char *msg = "1\nYou exited. BYE\n";
                    Rio_writen(fd, msg, strlen(msg));
                    exited = 1;
                } else {
                    char *msg = "1\nLogin refused\n";
                    Rio_writen(fd, msg, strlen(msg)); 
                }
                while(loggedin == 1){
                    if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
                        if(buf[0] == '/') {
                            // we have a command
                            int cmd_i = getCmd(buf, &command);
                            if(cmd_i == join_command) {
                                char *msg = "1\nYou are already logged in\n";
                                Rio_writen(fd, msg, strlen(msg));
                            } else if(cmd_i == lookup_command) {
                                
                            } else if(cmd_i == logout_command) {
                                loggedin = 0;
                                char *msg = "1\nYou logged out\n";
                                Rio_writen(fd, msg, strlen(msg));
                            } else if(cmd_i == exit_command) {
                                loggedin = 0;
                                exited = 1;
                                char *msg = "1\nYou logged out, and exited\n";
                                Rio_writen(fd, msg, strlen(msg));
                            } else if(cmd_i == lookup2_command) {
                                printf("start of LP2\n");
                                char msg[MAXLINE];
                                int toPrint = prints_connected_list(msg);
                                char send[20];
                                printf("after prints\n");
                                sprintf(send, "%i\n", toPrint);
                                strcat(send, msg);
                                printf("before write");
                                Rio_writen(fd, send, strlen(send));
                            } else {
                                char *msg = "1\nUnrecognized command\n"; 
                                Rio_writen(fd, msg, strlen(msg));
                            }   
                        } else {
                                char *msg = "1\nYou didnt enter a command\n";
                                Rio_writen(fd, msg, strlen(msg));
                        }
                    } 
                }
            } else{
                    char *msg = "1\nPlease login with /join <username> <password> <IP> <port> or exit with /exit\n";
                Rio_writen(fd, msg, strlen(msg));
            }
        }
    }
    // should remove peer from connected list here.
    Close(fd);
    return NULL;
}

int login(char *c) 
{
    cmd_t command;
    int cmd_i = getCmd(c, &command);
    if(cmd_i == join_command) {
        if(checkLogin(command.strings[1], command.strings[2])){
            printf("Login success!\n");
            return 1;
        }
        else{ 
            printf("Login refused");
            return 0;
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
