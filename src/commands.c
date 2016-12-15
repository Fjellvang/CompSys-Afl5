#include "commands.h"
#include "csapp.h"

// array to hold commands and their argument
// max arguments for a command is currently 4
char strings[5][MAXLINE];

void init_strings(){
    for(int i = 0; i < 5; i++) {
        strcpy(strings[i], "0000");
    }
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
