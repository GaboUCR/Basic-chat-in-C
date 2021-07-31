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
#include <pthread.h>
// #include <conio.h>
#define PORT 5000
#define buffer_size 1025

int main(void)
{
  int sock=0, valread, activity;
  struct sockaddr_in serv_addr;
  char buffer[buffer_size+1];
  fd_set connections;

  char username[18];
  printf("What username do you want? It must 17  characters or less");
  fgets(username, 18, stdin);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }
  // printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , sock , inet_ntoa(serv_addr.sin_addr) , ntohs(serv_addr.sin_port));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  // printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , sock , inet_ntoa(serv_addr.sin_addr) , ntohs(serv_addr.sin_port));
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

  // while(1)
  // {
  //   // printf("Press a key to write a message, if you're writing you cannot receive messages, you will receive them when you're done writing\n");
  //   // FD_ZERO(&connections);
  //   // FD_SET(sock, &connections);
  //   // activity = select(sock + 1 ,NULL ,  &connections , NULL , NULL);
  //   read(sock , buffer, buffer_size);
  //   printf("%s\n", buffer);
  // }
  // send(sock , "loknhg", 7, 0);

  read(sock , buffer, buffer_size);
  printf("%s\n", buffer);
  send(sock, "close", 6, 0);
    // return 0;
}
