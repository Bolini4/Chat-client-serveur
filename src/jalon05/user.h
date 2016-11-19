#ifndef USER_H_
#define USER_H_


#define SIZE_MSG 200
#define MAX_CLIENT 20



struct user {
  char* nickname;
  char* date_conexion;
  char* addr_ip;
  int port;
  int id;
  char* salon;
};

// Allocate memory for a tab_user based on max users
struct user* init_tab_user(int max);
// Set a user accordingly to given info
void set_user(struct user* user,  char* nickname, char* date_conexion, char* addr_ip, int port,int id);
// Delete user's info
void delete_user(struct user* user);


// Return user nickname
char* get_nickname(struct user* user);
// Change field nickname in user's info based on string nickname
void change_nickname(struct user* user,  char* nickname);

// Return user id
int get_id(struct user* user);

// Return user salon
char* get_user_salon(struct user* user);
// Change field salon in user's info based on name_salon
void set_salon_user(struct user* user, char* name_salon);

// Return user port in string port
void get_port(struct user* user,char* port);

// Return user IP_address
char* get_address(struct user* user);


// Return 1 if user exist, 0 otherwise
int user_existing(struct user* user);
// Return 1 if user is in a salon, 0 otherwise
int user_in_salon(struct user* user);

// Return a string with all the users stored in tab_user
char* who(struct user* user);
// set "string_whois" to a string describing the user with "nickname"
void whois_user(struct user* user, char* nickname, char* string_whois);

// Write to all users found in tab_user the string msgall
void send_all(struct user* tab_user, char* msgall, int sending_user);
// Write the unicast msg to the right user, directly taking the buffer
void unicast(struct user* tab_user, char* buffer, int sending_user);

#endif /* USER_H_ */
