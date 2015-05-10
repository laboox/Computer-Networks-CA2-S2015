/**
* File "main.cpp"
* Created by Sina on Fri May  8 18:02:42 2015.
*/

#include "ServiceManager.h"

int main (int argc, char *argv[]){
    string command;
    ServiceManager sm;
    if(argc<2){
        cout<<"Usage: ./Service_Provider.out #SERVICE_PATH.\n";
        exit(0);
    }
    try{
    while (cin>>command) {
        if(command == "Connect" && cin>>command && command=="Server"){
            int port;
            cin>>port;
            sm.init(port, argv[1]);
            sm.connectServer();
            sm.run();
            break;
        }
        else{
            cout << "Invalid command\n";
        }
    }
    }
    catch(Exeption ex){
        cout<<ex.get_error()<<endl;
    }
}
