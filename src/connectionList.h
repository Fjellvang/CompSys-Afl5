typedef struct{
    char client_hostname[MAXLINE];
    char client_port[MAXLINE];
    int active;
} connected_t;

void init_connect_list();

int add_to_connect_list(char *client_hostname, char *client_port);

void print_connected_list();
