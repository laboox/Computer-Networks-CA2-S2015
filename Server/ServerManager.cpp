/**
* File "ServerManager.cpp"
* Created by Sina on Sat May  9 13:48:32 2015.
*/

#include "ServerManager.h"

ServerManager::ServerManager (int port): firewall("AC.txt"){
    addr = address(SERVER_ADDR);
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
    
    if(bind(sockSP,(struct sockaddr *)&switch_sockadrr, length) < 0) 
        throw Exeption("Binding Error");
    this->port=port;
}

void ServerManager::run() {
    cout<<"running!\n";
    fd_set fdset;
    struct sockaddr_in SP_sockadrr = {0};
    struct sockaddr_in SW_sockadrr = {0} ;
    while(true)
    {
        try
        {
            FD_ZERO(&fdset);
            FD_SET(STDIN, &fdset);
            FD_SET(sockSW, &fdset);
            FD_SET(sockSP, &fdset);

            if(select(max(sockSW,sockSP)+1, &fdset, NULL, NULL, NULL) < 0)
                throw Exeption("Error in sockets select");
            if(FD_ISSET(STDIN , &fdset))
            {
                string cmd, param1, param2;
                int connected_port;
                getline(cin, cmd);
                stringstream ss(cmd);
                if(ss>>param1>>param2>>connected_port)
                { 
                    if(param1=="Connect" && param2=="Switch"){
                        connect(connected_port, &SW_sockadrr);
                        sw_port = connected_port;
                    }
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
                    cout<<("Invalid Command\nUsage: Connect Switch [#Switch Port Number]");
            }
            else if (FD_ISSET(sockSW , &fdset))
            {
                struct sockaddr_in from = {0};
                cout<<"packet recived from socket!\n";
                Packet p;
                p.recive(sockSW, &from);
                if(p.getType()==GET_SERVICES_LIST){
                   string services=getAllServiceList();
                   Packet res;
                   res.setType(GET_SERVICES_LIST);
                   res.setSource(p.getDest());
                   res.setDest(p.getSource());
                   res.setData(services);
                   res.send(sockSW, &from);
                   cout<<"response sent\n";
                }
                else if(p.getType()==REQ_READ || p.getType()==REQ_WRITE){
                    istringstream iss(p.getDataStr());
                    string file, uname;
                    if(getline(iss, file) && getline(iss, uname)){
                        cout<<"requsting to access "<<file<<" for "<<uname<<endl;
                        response(p.getSource(), uname, file, (p.getType()==REQ_READ)?READ:WRITE);
                    }
                    else{
                        sendError("packet is not valid.\n", p.getSource());
                    }
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
            cout<<ex.get_error()<<endl;
        } 
    } 
}

void ServerManager::response(address dest, string uname, string file, Access access){
    char buffer[2048] = {0};
    if(firewall.isGranted(uname, file, access)){
        struct sockaddr_in* serv = toService[file[0]];
        Packet req;
        req.setType(REQ_READ);
        req.setData(file);
        req.send(sockSP, serv);
        //Packet res;
        //res.recive();
        reciveFrame(buffer, sockSP, serv);
        sendFrame(buffer, strlen(buffer)+1, addr, dest, sockSW, sw_port);
    }
    else
        sendError("your dont have access on this service\n", dest);
}


void ServerManager::connect(int port, struct sockaddr_in* sw){
    if(sockSW != -1)
        throw Exeption("already connected");
    
    sockSW=socket(AF_INET, SOCK_DGRAM, 0);
	if(sockSW<0)
		throw Exeption("Error in opening sock");

	struct hostent *hp;
	hp=gethostbyname("localhost");
	if(hp==0)
		throw Exeption("Unknown host");

	sw->sin_family=AF_INET;
	bcopy((char *)hp->h_addr, (char *)&(sw->sin_addr), hp->h_length);
	sw->sin_port = htons(port);

    Packet req;
    req.setSource(addr);
    req.setType(REQ_SERVER);
    req.send(sockSW,port);
    cout<<"Connecting SW\n";
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
    cout<<"from "<<info.getSource().to_ulong()+'A'<<" cames "<<buf<<endl;
    return buf;
}

string ServerManager::getAllServiceList(){
    string servlist;
    for(int i=0;i<services.size();i++){
        servlist += getServiceList(services[i]);
    }
    return servlist;
}

void ServerManager::sendError(string message, address dest){
    Packet p;
    p.setType(ERROR);
    p.setDest(dest);
    p.setSource(addr);
    p.setData(message);
    p.send(sockSW, sw_port);
}
