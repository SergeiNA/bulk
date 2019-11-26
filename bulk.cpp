/**
 * @file bulk.cpp
 * @author SergeiNA (you@domain.com)
 * @brief OTUS homework number seven
 * 
 * package handler
 * @version 1.0
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "observer.h"
#include "command_handler.h"

int main([[maybe_unused]]int argc, char* argv[]){
    auto q_command = std::make_unique<QueueCommand>(atoi(argv[1]));
    q_command->subscribe(std::make_unique<terminal_observer>());
    q_command->subscribe(std::make_unique<log_observer>());
	CommandHandler cmdHandler(std::move(q_command));
	cmdHandler.Run(std::cin);
    return 0;
}