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
    unique_addr=port*100;

    cout<<"Switch bind on port "<<port<<endl;
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
    for(int i=0; i<connected_switch.size(); i++)
        if(connected_switch[i]==port)
            throw Exeption("You were connected to this port before.");
    for(map<string, pii>::iterator it=routing_table.begin(); it!=routing_table.end(); ++it)
    {
        Packet p;
        p.setType(UPDATE);
        p.setSource(address(this->port));
        p.setDest(address(it->DEST));
        string len=itoa((it->second).LEN);
        p.setData(len.c_str(), len.size()+1);
        
        p.send(sock, port);
    }

    connected_switch.push_back(port);

    Packet p;
    p.setType(CONNECT);
    p.setSource(address(this->port));
    p.send(sock, port);

    cout<<"Connect to port "<<port<<endl;
}

void Switch::disconnect(Packet p)
{
    string clinet_addr = p.getSource().to_string();
    if(connected_client.find(clinet_addr) == connected_client.end())
      throw Exeption("Error: User "+clinet_addr+"is not in connected list");
    connected_client.erase(connected_client.find(clinet_addr));
    cout << "Client "<<clinet_addr<<" disconnect"<<endl;
}

void Switch::update_routing_table(string dest, int length, int port)
{
    if(routing_table.find(dest)==routing_table.end() or routing_table[dest].LEN > (length+1))
    {
        routing_table[dest] = pii(port, length+1);

        for(int j=0; j<connected_switch.size(); j++)
        {
            Packet p;
            p.setType(UPDATE);
            p.setSource(address(this->port));
            p.setDest(address(dest));
            string len=itoa(length+1);
            p.setData(len.c_str(), len.size()+1);
          
            p.send(sock, connected_switch[j]);
        }
    }
}

void Switch::update(Packet p)
{

    char length[DATA_LEN] = {0};
    p.getData(length);
    update_routing_table(p.getDest().to_string(), atoi(length), p.getSource().to_ulong());
}

void Switch::pass_data(Packet p)
{
    string dest = p.getDest().to_string();
    if(connected_client.find(dest) != connected_client.end())
        p.send(sock, &connected_client[dest]);
    else if(routing_table.find(dest) != routing_table.end())
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
    for (int i=0; i<connected_switch.size(); ++i)
        if(connected_switch[i]==sender_port)
            return;
    connect(sender_port);
    cout<<"I accept connection of a switch on port "<<sender_port<<endl;
}

void Switch::set_addr(Packet p, struct sockaddr_in from)
{
    for(map<string, struct sockaddr_in>::iterator it=connected_client.begin(); it!=connected_client.end(); it++)
        if(memcmp(&(it->second), &from, sizeof(from)))
            throw Exeption("I recive a REQ_ADDR packet from a client whitch I connected before");

    string addr=address(++unique_addr).to_string();
    connected_client[addr]=from;

    p.setSource(address(this->port));
    p.setDest(address(addr));
    p.setType(SET_ADDR);
    p.setTtl(p.getTtl()-1);
    p.send(sock, &from);

    update_routing_table(addr, 0, port);
    cout<<"I set address for clinet "<<addr<<endl;
}

void Switch::set_server(Packet p, struct sockaddr_in from){
    for(map<string, struct sockaddr_in>::iterator it=connected_client.begin(); it!=connected_client.end(); it++)
        if(memcmp(&(it->second), &from, sizeof(from)))
            throw Exeption("I recive a REQ_ADDR packet from a client whitch I connected before");

    string addr=p.getSource().to_string();
    connected_client[addr]=from;

    /*p.setSource(address(this->port));
    p.setDest(address(addr));
    p.setType(SET_ADDR);
    p.setTtl(p.getTtl()-1);
    p.send(sock, &from);*/

    update_routing_table(addr, 0, port);
    cout<<"I set address for server "<<addr<<endl;
}

void Switch::parse_packet(Packet p, struct sockaddr_in from)
{
    if(p.getTtl()==0) 
        return;
    if(p.getType()==UPDATE)
        update(p);
    else if(p.getType()==CONNECT)
        accept_connection(p);
    else if(p.getType()==DISCONNECT)
        disconnect(p);
    else if(p.getType()==REQ_ADDR)
        set_addr(p, from);
    else if(p.getType() == REQ_SERVER)
        set_server(p, from);
    else 
        pass_data(p);
}

void Switch::run()
{
    
    fd_set fdset;
    while(true)
    {
        FD_ZERO(&fdset);
        FD_SET(STDIN, &fdset);
        FD_SET(sock, &fdset);

        try
        {
            if(select(sock+1, &fdset, NULL, NULL, NULL)<0)
                throw Exeption("Error in sockets select");
            if(FD_ISSET(STDIN , &fdset))
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
            else if (FD_ISSET(sock , &fdset))  
            {
                struct sockaddr_in from;
                Packet p;
                p.recive(sock, &from);
                parse_packet(p, from);        
            }
        }
        catch(Exeption ex)
        {
            cout << ex.get_error() << endl;
        }
    }
}

