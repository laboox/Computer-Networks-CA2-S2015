#include "Client.h"

Client::Client()
{
	addr=address(0);
}

bool Client::is_login()
{
	return username!="" and password!="";
}

bool Client::is_connected()
{
	return port!=0 and addr!=0;
}

void Client::login(string username, string password)
{
	if(is_login())
		throw Exeption("You must logout at first");
	this->username=username;
	this->password=password;

	cout<<"User "<<username<<" login successfully"<<endl;
}

void Client::connect(int port)
{
	if(! is_login())
		throw Exeption("You must login at first");
	if(is_connected())
		throw Exeption("You were connect before");

	sock=socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0)
		throw Exeption("Error in opening sock");

	struct hostent *hp;
	hp=gethostbyname("localhost");
	if(hp==0)
		throw Exeption("Unknown host");

	sw.sin_family=AF_INET;
	bcopy((char *)hp->h_addr, (char *)&sw.sin_addr, hp->h_length);
	sw.sin_port = htons(port);
	int length=sizeof(struct sockaddr_in);

	Packet p;
	p.setType(REQ_ADDR);
	p.send(sock, port);
	this->port=port;

	cout<<"Connect to port "<<port<<endl;
}

void Client::logout()
{
	if(! is_login())
		throw Exeption("You were logout before");
	
	username="";
	password="";

	Packet p;
	p.setType(DISCONNECT);
	p.setSource(addr);
	p.send(sock, port);

	//close(sock);

	cout<<"User "<<username<<"logout successfully"<<endl;
}

void Client::get_list_of_services()
{
	if(! is_login())
		throw Exeption("You must login at first");
	if(! is_connected())
		throw Exeption("You must connect at first");

    Packet p;
	p.setType(GET_SERVICES_LIST);
	p.setSource(addr);
    p.setDest(address(SERVER_ADDR));
	p.send(sock, port);
}


void Client::request(string service_name, string access_type)
{
	if(! is_login())
		throw Exeption("You must login at first");
	if(! is_connected())
		throw Exeption("You must connect at first");

    if(access_type!="read" && access_type!="write")
    	throw Exeption("Invalid access type");

    Packet p;
    p.setType(access_type=="read"? REQ_READ : REQ_WRITE);
    p.setData(service_name.c_str(), service_name.size()+1); 
    p.setSource(addr);
    p.setDest(address(SERVER_ADDR));
	p.send(sock, port);
}

void Client::send(string file)
{
	if(! is_login())
		throw Exeption("You must login at first");
	if(! is_connected())
		throw Exeption("You must connect at first");
	//manage packet fragmentation for sending file
}

void Client::append(string service_name, string data)
{
	if(! is_login())
		throw Exeption("You must login at first");
	if(! is_connected())
		throw Exeption("You must connect at first");
	//manage packet fragmentation for sending file
}

void Client::send_data(string data, string dest_addr)
{
	Packet p;
	p.setType(DATA);
	p.setSource(addr);
	p.setDest(address(dest_addr));
	p.setData(data.c_str(), data.size()+1);
	p.send(sock, port);

	cout<<"Data sent successfully"<<endl;
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
			login(username, pass);
		else
			throw Exeption("Invalid Command\nUsage: Login [#username, #password]");
	}
	else if(order=="Connect")
	{
		string param;
		int connected_port;
		if(ss>>param>>connected_port && param=="Switch")
			connect(connected_port);
		else 
			throw Exeption("Invalid Command\nUsage: Connect Switch [#Switch Port Number]");
	}
	else if(order=="Get")
	{
		string param1, param2, param3;
		if(ss>>param1>>param2>>param3 && param1=="List" && param2=="of" && param3=="Services")
			get_list_of_services();
		else 
			throw Exeption("Invalid Command\nUsage: Get List of Services[]");
	
	}
	else if(order=="Request")
	{
		string service_name, access_type;
		if(ss>>service_name>>access_type)
			request(service_name, access_type);
		else
			throw Exeption("Invalid Command\nUsage: Request[#Service Name, #Access Type]");

	}
	else if(order=="Send")
	{
		string file;
		if(ss>>file)
			send(file);
		else
			throw Exeption("Invalid Command\nUsage: Send[#File]");

	}
	else if(order=="Append")
	{
		string service_name, data;
		if(ss>>service_name>>data)
			append(service_name, data);
		else
			throw Exeption("Invalid Command\nUsage: Append[#Service Name, #Data]");
	}
	else if(order=="Logout")
		logout();
	else if(order=="SendData")
	{
		string data, dest_addr;
		if(ss>>data>>dest_addr)
			send_data(data, dest_addr);	
		else
			throw Exeption("Invalid Command\nUsage: TestSend[#Data , #Dest_Address]");
	}
	else
		throw Exeption("Invalid Command");
}

void Client::get_addr(Packet p)
{
	if(addr.to_ulong()!=0)
		throw Exeption("I have address but I get SET_ADDR packet");
	addr=p.getDest();
	cout<<"I recive addr "<<addr.to_string()<<endl;
}

void Client::get_data(Packet p)
{
	char data[MSG_LEN];
	p.getData(data);
	cout<<"I recive data "<<data<<" from client"<<p.getDest().to_ulong()<<endl;
}

void Client::parse_packet(Packet p)
{
	if(p.getTtl()==0) return;
	if(p.getType()==SET_ADDR)
		get_addr(p);
	else if(p.getType()==DATA)
	{
		get_data(p);
	}
}


void Client::run()
{
    fd_set fdset;
    while(true)
    {
        FD_ZERO(&fdset);
    	FD_SET(STDIN, &fdset);
    	FD_SET(sock, &fdset);

        try
        {
            if(select(sock+1, &fdset, NULL, NULL, NULL) < 0)
                throw Exeption("Error in sockets select");
            if(FD_ISSET(STDIN , &fdset))
            {
                string cmd;
                getline(cin, cmd);
            	parse_cmd(cmd);
            }
            else if (FD_ISSET(sock , &fdset))  
            {
                struct sockaddr_in from_sockadrr;
                Packet p;
                p.recive(sock, &from_sockadrr);
                parse_packet(p);       
            }
        }
        catch(Exeption ex)
        {
            cout << ex.get_error() << endl;
        }
    }
}
