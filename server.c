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
#define buffer_size 10

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

void main(void){
  // int *clients = malloc(4);
  // int clients_count=0, clients_space=1;

  int clients[max_clients];
  char buffer[buffer_size];

  char *messages = malloc(9);
  int str_space = 9;
  strcpy(messages, "Welcome!");
  safe_str_add(messages, " hola yo me llamo juan miguel mora rosas instructor de como jugar", &str_space);

  fd_set connections;
  int max_socket, i, activity, new_socket, chunks;

  for (i=0; i<max_clients; i++){
    clients[i] = -1;
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
    max_socket = server_sock;

    for (i=0; i<max_clients; i++)
    {
      if (clients[i] > -1)
      {
        FD_SET(clients[i], &connections);
      }
      if (clients[i] > max_socket)
      {
        max_socket = clients[i];
      }
    }
    // printf("%d\n", max_socket);
    activity = select( max_socket + 1 , &connections , NULL , NULL , NULL);
    // printf("somethings\n");
    //check for activity in the server Socket
    if (FD_ISSET(server_sock, &connections))
    {
      if (((new_socket = accept(server_sock, (struct sockaddr *)&address,
                         (socklen_t*)&addrlen)))<0)
      {
          perror("accept");
          exit(EXIT_FAILURE);
      }
      printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
      chunks = strlen(messages)/buffer_size + (strlen(messages)%buffer_size !=0);

      for (i=0; i < chunks; i++)
      {
        printf("we are here\n");
        if(send(new_socket, messages+i*buffer_size, buffer_size, 0) != buffer_size )
        {
          perror("send");
        }
      }
      send(new_socket, "end", 4, 0);

      for (i=0; i<max_clients; i++)
      {
        if (clients[i] == -1)
        {
          clients[i] = new_socket;
          break;
        }
        else
        {
          printf("available sockets %d\n", clients[i]);
        }
      }
    }
    else
    {
      for(i=0; i<max_clients; i++)
      {

        if (FD_ISSET(clients[i], &connections))
        {
          printf("%d\n", clients[i]);
          read(clients[i], buffer, buffer_size);
          printf("%s\n", buffer);
          if (strcmp(buffer, "close") == 0)
          {
            getpeername(clients[i] , (struct sockaddr*)&address , \
            (socklen_t*)&addrlen);
            printf("Host disconnected , ip %s , port %d \n" , \
            inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            close(clients[i]);
            clients[i] = -1;
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
