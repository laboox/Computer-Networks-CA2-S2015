/**
* File "ServiceManager.cpp"
* Created by Sina on Fri May  8 19:31:06 2015.
*/

#include "ServiceManager.h"

void ServiceManager::sendFile(string file, string addr){
    std::ifstream t(path+"/"+file);
    std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
    
}

void ServiceManager::run(){
    struct sockaddr_in from;
    while(true){
        Packet inp;
        inp.recive(sock, &from);
        try{
        if(inp.getType()==GET_SERVICES_LIST){
            string list = getList();
            inp.setData(list);
            inp.setSource(source);
            inp.send(sock, &from);
        }
        else if(inp.getType()==REQ_READ){
            string file=inp.getDataStr();
            cout<<"try to send "<<(path+"/"+file)<<" to server\n";
            if(!isFileExist(path+"/"+file)){
                sendError("service does not exist\n", &from);
                continue;
            }
            else{
                string data;
                data = readAllFile(path+"/"+file);
                char* buf = new char[data.size()+1];
                memcpy(buf, data.c_str(), data.size()+1);
                cout<<"I want to send file below to server:"<<endl<<buf<<endl;
                sendFrame(buf, data.size()+1, source, address(SERVER_ADDR), sock, &from);
                delete[] buf;
            }
        }
        }
        catch (Exeption ex){
            cout<<ex.get_error()<<endl;
        }
    }
}

void ServiceManager::sendError(string message, struct sockaddr_in* to){
    Packet p;
    p.setType(ERROR);
    p.setSource(source);
    p.setData(message);
    p.send(sock, to);
}

void ServiceManager::init(int portnum, string path){
   this->path = path;
   source = path[0] - 'A';
   port = portnum;
   int n;
   unsigned int length;
   struct hostent *hp;
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) Exeption("build sock error!");

   server.sin_family = AF_INET;
   hp = gethostbyname("localhost");
   if (hp==0) Exeption("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(portnum);
   length=sizeof(struct sockaddr_in);
}

void ServiceManager::connectServer(){
    Packet p;
    p.setType(SETUPSERVICE);
    p.setSource(source);
    p.setData(path);
    p.send(sock, port);
    return;
}

string ServiceManager::getList(){
    string ret;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        string filename = ent->d_name;
        if(filename != "." && filename!=".."){
            ret+=ent->d_name;
            ret+="\n";
        }
    }
    closedir (dir);
    return ret;
    } else {
      throw Exeption("failed to load directory!\n");
    }
}
