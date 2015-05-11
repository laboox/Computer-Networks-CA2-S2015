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
    if(this->port == port)
        throw Exeption("You can't connect yourself");
    for(int i=0; i<connectedـswitch; i++)
        if(connectedـswitch[i]==port)
            throw Exeption("You were connected to this port before");

    for(map<address, pii>::iterator it=routing_table.begin(); it!=routing_table.end(); ++it)
    {
        Packet p;
        p.setType(UPDATE);
        p.setSource(address(this->port));
        p.setDest((*it).DEST);
        string len=itoa((*it).LEN);
        p.setData(len.c_str(), len.size()+1);

        p.send(sock, port);
    }
    connectedـswitch.push_back(port);

    Packet p;
    p.Type=CONNECT;
    p.setSource(address(this->port));
    p.send(sock, port);

    cout<<"Connect to port "<<port<<endl;
}

void disconnect(Packet p)
{
    addr clinet_addr = p.getSource();
    if(connectedـclient.find(clinet_addr) == connectedـclient.end())
        throw Exeption("Error: User "+clinet_addr.to_string()+" is not in connected list");
    connectedـclient.erase(connectedـclient.find(clinet_addr));
}

void Switch::update_routing_table(address dest, int length, int port)
{
    if(routing_table.find(dest)==routing_table.end() or routing_table[dest].LEN > (len+1))
    {
        routing_table[dest] = pii(port, len+1);
        
        for(int j=0; j<connectedـswitch.size(); j++)
        {
            Packet p;
            p.setType(UPDATE);
            p.setSource(address(this->port));
            p.setDest(dest);
            string len=itoa(length+1);
            p.setData(len.c_str(), len.size()+1);
            
            p.send(sock, connectedـswitch[j]);
        }
    }
}

void Switch::update(Packet p)
{
    char[DATA_LEN] length;
    p.getData(length);
    update_routing_table(p.getDest(), atoi(length), p.getSource().to_ulong());
}

void Switch::pass_data(Packet p)
{
    address dest = p.getDest();
    if(connectedـclient.find(dest) != connectedـclient.end())
        return connectedـclient[dest];
    if(routing_table.find(dest) != routing_table.end())
    {
        p.setTtl(p.getTtl()-1);
        p.send(sock, routing_table[dest].PORT);   
    }
    else
        throw Exeption("I dont know any path to send this packet");
}

void Switch::accept_connection(Packet p)
{
    int sender_port = p.getSource().to_ulong();
    for(int i=0; i<connectedـswitch.size(); i++)
        if(connectedـswitch[i]==sender_port)
            return;
    connect(sender_port);

    cout<<"I accept connection of a switch on port "<<port<<endl;
}

void Switch::set_addr(Packet p, struct sockaddr_in* from)
{
    for(map<address, struct sockaddr_in*>::iterator it=connectedـclient.begin(); it!=connectedـclient.end(); ++it)
        if((*it)==from)
            throw Exeption("I recive a REQ_ADDR packet from a client whitch I connected before");

    address addr = address(unique_addr++);
    connectedـclient[addr] = from;

    p.setSource(address(this->port));
    p.setDest(addr);
    p.setType(SET_ADDR);
    p.setTtl(p.getTtl()-1);
    p.send(sock, from);

    cout<<"I set address for clinet "<<addr.to_string()<<endl;
}

void Switch::parse_packet(Packet p, struct sockaddr_in* from)
{
    if(p.getTtl()==0) 
        return;
    if(p.getType()==UPDATE)
        update(p);
    else if(p.getType()==DATA)
        pass_data(p);
    else if(p.getType()==CONNECT)
        accept_connection(p);
    else if(p.getType()==DISCONNECT)
        disconnect(p);
    else if(p.getType()==REQ_ADDR)
        set_addr(p, from);
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
                struct sockaddr_in from;
                Packet p;
                p.recive(sock, &from)
                parse_packet(p, &from);        
            }
        }
        catch(Exeption ex)
        {
            cout << ex.get_error() << endl;
        }
    }
}