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
sem_t mutex;

int checkLogin(char *username, char *password);

int main(int argc, char **argv) 
{
    //add a user
    strcpy(registered_users[0].usrname, "user");
    strcpy(registered_users[0].pswrd, "password");
    strcpy(registered_users[1].usrname, "jesus");
    strcpy(registered_users[1].pswrd, "christ");
    strcpy(registered_users[2].usrname, "saddam");
    strcpy(registered_users[2].pswrd, "hussein");
    //init list for connections
    init_connect_list();

    //Init semamphore 
    Sem_init(&mutex, 0, 1);
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    pthread_t tid; //[MAXCONNECTIONS]; 

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    for (;;) {
    	clientlen = sizeof(struct sockaddr_storage); 
        connfdp = Malloc(sizeof(int));
    	*connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        Pthread_create(&tid, NULL, thread, (void *)connfdp);
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
                    // currently case sensitive. Should cast username to Lower 
                    if(checkLogin(command.strings[1], command.strings[2])) {
                        loggedin=1;
                        // lock so they wont be assigned same spot in array
                        P(&mutex);
                        add_to_connect_list(command.strings[1],
                                            command.strings[3], 
                                            command.strings[4], fd);
                        V(&mutex);
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
                                char msg[MAXLINE];
                                int toPrint = print_user_info(command.strings[1], msg);
                                char send[20];
                                sprintf(send, "%i\n", toPrint);
                                strcat(send, msg);
                                Rio_writen(fd, send, strlen(send));
                            } else if(cmd_i == logout_command) {
                                loggedin = 0;
                                remove_from_connect_list(fd);
                                char *msg = "1\nYou logged out\n";
                                Rio_writen(fd, msg, strlen(msg));
                            } else if(cmd_i == exit_command) {
                                loggedin = 0;
                                exited = 1;
                                remove_from_connect_list(fd);
                                char *msg = "1\nYou logged out, and exited\n";
                                Rio_writen(fd, msg, strlen(msg));
                            } else if(cmd_i == lookup2_command) {
                                char msg[MAXLINE];
                                int toPrint = prints_connected_list(msg);
                                //amount of lines client should read
                                char send[20];
                                sprintf(send, "%i\n", toPrint);
                                // concat everything to a whole string
                                strcat(send, msg);
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
