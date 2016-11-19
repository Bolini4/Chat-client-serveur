#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

#include "user.h"
#include "salon.h"

struct salon* init_salon(int max){
    struct salon* tab_salon = malloc(max* sizeof(struct salon*)) ;
    if(tab_salon == NULL){
        printf("erreur allocation salon");
    }
    memset(tab_salon,'\0', sizeof(*tab_salon));
    return tab_salon;
}

void set_salon(struct salon* salon, char* name){
    salon->name = name;
    salon->current_client_nb = 0;
}

void delete_salon(struct salon* salon){
    memset(salon, '\0', sizeof(struct salon*));
}

void add_client_nb(struct salon* salon){
    salon->current_client_nb++;
}

char* get_salon_name(struct salon* salon){
    return salon->name;
}

int salon_existing(struct salon* salon){
    if (get_salon_name(salon) == NULL){
        return 0;
    }else{
        return 1;
    }

}

void create_salon(char* name, struct user* user, struct salon* tab_salon){
    int salon_create_indicator =0;
    int i;
    char msg_create[SIZE_MSG] = {0};

    // le salon existe?
    for(i=0; i< MAX_CLIENT ;i++){// i parcour salon
        if(salon_existing(tab_salon+i)){
            if(strcmp(name, get_salon_name(tab_salon+i))==0){
                strcat(msg_create,"Le salon existe déja, veuillez changer le nom de votre nouveau salon.");
                if(write(get_id(user)+4,msg_create,strlen(msg_create)) <0){
                    printf("erreur salon err_msg");
                }
                salon_create_indicator=1;
            }
        }
    }


    if (salon_create_indicator == 0){
        for(i=0; i< MAX_CLIENT ;i++){
            if(!salon_existing(tab_salon+i)){
                set_salon(tab_salon+i, name);
                printf("Salon créé\n");
                strcat(msg_create,"You have created channel ");
                strcat(msg_create,name);
                strcat(msg_create," \n");
                if(write(get_id(user)+4,msg_create,strlen(msg_create)) <0){
                    printf("erreur salon err_msg");
                }
                break;
            }
        }
    }
}


void join_salon(struct user* user, struct salon* tab_salon, char* name_salon){
    char send[SIZE_MSG] = {0};
    int i;
    int flag_existing_salon=0;

    // Inform the user if he already is in a channel
    if(get_user_salon(user) != NULL){
        if(strcmp(get_user_salon(user), name_salon)==0 ){

            sprintf(send, "Vous etes deja dans le salon");
            if(write(get_id(user)+4,send,strlen(send)) <0){
                printf("erreur salon join");
            }
        }
        else{
            sprintf(send, "Vous ne pouvez pas rejoindre un autre salon");
            if(write(get_id(user)+4,send,strlen(send)) <0){
                printf("erreur salon join");
            }
        }
    }

    else{
        for(i=0; i< MAX_CLIENT ;i++){ // parcours salon
            if(salon_existing(tab_salon+i)){
                if(strcmp(name_salon, get_salon_name(tab_salon+i))==0){ // On se trouve dans le bon salon
                    set_salon_user(user, name_salon);
                    add_client_nb(tab_salon+i);

                    sprintf(send, "Vous avez rejoint le salon");
                    if(write(get_id(user)+4,send,strlen(send)) <0){
                        printf("erreur salon join");
                    }
                    flag_existing_salon = 1;
                    break;

                }
            }

        }

        // If there is no channel "name_salon"
        if(flag_existing_salon == 0){
            sprintf(send, "le salon n'existe pas");
            if(write(get_id(user)+4,send,strlen(send)) <0){
                printf("erreur salon join");
            }
        }

    }
}

void quit_salon(struct salon* tab_salon, struct user* user, char* name_salon){
    char send[SIZE_MSG]={0};
    int i;
    if(get_user_salon(user) != NULL){
        for(i=0; i< MAX_CLIENT ;i++){ // Check channels
            if(salon_existing(tab_salon+i)){
                if(strcmp(name_salon, get_salon_name(tab_salon+i))==0){ // On se trouve dans le bon channel

                    (tab_salon+i)->current_client_nb--;
                    set_salon_user(user, NULL);
                    sprintf(send, "Vous  avez quitté le salon");

                    if ((tab_salon+i)->current_client_nb == 0){
                        delete_salon(tab_salon+i);
                    }
                    if(write(get_id(user)+4,send,strlen(send)) <0){
                        printf("erreur salon quit");
                    }
                }
            }
        }
    }
    // Inform the user if he tried to quit salon but wasn't in any channel
    else{
        sprintf(send, "Vous n'etes dans aucun salon");
        if(write(get_id(user)+4,send,strlen(send)) <0){
            printf("erreur salon quit");
        }
    }
}

void write_in_salon(struct user* tab_user, char* msg, struct user* sending_user){
    int i;
    char msg_write[SIZE_MSG]={0};
    int flag_other_user=0;
    for(i=0; i< MAX_CLIENT ;i++){
        if(user_existing(tab_user+i) && (get_id(sending_user) != i) && (get_user_salon(tab_user+i) != NULL)){
            // Look for the users that are in the same channel as sending_user
            if(strcmp(get_user_salon(tab_user+i),get_user_salon(sending_user)) == 0){

                strcat( msg_write,"[");
                strcat( msg_write,get_nickname(sending_user));
                strcat( msg_write,"](salon) : ");
                strcat( msg_write,msg);

                if(write(i+4,msg_write, strlen(msg_write) ) <0){
                    printf("erreur sendall salon");
                }
                flag_other_user=1;
                memset(msg_write, '\0', sizeof(msg_write));
            }
        }
    }
    // If there is no other users in the salon, inform the sending user
    if(flag_other_user == 0){
        sprintf(msg_write, "Il n'y a pas d'autres utilisateurs dans le salon.");

        if(write(get_id(sending_user)+4,msg_write, strlen(msg_write) ) <0){
            printf("erreur sendall salon");
        }
    }
}
