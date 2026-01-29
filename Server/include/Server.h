#pragma once
#include "Logger.h"

class Server {
public:
	Server();
	~Server();
	void Step();

private:
	Logger m_Logger;
};