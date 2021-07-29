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
#define max_clients 50

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

}

//
// void main(void){
//   int clients[max_clients];
//
//   for (int i=0; i<max_clients; i++)
//   {
//
//   }
//   struct sockaddr_in address;
//   int opt = 1;
//   int addrlen = sizeof(address); //defined lower in the example
//   char buffer[1024] = {0};
//   int server_sock = socket(AF_INET, SOCK_STREAM, 0);
//   //???? the example compared this value with 0
//   if (server_sock == -1){
//     perror("setsockopt");
//     exit(EXIT_FAILURE);
//   }
//   //???? what does opt do? IN the example opt is a char*
//   if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR,
//                  &opt, sizeof(opt)))
//   {
//     perror("setsockopt");
//     exit(EXIT_FAILURE);
//   }
//   address.sin_family = AF_INET;
//   address.sin_addr.s_addr = INADDR_ANY;
//   address.sin_port = htons(PORT);
//   //???? haven't seen this struct notation before
//   if (bind(server_sock, (struct sockaddr *)&address,
//                          sizeof(address))<0)
//   {
//       perror("bind failed");
//       exit(EXIT_FAILURE);
//   }
//
//   if (listen(server_sock, 3) < 0)
//   {
//       perror("listen");
//       exit(EXIT_FAILURE);
//   }
//
//   while (1)
//   {
//
//
//   }
//
//   if (((clients[0] = accept(server_sock, (struct sockaddr *)&address,
//                      (socklen_t*)&addrlen)))<0)
//   {
//       perror("accept");
//       exit(EXIT_FAILURE);
//   }
//   char msg[]= "hello world patats";
//   send(clients[0] , msg , strlen(msg) , 0 );
//   printf("Hello message sent\n");
//
//
//
//
// }

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
