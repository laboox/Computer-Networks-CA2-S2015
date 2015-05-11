#include "Switch.h"

Switch::Switch(int port)
{
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock<0)
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

void Switch::connect(int port)
{
    for(int i=0; i<connectedـswitch; i++)
    {
        if(connectedـswitch[i]==port)
            throw Exeption("You were connected to this port before");
    }

    for(map<bitset<ADDR_SIZE>, pii>::iterator it=routing_table.begin(); it!=routing_table.end(); ++it)
    {
        Packet p;
        p.setType(UPDATE);
        p.setDest((*it).DEST);
        string len=itoa((*it).LEN);
        p.setData(len.c_str(), len.size()+1);

        p.send(sock, port);
    }
    connectedـswitch.push_back(port);

    Packet p;
    p.Type=CONNECT;
    p.send(sock, port);

    cout<<"Connect to port "<<port<<endl;
}

void Switch::update_routing_table(bitset<ADDR_LEN> dest, int length, int port)
{
    if(routing_table.find(dest)==routing_table.end() or routing_table[dest].LEN > (len+1))
    {
        routing_table[dest].LEN=len+1;
        routing_table[dest].PORT=port;
        
        for(int j=0; j<connectedـswitch.size(); j++)
        {
            Packet p;
            p.setType(UPDATE);
            p.setDest(dest);
            string len=itoa(length+1);
            p.setData(len.c_str(), len.size()+1);
            
            p.send(sock, connectedـswitch[j]);
        }
    }
}

void Switch::update(Packet p, int port)
{
    char[DATA_LEN] length;
    p.getData(length);
    update_routing_table(p.getDest(), atoi(length), port);
}

void Switch::pass_data(Packet p)
{
    p.setTtl(p.getTtl()-1);
    if(routing_table.find(p.getDest()) == routing_table.end())
        throw Exeption("I dont know any path to send this packet");
    p.send(sock, routing_table[p.getDest()].PORT);   
}

void Switch::accept_connection(int port)
{
    for(int i=0; i<connectedـswitch.size(); i++)
        if(connectedـswitch[i]==port)
            return;
    connect(port);

    cout<<"I accept connection of a switch on port "<<port<<endl;
}

void Switch::set_addr(Packet p, int port)
{
    bitset<ADDR_LEN> addr = bitset<ADDR_LEN>(unique_addr++);
    for(int i=0; i<connectedـclient.size(); i++)
        if(connectedـclient[i]==port)
            throw Exeption("I recive a REQ_ADDR packet from a client whitch I connected before");

    routing_table[addr] = pii(port,1);
    connectedـclient.push_back(port);

    p.setDest(addr);
    p.setType(SET_ADDR);
    p.setTtl(p.getTtl()-1);
    p.send(sock, port);

    cout<<"I set address for clinet "<<addr.to_string()<<" on port "<<port<<endl;
}

void Switch::parse_packet(Packet p, int port)
{
    if(p.getTtl()==0) 
        return;
    if(p.getType()==UPDATE)
        update(p, port);
    else if(p.getType()==DATA)
        pass_data(p);
    else if(p.getType()==CONNECT)
        accept_connection(port);
    else if(p.getType()==REQ_ADDR)
        set_addr();
    else
        throw Exeption("Invalid Packet Type");
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
                string cmd, param1, param2;
                int connected_port;
                getline(cin, cmd);
                stringstream ss(cmd);
                if(ss>>param1>>param2>>connected_port)
                { 
                    if(param1=="Connect" && param2=="Switch")
                        connect(connected_port);
                }
                else
                    ("Invalid Command\nUsage: Connect Switch [#Switch Port Number]");  
            }
            else if (FD_ISSET(sock , &read_fds))  
            {
                struct sockaddr_in from_sockadrr;
                Packet p;
                p.recive(sock, from_sockadrr)
                parse_packet(p, from_sockadrr->sin_port);        
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
    if(argc<2)
    {
        cout << "Usage: ./Switch.out #Port_Number" << endl;
        exit(0);
    }

    Switch sw(atoi(argv[1]));
    sw.run();
}






/*
void Switch::send(char* msg, int port)
{
    struct sockaddr_in to_sockadrr;
    int tolen = sizeof(to_sockadrr);
    bzero(&to, length);
    
    to_sockadrr.sin_family=AF_INET;
    to_sockadrr.sin_addr.s_addr=INADDR_ANY;
    to_sockadrr.sin_port=htons(port);
    int n=sendto(sock, msg, MSG_LEN, 0, (struct sockaddr *)&to_sockadrr, tolen);
    if(n<0) 
        throw Exeption("Error in sendto");
}

void Switch::Send(Packet p; int port)
{
    char msg[MSG_LEN];
    P.getMessageByPacket(msg);
    send(msg, port);
}
*/
