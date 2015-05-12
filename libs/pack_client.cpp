/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Packet.h"
#include "Framer.h"

void error(const char *);
int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];
   
   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   cout<<"connecting sock!\n";
   
   string file = readAllFile("1.txt");

   cout<<"file: "<<file<<endl;
   
   char* filep= (char*)malloc((file.size()+1)*sizeof(char));
   
   memcpy(filep, file.c_str(), file.size()+1);

   cout<<"try to send "<<filep<<endl;

   sendFrame(filep, file.size()+1, address(0), address(0), sock, &server);
      
   /*Packet p;
   p.setData("salam", 6);
   p.send(sock, atoi(argv[2]));
   p.recive(sock, &from);

   p.getData(buffer);
   cout<<buffer<<endl;

   printf("Please enter the message: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);
   n=send(sock,buffer,
            strlen(buffer),0);
   if (n < 0) error("Sendto");
   n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
   if (n < 0) error("recvfrom");
   write(1,"Got an ack: ",12);
   write(1,buffer,n);*/
   close(sock);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
