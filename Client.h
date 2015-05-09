#pragma once

#include "primary_header.h"
#include "Packet.h"
#include "Socket.h"

class Client
{
public:
	Client();
	void run();
	void parse_cmd(string cmd);
	void parse_msg(char* msg, const struct sockaddr_in* from);
	~Client();
private:
	string username;
	string password;
	int port;
	int sock;
};