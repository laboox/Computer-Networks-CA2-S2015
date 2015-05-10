#pragma once

#include "primary_header.h"
#include "Packet.h"

#define DEST first
#define PORT first
#define LEN second


class Switch
{
public:
	Switch(int port);
	void run();
	void update_routing_table(bitset<ADDR_LEN> dest, int length, const struct sockaddr_in* from);
	void parse_msg(char* msg, const struct sockaddr_in* from);
	void connect(int port);
	~Switch();

private:
	int port;
	int sock;
	map<bitset<ADDR_SIZE>, pii> routing_table; 
	vector<int> connectedـswitch;
	vector<int> connectedـclient;
};
