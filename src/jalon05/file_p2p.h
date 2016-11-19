#ifndef FILE_P2P_H_
#define FILE_P2P_H


// handle the file reception from the socket receive
void receive_file(char* input, char* buffer, char* nick,  int sock_recv );

// handle the connexion in p2p and the file sending
void file_p2p_client(char* input, char* buffer, char* nick );

// Extract the filename and receiving user from string buffer ( /send <user> <filename> )
int extract_file(char* buffer, char* receiving_user, char* filename, int position);

// Get the adress and port by looking in the tab_user for the name of the user
void extract_addr_p2p(struct user* tab_user, char* receiving_user,char* dest_addr,char* dest_port);



#endif /* FILE_P2P_H_ */
