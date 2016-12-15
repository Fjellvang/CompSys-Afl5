#define MAXCONNECTIONS (20)
#include "csapp.h"
#ifndef MAXLINE
#define	MAXLINE	 8192  /* Max text line length */
#endif

typedef struct{
    char client_hostname[MAXLINE];
    char client_port[MAXLINE];
    int active;
//TODO Add field for clients File Descriptor?
} connected_t;

// initialize connection list. Should be run when a server starts
void init_connect_list();

// find a free spot to save a peers hostname and port. 
// return 1 on success, -1 on error
int add_to_connect_list(char *client_hostname, char *client_port);

//prints the list of peers whose active field is set to 1
void print_connected_list();
