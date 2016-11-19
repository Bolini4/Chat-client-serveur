#ifndef FONCTION_H_
#define FONCTION_H_
#include <netinet/in.h>

// Function that prints the error for read/write and other action using file descriptors
void error(const char *msg);
// Handle function connect using port and IP
void do_connect(int sock_client, char hostname[], int port, struct sockaddr_in* sock_host);
// Init address from a port
struct sockaddr_in init_serv_addr(int port);


// return the sub_string between start and end
char *str_sub (const char *s, unsigned int start, unsigned int end);
//Get the first arg from string buffer
char* get_1_arg(char* buffer);
//Get the first arg from string buffer
char* get_2_arg(char* buffer);
//Get the first arg from string buffer
char* get_3_arg(char* buffer);

//Put the msg after "/msgall " in string msg
void get_msgall(const char* buffer, char* msg);

// Put user and msg after "/msg " in string receiving_user and msg_to_send
// Return 1 if the msg isn't in the expected format, 0 otherwise
int extract_unicast(char* buffer, char* receiving_user, char* msg_to_send);


//Return the current date in date_in
void getdate(char* date_in);

void free_ressource();

#endif /* FONCTION_H_ */
