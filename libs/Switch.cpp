#include "Switch.h"

Switch::Switch(int port)
{
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        throw Exeption("Error in opening sock");
    
    struct sockaddr_in switch_sockadrr;
    int length = sizeof(switch_sockadrr);
    bzero(&switch_sockadrr, length);
    
    switch_sockadrr.sin_family=AF_INET;
    switch_sockadrr.sin_addr.s_addr=INADDR_ANY;
    switch_sockadrr.sin_port=htons(port);
    
    if(bind(sock, (struct sockaddr *)&switch_sockadrr, length) < 0) 
        throw Exeption("Binding Error");
    this->port=port;
}

string itoa(int n)
{
    string s;
    while(n>0)
    {
        char c=n%10+'0';
        s=c+s;
        n/=10;
    }
    return s;
}   


void Switch::send(char* msg, int port)
{
    struct sockaddr_in to_sockadrr;
    int tolen = sizeof(to_sockadrr);
    bzero(&to, length);
    
    to_sockadrr.sin_family=AF_INET;
    to_sockadrr.sin_addr.s_addr=INADDR_ANY;
    to_sockadrr.sin_port=htons(port);
    int n=sendto(sock,msg, MSG_LEN, 0, (struct sockaddr *)&to_sockadrr, tolen);
    if(n<0) 
        throw Exeption("Error in sendto");
}

void Switch::connect(int port)
{
    for(int i=0; i<adjacentـports; i++)
    {
        if(adjacentـports[i]==port)
            throw Exeption("You were connected to this port before");
    }

    for(map<bitset<ADDR_SIZE>, pii>::iterator it=routing_table.begin(); it!=routing_table.end(); ++it)
    {
        Packet p;
        p.setType(CONTROL);
        p.setDest((*it).DEST);
        p.setTtl(INIT_TTL);
        string len=itoa((*it).LEN);
        p.setData(len.c_str(), len.size()+1);
        
        char msg[MSG_LEN];
        p.getMessageByPacket(msg);
        send(msg, port);
    }
    adjacentـports.push_back(port);

    Packet p;
    p.Type=CONNECT;
    char[MSG_LEN] msg;
    p.getMessageByPacket(msg);
    send(msg, port);
}

void Switch::update_routing_table(bitset<ADDR_LEN> dest, int length, const struct sockaddr_in* from)
{
    if(routing_table[dest].LEN > (len+1))
    {
        routing_table[dest].LEN=len+1;
        routing_table[dest].PORT=from->sin_port;
        
        for(int j=0; j<adjacentـports.size(); j++)
        {
            Packet p;
            p.setType(CONTROL);
            p.setDest(dest);
            p.setTtl(INIT_TTL);
            string len=itoa(length+1);
            p.setData(len.c_str(), len.size()+1);
            
            char msg[MSG_LEN];
            p.getMessageByPacket(msg);
            send(msg, adjacentـports[j]);
        }
    }
}

void Switch::parse_msg(char* msg, const struct sockaddr_in* from)
{
    Packet p(msg);
    if(p.getTtl()==0) return;

    if(p.getType()==CONTROL)
    {
        char[DATA_LEN] data;
        p.getData(data);
        update_routing_table(p.getDest(), atoi(data), from);
    }
    else if(p.getType()==DATA)
    {
        p.setTtl(p.getTtl()-1);
        if(routing_table.find(p.getDest()) == routing_table.end())
            throw Exeption("I dont know any path to send this packet");
        
        char[MSG_LEN] data;
        p.getMessageByPacket(data);  
        send(data, routing_table[p.getDest()].PORT);   
    }
    else if(p.getType()==CONNECT)
    {
        for(int i=0; i<adjacentـports.size(); i++)
            if(adjacentـports[i]==from->sin_port)
                return;
        connect(from->sin_port);
    }
    else if(p.getType()==SETADDR)
    {

    }
    else
    {
        throw Exeption("Invalid Packet Type");
    }
}

void Switch::run()
{
    
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(STDIN, &fdset);
    FD_SET(sock, &fdset);
    while(true)
    {
        try
        {
            if(select(sock+1, &fdset, NULL, NULL, NULL) < 0)
                throw Exeption("Error in sockets select");
            if(FD_ISSET(STDIN , &read_fds))
            {
                string cmd, param1, param2, connected_port;
                getline(cin, cmd);
                stringstream ss(cmd);
                if(ss>>param1>>param2>>connected_port)
                { 
                    if(param1=="Connect" && param2=="Switch")
                        connect(atoi(connected_port));
                    /*else if(param1=="Send")
                    {
                        Packet p;
                        p.setType(DATA);
                        p.setData(param2.c_str(), param2.size()+1);
                        char[MSG_LEN] msg;
                        p.getPacketByteArray(msg);
                    }*/
                }
                else
                    ("Invalid Command\nUsage: Connect Switch [#Switch Port Number]");  
            }
            else if (FD_ISSET(sock , &read_fds))  
            {
                char msg[MSG_LEN];
                struct sockaddr_in from_sockadrr;
                socklen_t fromlen = sizeof(struct sockaddr_in);
                int n=recvfrom(sock, msg, MSG_LEN, 0, (struct sockaddr *)&from_sockadrr, &fromlen);
                if(n<0) 
                    throw Exeption("Error in recvfrom");
                parse_msg(msg, from);        
            }
        }
        catch(Exeption ex)
        {
            cout << ex.get_error() << endl;
        }
    }
}

    
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        cout << "Usage: ./Switch.out #Port_Number" << endl;
        exit(0);
    }

    Switch sw(atoi(argv[1]));
    sw.run();
}


