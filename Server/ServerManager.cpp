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
            struct sockaddr_in SP_sockadrr;
            struct sockaddr_in SW_sockadrr;

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
                else if(cmd == "getlist"){
                    cout<<"getting services list from.\n";
                    cout<<getAllServiceList();
                }
                else
                    ("Invalid Command\nUsage: Connect Switch [#Switch Port Number]");
            }
            else if (FD_ISSET(sockSW , &fdset))
            {
                 Packet p;
                 p.recive(sockSW, &SW_sockadrr);
                 if(p.getType()==GET_SERVICES_LIST){
                    //TODO
                 }
            }
            else if (FD_ISSET(sockSP , &fdset))  
            {
                cout<<"a packet coming from Service Provider.\n";
                Packet p;
                p.recive(sockSP, &SP_sockadrr);
                if(p.getType()==SETUPSERVICE){
                    services.push_back(SP_sockadrr);
                    toService[p.getDataStr()[0]] = &(services[services.size()-1]);
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

string ServerManager::getServiceList(struct sockaddr_in sp){
    struct sockaddr_in from;
    char buf[DATA_LEN]={0};
    Packet p;
    p.setType(GET_SERVICES_LIST);
    p.send(sockSP, &sp);
    Packet info;
    info.recive(sockSP, &from);
    info.getData(buf);
    return buf;
}

string ServerManager::getAllServiceList(){
    string servlist;
    for(int i=0;i<services.size();i++){
        servlist = getServiceList(services[i]);
    }
    return servlist;
}

void ServerManager::sendError(string message, address dest){
}
