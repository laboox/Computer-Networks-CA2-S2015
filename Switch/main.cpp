#include "Switch.h"

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        cout << "Usage: ./Switch.out #Port_Number" << endl;
        exit(0);
    }

    Switch sw(atoi(argv[1]));
    sw.run();
}
