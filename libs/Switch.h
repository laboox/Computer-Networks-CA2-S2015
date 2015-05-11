#pragma once

#include "primary_header.h"
#include "Packet.h"

#define DEST first
#define PORT first
#define LEN second

typedef pair<struct sockaddr_in, int> route_data;


class Switch
{
public:
	Switch(int port);
	void connect(int port);
	void update_routing_table(address dest, int length, int port);
	void update(Packet p);
	void pass_data(Packet p);
	void accept_connection(Packet p);
	void set_addr(Packet p, struct sockaddr_in* from);
	void parse_packet(Packet p, struct sockaddr_in* from);
	void run();
	~Switch();

private:
	int port;
	int sock;
	map<address, pii> routing_table; 
	map<address, struct sockaddr_in*> connectedـclient; 
	vector<int> connectedـswitch;
};
