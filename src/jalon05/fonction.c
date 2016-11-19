#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "fonction.h"


void error(const char *msg){
  perror(msg);
  exit(0);
}

void do_connect(int sock_client, char hostname[], int port, struct sockaddr_in* sock_host ){

  memset(sock_host, '\0', sizeof(*sock_host));
  sock_host->sin_family = AF_INET;
  sock_host->sin_port = htons(port);
  inet_aton(hostname, & sock_host->sin_addr);

  if (connect(sock_client, (struct sockaddr*)  sock_host, sizeof(*sock_host)) < 0)
    error("erreur connect");
}

struct sockaddr_in init_serv_addr(int port){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port) ;
    return addr;
}


//Extract sub string from string
char *str_sub (const char *s, unsigned int start, unsigned int end){
  char *new_s = NULL;

  if (s != NULL && start < end){
    new_s = malloc (sizeof (*new_s) * (end - start + 2));
    if (new_s != NULL){
      int i;
      for (i = start; i <= end; i++){
        new_s[i-start] = s[i];
      }
      new_s[i-start] = '\0';
    }
    else{
      fprintf (stderr, "Memoire insuffisante\n");
      exit (EXIT_FAILURE);
    }
  }
  return new_s;
}

char* get_1_arg(char* buffer){
  int i=0;
  int position=0;
  char* pseudo=malloc(20);
  while(1){
    if((buffer[i] == ' ')  || (buffer[i] == '\n') || (buffer[i] == '\0')){
      pseudo[i-position]='\0';
      return pseudo;
    }
    else{
      pseudo[i-position]=buffer[i];
      i++;
    }
  }
}

char* get_2_arg(char* buffer){
  int i=0;
  int arg2_position;
  char* pseudo=malloc(20);

  while(buffer[i]!=' '){
    i++;
  }
  i++;
  arg2_position=i;

  while(1){
    if((buffer[i] == ' ')  || (buffer[i] == '\n') || (buffer[i] == '\0')){
      pseudo[i-arg2_position]='\0';
      return pseudo;
    }
    else{
      pseudo[i-arg2_position]=buffer[i];
      i++;
    }
  }
}


char* get_3_arg(char* buffer){
  int i=0;
  int position;
  char* arg_3=malloc(20);

  int n=0;
  while(n<2){
    if(buffer[i]==' '){
      n++;
    }
    i++;
  }

  position=i;
  while(1){
    if((buffer[i] == ' ')  || (buffer[i] == '\n') || (buffer[i] == '\0')){
      arg_3[i-position]='\0';
      return arg_3;
    }
    else{
      arg_3[i-position]=buffer[i];
      i++;
    }
  }
}

void get_msgall(const char* buffer, char* msg){
    int i=8;
    while(buffer[i] != '\n'){
        msg[i-8]=buffer[i];
        i++;
    }
    msg[i-8]='\0';
}

int extract_unicast(char* buffer, char* receiving_user, char* msg_to_send){
    int position=5;
    int i =position;


    while((buffer[i] != ' ') ){
        if (buffer[i] == '\n'){
            return 1;
        }
        receiving_user[i-position]=buffer[i];
        i++;
    }
    receiving_user[i-position]='\0';
    i++;
    while((buffer[i] != '\n') ){
        msg_to_send[i-position - strlen(receiving_user)-1]=buffer[i];
        i++;
    }
    msg_to_send[i-position - strlen(receiving_user) ]='\0';
    return 0;
}




void getdate(char* date_in){

    time_t secondes;
    struct tm instant;
    char date[20] ={0};
    char tmp[5];
    time(&secondes);
    instant=*localtime(&secondes);

    sprintf(tmp, "%d", instant.tm_mday);
    strcat(date,tmp);
    strcat(date,"/");

    sprintf(tmp, "%d", instant.tm_mon+1);
    strcat(date,tmp);
    strcat(date,"/");


    sprintf(tmp,"%d", instant.tm_year+1900);
    strcat(date,tmp);
    strcat(date,"@");

    sprintf(tmp, "%d", instant.tm_hour);
    strcat(date,tmp);
    strcat(date,":");

    sprintf(tmp, "%d", instant.tm_min);
    strcat(date,tmp);

    strcpy(date_in, date);

}
