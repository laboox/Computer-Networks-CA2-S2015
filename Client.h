#pragma once

#include "primary_header.h"
#include "Packet.h"
#include "Socket.h"

class Client
{
public:
	Client();
	~Client();
	
	void parse_cmd(string cmd);
	void parse_msg(char* msg, const struct sockaddr_in* from);
	
	bool is_login();
	bool is_connected();
	
	void login(string username, string password);
	void connect(int port);
	void logout();
	void get_list_of_services();
	void request(string service_name, string access_type);
	void send(string file);
	void append(string service_name, string data);
	void parse_cmd(string cmd);
	void parse_packet(Packet p, const struct sockaddr_in* from);
	
	void run();

private:
	string username;
	string password;
	struct sockaddr_in sw;
	int port;
	int sock;
	bitset<ADDR_LEN> addr;
};
