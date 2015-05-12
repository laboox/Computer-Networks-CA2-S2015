#include "Switch.h"

int main(int argc, char *argv[])
{
	try
    {
    	if(argc<2)
	    {
	        cout << "Usage: ./Switch.out #Port_Number" << endl;
	        exit(0);
	    }

	    Switch sw(atoi(argv[1]));
	    sw.run();
    }
    catch(Exeption ex)
	{
            cout << ex.get_error() << endl;
    }        
}
