/**
* File "ServerManager.cpp"
* Created by Sina on Sat May  9 13:48:32 2015.
*/

#include "ServerManager.h"

ServerManager::ServerManager (int port){
    sockSW = -1;
    sockSP=socket(AF_INET, SOCK_DGRAM, 0);
    if (sockSP < 0)
        throw Exeption("Error in opening sock");
    
    struct sockaddr_in switch_sockadrr;
    int length = sizeof(switch_sockadrr);
    bzero(&switch_sockadrr, length);
    
    switch_sockadrr.sin_family=AF_INET;
    switch_sockadrr.sin_addr.s_addr=INADDR_ANY;
    switch_sockadrr.sin_port=htons(port);
    
    if(bind(sockSP, (struct sockaddr *)&switch_sockadrr, length) < 0) 
        throw Exeption("Binding Error");
    this->port=port;
}

void ServerManager::run() {
    cout<<"running!\n";
    
    while(true)
    {
        try
        {
            fd_set fdset;
            FD_ZERO(&fdset);
            FD_SET(STDIN, &fdset);
            FD_SET(sockSW, &fdset);
            FD_SET(sockSP, &fdset);
            if(select(max(sockSW,sockSP)+1, &fdset, NULL, NULL, NULL) < 0)
                throw Exeption("Error in sockets select");
            if(FD_ISSET(STDIN , &fdset))
            {
                string cmd, param1, param2, connected_port;
                getline(cin, cmd);
                stringstream ss(cmd);
                if(ss>>param1>>param2>>connected_port)
                { 
                    if(param1=="Connect" && param2=="Switch")
                        connect(atoi(connected_port.c_str()));
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
            else if (FD_ISSET(sockSW , &fdset))  
            {
                 
            }
            else if (FD_ISSET(sockSP , &fdset))  
            {
                cout<<"a packet coming from Service Provider.\n";
                Packet p;
                struct sockaddr_in from_sockadrr;
                socklen_t fromlen = sizeof(struct sockaddr_in);
                p.recive(sockSP, (struct sockaddr*)&from_sockadrr, &fromlen);
                if(p.getType()==SETUPSERVICE){
                    cout<<"service provider connected.\n";
                }
            }
        }
        catch(Exeption ex)
        {
        } 
    } 
}

void ServerManager::connect(int port){
}
