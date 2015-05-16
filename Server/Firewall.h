/**
* File "Firewall.h"
* Created by Sina on Wed May 13 00:20:24 2015.
*/

#pragma once

#include "primary_header.h"

typedef pair<string,string> pss;

enum Access{
    READ,
    WRITE
};

class Permission{
public:
    Permission(bool read, bool write);
    Permission() {read = false; write=false;}
    bool checkAccess(Access access);
    void addAccess(bool read, bool write);
private:
    bool read;
    bool write;
};

class Firewall{
public:
    Firewall(string);
    bool isGranted(string uname, string file, Access access);
private:
    map<pair<string,string>, Permission> table;
};
