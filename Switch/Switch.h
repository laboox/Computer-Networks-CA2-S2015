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
 	
 	void connect(int port);
 	void disconnect(Packet p);
 	void update_routing_table(string dest, int length, int port);
 	void update(Packet p);
 	void pass_data(Packet p);	
 	void accept_connection(Packet p);
 	void set_addr(Packet p, struct sockaddr_in from);
 	void parse_packet(Packet p, struct sockaddr_in from);
 	void run();
 private:
 	int unique_addr;
 	int port;
 	int sock;
 	map<string, pii> routing_table;
 	map<string, struct sockaddr_in> connected_client;
 	vector<int> connected_switch;
 }; 