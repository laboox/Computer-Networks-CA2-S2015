/**
* File "Firewall.cpp"
* Created by Sina on Wed May 13 00:20:29 2015.
*/

#include "Firewall.h"

Permission::Permission(bool read, bool write){
    this->read = read;
    this->write = write;
}

void Permission::addAccess(bool read, bool write){
    this->read = read | this->read;
    this->write = write | this->write;
}

bool Permission::checkAccess(Access access){
    if(access==READ)
        return read;
    else
        return write;
}

Firewall::Firewall(string path){
    ifstream ifs(path);
    string line;
    while(getline(ifs,line)){
        string user, file, access;
        istringstream iss(line);
        if(iss>>user>>file>>access){
            bool read=false;
            bool write=false;
            if(access=="Read")
                read = true;
            else if(access=="Write")
                write = true;
            else    
                throw Exeption("Error parsing AC.txt\n");
            table[pss(user, file)].addAccess(read, write);
        }
        else
            throw Exeption("Error parsing AC.txt\n");
    }
}

bool Firewall::isGranted(string uname, string file, Access access){
    if(table.count(pss(uname, file))==0)
        return false;
    else
        return table[pss(uname, file)].checkAccess(access);
}
