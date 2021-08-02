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

void receive_msg(int fd_sock, char* buffer)
{
  while (1)
  {
    read(fd_sock , buffer, buffer_size);
    if (strcmp(buffer, "end") == 0)
    {
      break;
    }
    printf("%s", buffer);
  }
}

int main(void)
{
  int sock=0, valread, activity;
  struct sockaddr_in serv_addr;
  char buffer[buffer_size+1];
  fd_set connections;

  char username[20];
  char placeholder[18];

  printf("What username do you want? It must 17  characters or less, more than that will cause an error\n");
  fgets(placeholder, 18, stdin);
  while ((getchar()) != '\n');

  memmove(username, placeholder, strlen(placeholder)-1);
  strcat(username, ": ");


  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

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
  //ask for all of the messages
  receive_msg(sock, buffer);

  char msg[buffer_size+1];
  char text[buffer_size+1-strlen(username)];
  strcpy(msg, username);
  char in[2];
  printf("%s", msg);
  while(1)
  {
    printf("\nPress r to receive any new message, press w to write a message, you will not receive messages will you write. Press c to close");
    scanf("%s", in);
    while ((getchar()) != '\n');

    if (strcmp(in, "r") == 0)
    {
      printf("het\n");
      receive_msg(sock, buffer);
    }
    if (strcmp(in, "w") == 0)
    {
      // scanf("%s\n", text);
      fgets(text, buffer_size+1-strlen(username), stdin);
      while ((getchar()) != '\n');
      strcat(msg, text);

      printf("%s\n", msg);
      send(sock, msg, buffer_size, 0);
      strcpy(msg, username);
    }
    if (strcmp(in, "c") == 0)
    {
      break;
    }
    strcpy(in, "");

  }
  send(sock, "close", 6, 0);
  return 0;
}
