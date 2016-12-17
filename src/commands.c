#include "csapp.h"

// array to hold commands and their argument
// max arguments for a command is currently 4
char strings[5][MAXLINE];


int getCmd(char *c, cmd_t *command){
    char s[2] = " ";
    char *token;
    // array of 4 strings. max 4 cmds
    token = strtok(c, s);
    command->numargs = 0;    
    while ( token != NULL) {
        if((command->numargs < 5)){
            strcpy(command->strings[command->numargs], token);
        } else {
            printf("Too many arguments\n");
            return -1;
        } 
        token = strtok(NULL, s);
        command->numargs++;
    }
    
    return getcmdint(command->strings[0]);
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

cmd_t handlecmd(char *c){
    char tmp[MAXLINE];
    strcpy(tmp, c);
    cmd_t command;
    int cmd = getCmd(tmp, &command);

    switch(cmd) {
        case join_command:
            if(command.numargs != 5){
                printf("join: Too few arguments");
            } 
            return command;
        case lookup_command:
        case logout_command:
        case exit_command:
            break;
    }
    return command;
}
