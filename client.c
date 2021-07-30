#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#define PORT 5000
#define buffer_size 10

int main(void){
  int sock=0, valread, activity;
  struct sockaddr_in serv_addr;
  char buffer[buffer_size+1];


  char username[18];
  printf("What username do you want? It must 17  characters or less");
  fgets(username, 18, stdin);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
  {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
      printf("\nConnection Failed \n");
      return -1;
  }
  // send(sock , "hello" , 6 , 0 );
  char msg[buffer_size];
  //ask for all of the messages
  while (1)
  {
    read(sock , buffer, buffer_size);
    if (strcmp(buffer, "end") == 0)
    {
      break;
    }
    // printf("%d\n", valread);
    printf("%s", buffer);
  }

  while(1)
  {
    activity = select( sock + 1 , &connections , NULL , NULL , NULL);

  }

  send(sock, "close", 6, 0);


    // return 0;
  }
