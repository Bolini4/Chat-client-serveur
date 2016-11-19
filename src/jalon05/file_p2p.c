#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include "user.h"
#include "fonction.h"

#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

void file_p2p_client(char* input, char* buffer, char* nick ){

    struct sockaddr_in addr_client_p2p;
    char path[SIZE_MSG];
    char dest_adress[SIZE_MSG];
    int port_recv;
    int sock_file;
    int fdfile;
    struct stat buf;
    char sizec[10];
    int size_file_send = -1;
    off_t offset = 0;
    int nb_sent=0;

    //init
    sprintf(dest_adress, "%s", get_2_arg(buffer));
    port_recv = atoi(get_3_arg(buffer));
    sprintf(path,"%s", get_3_arg(input));

    //get the socket
    sock_file = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
    if (sock_file < 0){
        error("erreur socket");
    }

    //connect to remote socket
    do_connect(sock_file, dest_adress, port_recv, &addr_client_p2p);

    //file descriptore on the file wich will be send
    if((fdfile = open(path, O_RDONLY)) <0){
        error("open fdfile");
    }
    fstat(fdfile, &buf);
    size_file_send = (int) buf.st_size;
    sprintf(sizec,"%d", size_file_send);

    //build the message Do you accept? [y/n]
    sprintf(buffer, "%s", nick);
    strcat(buffer, " wants you to accept the transfer of the file named ");
    strcat(buffer, get_3_arg(input));
    strcat(buffer, ". Do you accept? [y/n]\n");
    if (write(sock_file, buffer, strlen(buffer))<0){
        error("write ");
    }

    //read the answer
    memset(buffer, '\0', strlen(buffer));
    if (read(sock_file, buffer, SIZE_MSG) < 0){
        error("read y/n");
    }

    if (strcmp(get_1_arg(buffer), "y") == 0){
        printf("%s a accepter le transfert \n", get_2_arg(buffer));

        //we send the file size
        if (write(sock_file, sizec, SIZE_MSG)<0){
            error("write size");
        }

        //sending the file
        while (((nb_sent = sendfile(sock_file, fdfile, &offset, SIZE_MSG)) > 0) && (size_file_send > 0)) {
            size_file_send -= nb_sent;
        }


    }
    else{
        printf("%s a annulé l'opération \n", get_2_arg(buffer));
    }



    printf("\n------------------------------ \n");
    close(fdfile);
    close(sock_file);

}



void receive_file(char* input, char* buffer, char* nick, int sock_recv ){

    int size_file_rcv;
    int received_file;
    int nb_rcv = 0;

    //accept ? y/n
    if (read(sock_recv, buffer, SIZE_MSG) < 0){
        error("read y/n");
    }
    printf("%s", buffer );

    //answer
    fgets(input, sizeof(input), stdin);
    sprintf(buffer, "%c", input[0]);
    strcat(buffer, " ");
    strcat(buffer, nick);

    if(write(sock_recv, buffer, strlen(buffer))< 0){
        error("write answer");
    }

    //size file
    if(read(sock_recv, buffer, SIZE_MSG) < 0){
        error("erreur read");
    }
    size_file_rcv = atoi(buffer);

    if(input[0] == 'y'){
      //file descriptor for the new file
        if((received_file = open("./../inbox/receive", O_CREAT | O_WRONLY, S_IRWXU ) )< 0){
            error("open receive file");
        }

        // we receive the file and write in the received_file descriptor
        while (((nb_rcv = read(sock_recv, buffer, SIZE_MSG)) > 0) && (size_file_rcv > 0)) {
            if(write(received_file, buffer, nb_rcv) < 0){
                error("write in the file receive");
            }
            size_file_rcv -= nb_rcv;

        }
    }


    close(received_file);
    close(sock_recv);
    printf("\n------------------------------ \n");
}


int extract_file(char* buffer, char* receiving_user, char* filename, int position){
    int i=position;

    while((buffer[i] != ' ') ){
        if (buffer[i] == '\n'){
            return 1;
        }
        receiving_user[i-position]=buffer[i];
        i++;
    }
    receiving_user[i-position]='\0';
    i++;
    while((buffer[i] != '\n')){
        filename[i-position - strlen(receiving_user)-1]=buffer[i];
        i++;
    }
    filename[i-position - strlen(receiving_user) ]='\0';
    return 0;
}


void extract_addr_p2p(struct user* tab_user, char* receiving_user,char* dest_addr,char* dest_port){
    memset(dest_port, '\0', sizeof(dest_port));
    memset(dest_addr, '\0', sizeof(dest_addr));
    int i;
    for(i=0; i< MAX_CLIENT ;i++){
        if(user_existing(tab_user+i)){
            if(strcmp(receiving_user, get_nickname(tab_user+i))==0){
                strcat(dest_addr,get_address(tab_user+i));
                get_port((tab_user+i),dest_port);

            }

        }
    }
}
