#include "Client.h"

void Client::login(string username, string password)
{
	if(username!="" or password!="")
		throw Exeption("At firts logout please");
}
void Client::parse_cmd(string cmd)
{
	string order;
	stringstream ss(cmd);
	ss>>order;
	if(order=="Login")
	{
		string username, pass;
		if(ss>>username>>pass)
		{
			login(username, pass);
		}
		else
			throw Exeption("Invalid Command\nUsage: Login [#username, #password]");
	}
	else if(order=="Connect")
	{
		string param, connected_port;
		if(ss>>param>>connected_port && param=="Switch")
		{

		}
		else 
			throw Exeption("Invalid Command\nUsage: Connect Switch [#Switch Port Number]");
	}
	else if(order=="Get")
	{
		string param1, param2, param3;
		if(ss>>param1>>param2>>param3 && param1=="List" && param2=="of" && param3=="Services")
		{

		}
		else 
			throw Exeption("Invalid Command\nUsage: Get List of Services[]");
	
	}
	else if(order=="Request")
	{
		string service_name, access_type;
		if(ss>>service_name>>access_type)
		{

		}
		else
			throw Exeption("Invalid Command\nUsage: Request[#Service Name, #Access Type]");

	}
	else if(order=="Send")
	{
		string file;
		if(ss>>file)
		{

		}
		else
			throw Exeption("Invalid Command\nUsage: Send[#File]");

	}
	else if(order=="Append")
	{
		string service_name, data;
		if(ss>>service_name>>data)
		{

		}
		else
			throw Exeption("Invalid Command\nUsage: Append[#Service Name, #Data]");
	}
	else if(order=="Logout")
	{

	}
	else
		throw Exeption("Invalid Command");
}

void Client::run()
{
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(STDIN, &fdset);
    FD_SET(sock, &fdset);
    while(true)
    {
        try
        {
            if(select(sock+1, &fdset, NULL, NULL, NULL) < 0)
                throw Exeption("Error in sockets select");
            if(FD_ISSET(STDIN , &read_fds))
            {
                string cmd;
                getline(cin, cmd);
            	parse_cmd(cmd);
            }
            else if (FD_ISSET(sock , &read_fds))  
            {
                char msg[MSG_LEN];
                struct sockaddr_in from_sockadrr;
                socklen_t fromlen = sizeof(struct sockaddr_in);
                int n=recvfrom(sock, msg, MSG_LEN, 0, (struct sockaddr *)&from_sockadrr, &fromlen);
                if(n<0) 
                    throw Exeption("Error in recvfrom");
                parse_msg(msg, from);        
            }
        }
        catch(Exeption ex)
        {
            cout << ex.get_error() << endl;
        }
    }
}

int main()
{
	Client client();
	client.run();
}