#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <exception>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <map>
#include <sstream>
#include <time.h> 
#include <bitset>
#include <cctype>
#include <cstdint>

using namespace std;

typedef pair<int, int> pii;

class Exeption {
private:
    string error;
public:
    Exeption(string err): error(err) {};
    string getErr() {return error;}
};
