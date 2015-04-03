#include "UDPManagerClient.h"

UDPManagerClient::UDPManagerClient(SessionManager &session, const std::string &ip, unsigned short port, bool noUDP) :
UDPManager(session, port, !noUDP, false)
{
	(void)ip;
	/*
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip.c_str());
	*/

	this->launchThreads();
}


UDPManagerClient::~UDPManagerClient()
{
}
