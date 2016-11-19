#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/select.h>
#include <time.h>
#include <arpa/inet.h>

#include "user.h"
#include "salon.h"
#include "callback.h"
#include "fonction.h"
#include "file_p2p.h"


void handle_msg_from_client(char* buffer, struct user* tab_user, struct salon* tab_salon, int i, int* nb_connexion){

    char msg_usage[SIZE_MSG] ={0};
    sprintf(msg_usage,"Usage :\n /msg <user>\n /msgall\n /create <salon>\n /join <salon>\n /who\n /whois <user>\n /send <user> <filename> \n /nick <new_username>\n /quit <salon> \n /quit\n");

    char* all_user = malloc(SIZE_MSG);
    char* pseudo = malloc(SIZE_MSG);
    char* string_whois = malloc(SIZE_MSG);
    int whois_indicator =0;
    char* msg_all = malloc(SIZE_MSG);
    char* receiving_user = malloc(SIZE_MSG);
    char* filename = malloc(SIZE_MSG);
    char * dest_addr=malloc(SIZE_MSG);
    char * dest_port=malloc(SIZE_MSG);
    char* p2p_msg = malloc(SIZE_MSG);
    char* port_client=malloc(SIZE_MSG);
    int j;

    if (strcmp(buffer, "/who\n") == 0){
        all_user = who(tab_user);
        if(write(i,all_user,strlen(all_user)) < 0){
            error("erreur write");
        }
        memset(buffer, '\0', sizeof(buffer));

    }

    // --------------------- /whois ----------------------
    else if (strcmp(str_sub(buffer,0,6), "/whois ") == 0){

        pseudo=get_2_arg(buffer);

        for(j=0;j<*nb_connexion;j++){

            // Search the fitting user in the table
            if (strcmp(pseudo,get_nickname(tab_user +j))==0){
                //desc_user(tab_user+j);


                whois_user(tab_user +j, pseudo, string_whois); //Fill the string with the right informations
                if(write(i,string_whois,200) < 0){
                    error("erreur write");
                }
                memset(string_whois, '\0', sizeof(string_whois));
                whois_indicator = 1;
                break;
            }
        }
        if (whois_indicator == 0){
            if(write(i,"User not found in tab",50) < 0){
                error("erreur write");
            }
        }
        if (whois_indicator == 1){
            whois_indicator=0;
        }


    }


    // /msgall
    else if (strcmp(str_sub(buffer,0,7), "/msgall ") == 0){

        get_msgall(buffer,msg_all);
        send_all(tab_user, msg_all, i-4);

        memset(msg_all, '\0', sizeof(msg_all));
        memset(buffer, '\0', sizeof(buffer));

    }
    // /unicast
    else if (strcmp(str_sub(buffer,0,4), "/msg ") == 0){

        unicast(tab_user,buffer, i-4);
        memset(buffer, '\0', sizeof(buffer));

    }

    // /create (salon)
    else if (strcmp(str_sub(buffer,0,7), "/create ") == 0){

        create_salon(get_2_arg(buffer), tab_user+i-4, tab_salon );
        memset(buffer, '\0', sizeof(buffer));

    }
    // /join (salon)
    else if (strcmp(str_sub(buffer,0,5), "/join ") == 0){

        join_salon(tab_user+i-4, tab_salon,get_2_arg(buffer));
        memset(buffer, '\0', sizeof(buffer));
    }

    // /quit (salon)
    else if(strcmp(str_sub(buffer,0,5), "/quit ") == 0){

        quit_salon(tab_salon, tab_user+i-4, get_2_arg(buffer));
        memset(buffer, '\0', sizeof(buffer));

    }
    // /send (user filename)
    else if(strcmp(str_sub(buffer,0,5), "/send ") == 0){
        extract_file(buffer, receiving_user, filename, 6);
        extract_addr_p2p(tab_user,receiving_user,dest_addr,dest_port);

        memset(p2p_msg, '\0', sizeof(p2p_msg));
        strcat(p2p_msg,"/connect ");
        strcat(p2p_msg,dest_addr);
        strcat(p2p_msg," ");
        strcat(p2p_msg,dest_port);


        if(write(i,p2p_msg,strlen(p2p_msg)) < 0){
            error("erreur write");
        }

    }


    //write back to the client
    else{
        if(get_user_salon(tab_user+i-4) != NULL){

            write_in_salon(tab_user,buffer,tab_user+i-4);

        }
        else{
            if(write(i,msg_usage,SIZE_MSG) < 0){
                error("erreur write");
            }
        }
    }

    free(all_user);
    free(pseudo);
    free(string_whois);
    free(msg_all);
    free(receiving_user);
    free(filename);
    free(dest_addr);
    free(dest_port);
    free(p2p_msg);
    free(port_client);

}
