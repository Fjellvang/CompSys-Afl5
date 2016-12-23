#define MAXCONNECTIONS (20)
#include "csapp.h"
#ifndef MAXLINE
#define	MAXLINE	 8192  /* Max text line length */
#endif

typedef struct{
    char client_hostname[MAXLINE];
    char client_port[MAXLINE];
    char nick[MAXLINE];
    int fd;
    int active;
} connected_t;

// initialize connection list. Should be run when a server starts
void init_connect_list();

// find a free spot to save a peers hostname and port. 
// return 1 on success, -1 on error
int add_to_connect_list(char *nick, char *client_hostname, char *client_port, int fd);

//prints the list of peers whose active field is set to 1
//Used for debugging
void print_connected_list();

//returns nr of peers conneccted
int npeersconnected();

// Adds user info for all logged in users to string. Returns N lines for client to read
int prints_connected_list(char* tosend);

//remove a client from list, identified by its assigned FD
int remove_from_connect_list(int fd); 

// Adds user infor for one user to string. Returns N lines the clients should read.
int print_user_info(char *nick, char *tosend); 
