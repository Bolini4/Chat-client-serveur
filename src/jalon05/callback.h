#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "user.h"
#include "salon.h"

void handle_msg_from_client(char* buffer, struct user* tab_user, struct salon* tab_salon, int i, int* nb_connexion);

#endif /* CALLBACK_H_ */
