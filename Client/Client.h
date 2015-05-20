#pragma once

#include "primary_header.h"
#include "Packet.h"
#include "Framer.h"

class Client
{
public:
	Client();

	void parse_cmd(string cmd);
	void parse_packet(Packet p);
	
	bool is_login();
	bool is_connected();
	
	void login(string username, string password);
	void logout();
	void connect(int port);
	void get_list_of_services();
	void request(string service_name, string access_type);
	void send(string file);
	void send_data(string data, string dest_addr);
	void append(string service_name, string data);

	void get_data(Packet p);
	void get_addr(Packet p);
	
	void run();

private:
	string username;
	string password;
	struct sockaddr_in sw;
	int port;
	int sock;
	bitset<ADDR_LEN> addr;
};
