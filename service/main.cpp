/**
* File "main.cpp"
* Created by Sina on Fri May  8 18:02:42 2015.
*/

#include "ServiceManager.h"

int main (){
    string command;
    ServiceManager sm;
    while (cin>>command) {
        if(command == "Connect" && cin>>command && command=="Server"){
            int port;
            cin>>port;
            sm.init(port);
            sm.connectServer();
            sm.run();
            break;
        }
        else{
            cout << "Invalid command";
        }
    }
}
