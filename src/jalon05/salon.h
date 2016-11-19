#ifndef SALON_H_
#define SALON_H_
#include "user.h"



struct salon{
    char* name;
    int current_client_nb;
};

// Allocate memory for the max number of salon 
struct salon* init_salon(int max);


// Set a salon accordingly to the given info
void set_salon(struct salon* salon, char* name);
// Delete salon informations
void delete_salon(struct salon* salon);

// Increment the "current_client_nb" field of the salon
void add_client_nb(struct salon* salon);
// Return the name of the salon
char* get_salon_name(struct salon* salon);
// Check if the salon exists, return 1 if it exists, 0 otherwise
int salon_existing(struct salon* salon);


//Create a new salon with given name and store it in "tab_salon"
void create_salon(char* name, struct user* first_user, struct salon* tab_salon);

// Join a salon if the user doesn't have one already and inform the user of the actions
void join_salon(struct user* user, struct salon* tab_salon, char* name_salon);

// Handle the case when the user qui the salon, taking into acount the number of users remaining in the salon 
// and delete the salon if it was the last user. Also checks if the user is not in a salon and send a message if wether or not the user left a salon
void quit_salon(struct salon* salon, struct user* user, char* name_salon);

// Write the given "msg" to all users in the salon except the user sending the message 
void write_in_salon(struct user* tab_user, char* msg, struct user* sending_user );



#endif /* SALON_H_ */
