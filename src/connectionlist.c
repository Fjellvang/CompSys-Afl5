#include "connectionlist.h"

static connected_t connected_list[MAXCONNECTIONS];


//TODO save the assigned FD to the connected_t struct, and save it in add_to_connect_list maybe
//TODO add function to remove peer from list. Maybe identified by his assigned FD?

void init_connect_list(){
    for(int i = 0; i < MAXCONNECTIONS; i++){
        connected_list[i].active = -1;
    }
}

int add_to_connect_list(char *client_hostname, char *client_port){
    for(int i = 0; i < MAXCONNECTIONS; i++){
        if(connected_list[i].active == -1){
            connected_list[i].active = 1;
            strcpy(connected_list[i].client_port, client_port);
            strcpy(connected_list[i].client_hostname, client_hostname);
            return 1;
        }
    }
    // not enough space
    printf("NO MOAR SPACE MAN \n");
    return -1;
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
            printf("Connected to (%s, %s)\n",
                 connected_list[i].client_hostname, connected_list[i].client_port);
        }
    }
}
