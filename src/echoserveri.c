/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#include "connectionlist.h"

void echo(int connfd);
void *thread(void *vargp);


int main(int argc, char **argv) 
{
    //init list for connections
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

        // læs fra fd, se om login / pass passer
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        add_to_connect_list(client_hostname, client_port);	
        //printf("Connected to (%s, %s)\n", client_hostname, client_port);
        print_connected_list();
        Pthread_create(&tid/*&tid[tindex]*/, NULL, thread, (void *)connfdp);
        //tindex++;
        /*
        echo(connfd);
        printf("Echo returned\n");
        Close(connfd);*/
    }
    exit(0);
}

void *thread(void *vargp){
    
    int fd = *((int*)vargp);
    // detach thread så den kan reapes af kernen når tråden exitter
    Pthread_detach(Pthread_self());
    Free(vargp);
    printf("FD is: %i\n", fd);
    echo(fd);
    // should remove peer from connected list here.
    Close(fd);
    return NULL;
}
/* $end echoserverimain */
