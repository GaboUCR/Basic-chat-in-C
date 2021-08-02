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
#define max_clients 100
#define buffer_size 1025

//here the file descriptor of each client will be saved and the messages that
//this client has not receive
struct client
{
  int fd;
  char *msgs;
};

//str must have been allocated in the heap
void safe_str_add(char *str, char *new_str, int *space)
{
  int needed_space = strlen(str) +strlen(new_str)+1;

  if (needed_space > *space)
  {
    *space = (needed_space)*2;
    str = (char *) realloc(str, *space);
  }

  strcat(str, new_str);
}
//first must have been allocated in the heap
void safe_int_array_add(int *first, int new_int, int*length, int*space)
{
  (*length)++;

  if (*length +1 > *space)
  {
    first = (int *) realloc(first, (*space)*2*4);
    *space = (*space)*2;
  }

  first[*length] = new_int;
  printf("%d\n", first[0]);
}

void send_message(int fd_sock, char*messages, int chunks)
{
  for (int i=0; i < chunks; i++)
  {
    if(send(fd_sock, messages+i*buffer_size, buffer_size, 0) != buffer_size )
    {
      perror("send");
    }
  }
  send(fd_sock, "end", 4, 0);
}

void main(void){
  // int *clients = malloc(4);
  // int clients_count=0, clients_space=1;

  struct client clients[max_clients];
  char buffer[buffer_size+1];

  char *messages = malloc(11);
  int str_space = 11;
  strcpy(messages, "Welcome!\n");

  struct timeval time = {.tv_sec=0, .tv_usec=0};
  fd_set connections;
  fd_set w_connections;
  int max_socket, i, activity, new_socket, chunks;

  for (i=0; i<max_clients; i++){
    clients[i].fd = -1;
  }

  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  //???? the example compared this value with 0
  if (server_sock == -1){
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  //???? what does opt do? IN the example opt is a char*
  if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR,
                 &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_sock, (struct sockaddr *)&address,
                         sizeof(address))<0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", PORT);

  if (listen(server_sock, 3) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  puts("Waiting for connections ...");

  while (1)
  {
    FD_ZERO(&connections);
    FD_SET(server_sock, &connections);
    FD_ZERO(&w_connections);
    FD_SET(server_sock, &w_connections);
    max_socket = server_sock;

    for (i=0; i<max_clients; i++)
    {
      if (clients[i].fd > -1)
      {
        FD_SET(clients[i].fd, &connections);
        FD_SET(clients[i].fd, &w_connections);
      }
      if (clients[i].fd > max_socket)
      {
        max_socket = clients[i].fd;
      }
    }
    // printf("%d\n", max_socket);
    // printf("max socket %d\n", max_socket);
    activity = select(max_socket + 1 , &connections , &w_connections , NULL , &time);
    // printf("somethings\n");
    //check for activity in the server Socket
    if (FD_ISSET(server_sock, &connections) || FD_ISSET(server_sock, &w_connections))
    {
      if (((new_socket = accept(server_sock, (struct sockaddr *)&address,
                         (socklen_t*)&addrlen)))<0)
      {
          perror("accept");
          exit(EXIT_FAILURE);
      }
      printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
      chunks = strlen(messages)/buffer_size + (strlen(messages)%buffer_size !=0);
      send_message(new_socket, messages, chunks);

      for (i=0; i<max_clients; i++)
      {
        if (clients[i].fd == -1)
        {
          clients[i].fd = new_socket;
          break;
        }
        // else
        // {
        //   printf("available sockets %d\n", clients[i].fd);
        // }
      }
    }
    else
    {
      for(i=0; i<max_clients; i++)
      {
        // if (FD_ISSET(clients[i].fd, &w_connections))
        // {
        //   printf("listening to %d\n", clients[i].fd);
        //   read(clients[i].fd, buffer, buffer_size);
        //   printf("ends listening to %d\n", clients[i].fd);
        //   printf("%s\n", buffer);
        // }
        if (FD_ISSET(clients[i].fd, &connections))
        {
          read(clients[i].fd, buffer, buffer_size);
          printf("%s\n", buffer);
          if (strcmp(buffer, "close") == 0)
          {
            getpeername(clients[i].fd, (struct sockaddr*)&address , \
            (socklen_t*)&addrlen);
            printf("Host disconnected , ip %s , port %d \n" , \
            inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            close(clients[i].fd);
            clients[i].fd = -1;
          }

        }
      }
    }

  }
}

// void main(){
//   char s[] = realloc(NULL,15);
//   char *s = malloc(7);
//   strcpy(s,"perros");
//   char *t = "fsef";
//   int l = 7;
//   int *t = malloc(4*3);
//   int len=-1, space=3;
//
//   safe_int_array_add(t, 5, &len, &space);
//   safe_int_array_add(t, 7, &len, &space);
//   safe_int_array_add(t, 50, &len, &space);
//   safe_int_array_add(t, 506, &len, &space);
//
//   printf("%d\n", t[0]);
//   printf("%d\n", t[1]);
//   printf("%d\n", t[2]);
//   printf("%d\n", space);
// }
