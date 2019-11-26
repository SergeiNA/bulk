#include <iostream>
#include <fstream>
#include <memory>

#include "observer.h"
#include "command_handler.h"

int main([[maybe_unused]]int argc, char* argv[]){
    auto q_command = std::make_unique<QueueCommand>(atoi(argv[1]));
    q_command->subscribe(std::make_unique<terminal_observer>());
    q_command->subscribe(std::make_unique<log_observer>());
	CommandHandler cmdHandler(std::move(q_command));
	std::ifstream ifile("cmds.txt");
	cmdHandler.Run(std::cin);
    return 0;
}