// Increase this every time you add a command
#define NROFCOMMANDS (4)
#ifndef MAXLINE
#define MAXLINE 8192
#endif

typedef struct {
    char strings[5][MAXLINE];
    int numargs;
    int cmd; //which command did we get?
} cmd_t;

enum cmds {
    join_command,
    lookup_command,
    logout_command,
    exit_command
};
// if we have / as first char, we have a command. call this func on a string with a cmd
cmd_t handlecmd(char *c);

// If we have a string with a cmd. this function chunks the string up, and puts them
// in the strings array
int getCmd(char *c, cmd_t *command);

//Takes a string like "/cmd" if it is a defined command, return index of cmd. 
//if not defined return NULL
int getcmdint(char *s);
// returns a string given a int. add new commands here
const char* getcmdstring(int i);

