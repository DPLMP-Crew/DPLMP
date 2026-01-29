#include "Server.h"
#include <enet.h>
#include <stdexcept>

Server::Server() : 
	m_Logger("SERVER") 
{
	if (enet_initialize() == 0) {
		m_Logger.Log(LogType::INFO, "ENet initialized!");
	}
	else
	{
		throw std::runtime_error("Failed to initialize ENet");
	}
}

void Server::Step() {

}

Server::~Server() {
	enet_deinitialize();
}