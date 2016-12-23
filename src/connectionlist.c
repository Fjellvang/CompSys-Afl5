#include "connectionlist.h"


static connected_t connected_list[MAXCONNECTIONS];



void init_connect_list(){
    for(int i = 0; i < MAXCONNECTIONS; i++){
        connected_list[i].active = -1;
    }
}

int add_to_connect_list(char *nick, char *client_hostname, char *client_port, int fd){
    for(int i = 0; i < MAXCONNECTIONS; i++){
        if(connected_list[i].active == -1){
            connected_list[i].active = 1;
            strcpy(connected_list[i].client_port, client_port);
            strcpy(connected_list[i].client_hostname, client_hostname);
            strcpy(connected_list[i].nick, nick);
            connected_list[i].fd = fd;
            return 1;
        }
    }
    // not enough space
    printf("NO MOAR SPACE MAN \n");
    return -1;
}

int remove_from_connect_list(int fd) {
    for(int i = 0; i < MAXCONNECTIONS; i++) {
        if (connected_list[i].fd == fd) {
            connected_list[i].active = -1;            
            return 1;
        }
    }
    return 0;
}

int npeersconnected(){
    //returns nr of peers connected
    int n=0;
    for(int i =0; i < MAXCONNECTIONS; i++){
        if(connected_list[i].active == 1) {
            n++;
        }
    }
    return n;
}

void print_connected_list(){
    for(int i = 0; i < MAXCONNECTIONS; i++){
        if(connected_list[i].active == 1){
            printf("connected to (%s, %s)\n",
                 connected_list[i].client_hostname, connected_list[i].client_port);
        }
    }
}
int print_user_info(char *nick, char *tosend) {
    int nrOfnewline = 1;
    strtok(nick, "\n");
    for(int i = 0; i < MAXCONNECTIONS; i++) {
        if(!strcmp(connected_list[i].nick, nick)) {
            sprintf(tosend, "%s is online\nHost: %s\nPort: %s\n", nick, 
                connected_list[i].client_hostname,
                connected_list[i].client_port);
            nrOfnewline = nrOfnewline + 3;
            return nrOfnewline;
        }
    }
    sprintf(tosend, "%s is not a valid user\n", nick);
    //nrOfnewline = nrOfnewline + 2;
    return nrOfnewline;
}

int prints_connected_list(char* tosend){
    int nrOfnewline = 0;    
    char tmp[MAXLINE];
    int npeers = npeersconnected();
    sprintf(tosend, "%i peers connected, The list follows\n", npeers);
    nrOfnewline = nrOfnewline + 1;
    for(int i = 0; i < MAXCONNECTIONS; i++){
        if(connected_list[i].active == 1){
            sprintf(tmp, "Nick: %s\nHost: %s\nPort: %s\n",connected_list[i].nick, connected_list[i].client_hostname, connected_list[i].client_port);
            nrOfnewline = nrOfnewline + 3;
            strcat(tosend, tmp);
        }
    }
    // add nr of peers to make sure we get the right amount of reads from client
    nrOfnewline += npeers;
    return nrOfnewline;
}
