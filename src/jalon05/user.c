#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/select.h>
#include <netinet/in.h>

#include "user.h"
#include "fonction.h"

struct user* init_tab_user(int max){
    struct user* tab_user = malloc(max* sizeof(struct user*)) ;
    if(tab_user == NULL){
        printf("erreur allocation user");
    }
    memset(tab_user,'\0', sizeof(*tab_user));
    return tab_user;
}

void set_user(struct user* user,  char* nickname, char* date_conexion, char* addr_ip, int port,int id){
    user->nickname = nickname;
    user->date_conexion = date_conexion;
    user->addr_ip = addr_ip;
    user->port = port;
    user->id=id;
    user->salon = NULL;
}

void delete_user(struct user* user){
    memset(user, '\0', sizeof(struct user*));
}

char* get_nickname(struct user* user){
    return user->nickname;
}

void change_nickname(struct user* user,  char* nickname){
    user->nickname = nickname;
}

int get_id(struct user* user){
    return user->id;
}

char* get_user_salon(struct user* user){
    return user->salon;
}

void set_salon_user(struct user* user, char* name_salon){
    user->salon = name_salon;
}

char* get_address(struct user* user){
    return user->addr_ip;
}

void get_port(struct user* user,char* port){
    sprintf(port,"%d", user->port);
}


int user_existing(struct user* user){
    if (get_nickname(user) == NULL){
        return 0;
    }else{
        return 1;
    }
}

int user_in_salon(struct user* user){
    if (get_user_salon(user) == NULL){
        return 0;
    }else{
        return 1;
    }
}



char* who(struct user* user){
    int i;
    char* str_out = calloc(SIZE_MSG,sizeof(char));

    if (str_out != NULL){
        for (i =0; i<MAX_CLIENT; i++){
            if (get_nickname(user+i) != NULL){
                strcat(str_out, get_nickname(user+i));
                strcat(str_out, "\n");
            }
        }
    }
    return str_out;
}



void whois_user(struct user* user, char* nickname, char* string_whois){

    char port[10];
    sprintf(port,"%d", user->port);

    strcat(string_whois,nickname);
    strcat(string_whois," connected since ");

    strcat(string_whois,user->date_conexion);
    strcat(string_whois," with IP address ");

    strcat(string_whois,user->addr_ip);
    strcat(string_whois," and port number ");

    strcat(string_whois,port);
}


void send_all(struct user* tab_user, char* msgall, int sending_user){
    int i;
    char msgall_write[SIZE_MSG] = {0};
    for(i=0; i< MAX_CLIENT ;i++){
        if(user_existing(tab_user+i) && (sending_user != i)){

            strcat( msgall_write,"[");
            strcat( msgall_write,get_nickname(tab_user+sending_user));
            strcat( msgall_write,"](msgall) : ");
            strcat( msgall_write,msgall);

            if(write(i+4,msgall_write, strlen(msgall_write) ) <0){
                printf("erreur sendall");
            }
            memset(msgall_write, '\0', sizeof(msgall_write));
        }
    }
}



void unicast(struct user* tab_user, char* buffer, int sending_user){
    int i;
    int flag_err;
    char unicast_write[SIZE_MSG] = {0};
    char msg_unicast[SIZE_MSG] = {0};
    char receiving_user[SIZE_MSG] = {0};
    char* user_outofsalon ="L'utilisateur désigné ne se trouve pas dans le salon";


    flag_err=extract_unicast(buffer, receiving_user, msg_unicast);

    if (flag_err){
        if(write(sending_user+4,user_outofsalon ,strlen(user_outofsalon) ) <0){
            printf("erreur uniscast");
        }
    }


    for(i=0; i< MAX_CLIENT ;i++){
        if(user_existing(tab_user+i)){
            //check if the users are in a salon
            if(user_in_salon(tab_user+i) && user_in_salon(tab_user+sending_user)){
                if(strcmp(receiving_user, get_nickname(tab_user+i))==0){

                    // on cherche tous les utilisateurs qui sont dans le salon de sending_user
                    if(strcmp(get_user_salon(tab_user+i),get_user_salon(tab_user+sending_user)) == 0){
                        strcat( unicast_write,"[");
                        strcat( unicast_write,get_nickname(tab_user+sending_user));
                        strcat( unicast_write,"](Unicast) : ");
                        strcat( unicast_write,msg_unicast);
                        if(write(i+4,unicast_write, sizeof(unicast_write) ) <0){
                            printf("erreur uniscast");
                        }
                    }
                    else{
                        if(write(i+4,user_outofsalon ,sizeof(user_outofsalon) ) <0){
                            printf("erreur uniscast");
                        }
                    }
                }
            }
            else if (!(user_in_salon(tab_user+i)) && !(user_in_salon(tab_user+sending_user))){
                if(strcmp(receiving_user, get_nickname(tab_user+i))==0){
                    strcat( unicast_write,"[");
                    strcat( unicast_write,get_nickname(tab_user+sending_user));
                    strcat( unicast_write,"](Unicast) : ");
                    strcat( unicast_write,msg_unicast);
                    if(write(i+4,unicast_write, sizeof(unicast_write) ) <0){
                        printf("erreur uniscast");
                    }
                }

            }
        }
    }
}
