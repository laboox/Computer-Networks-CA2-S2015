/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include "Packet.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];

    if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
    }

    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("Opening socket");
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(atoi(argv[1]));
    if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
    fromlen = sizeof(struct sockaddr_in);
    try{
    while (1) {
       Packet p;
       //p.recive(sock, &from);
       cout<<"enter pm"<<endl;
       string pm; cin>>pm;
       p.setData(pm.c_str(), pm.size()+1);
       
       cout<<"enter port\n";
       int port; cin>>port;
       p.send(sock, port);

       p.recive(sock, &from);
       cout<<"from port is"<<from.sin_port<<endl;
       char msg[23];
       p.getData(msg);
       cout<<"msg is "<<msg<<endl;
       cout<<"packet recived!\n";
       /*n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       write(1,"Received a datagram: ",21);
       write(1,buf,n);
       n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");*/
    }
    }
    catch (Exeption ex){
        cout<< ex.get_error();
    }
    return 0;
 }

