#include "Server.h"
#include <enet.h>

Server::Server() {
	enet_initialize();
}

Server::~Server() {
	enet_deinitialize();
}

void Server::Step() {

}