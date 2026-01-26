#include "Server.h"
#include <memory>

int main() {
	std::unique_ptr<Server> gameServer = std::make_unique<Server>();
	while (true) {
		gameServer->Step();
	}
	return 0;
}