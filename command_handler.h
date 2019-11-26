#pragma once
#include "observer.h"
#include <queue>
#include <vector>
#include <utility>
#include <chrono>

enum class cmdState{
    regular = 0,
    nested
};

class QueueCommand{
public:
    QueueCommand():block_size_{100} {};
    QueueCommand(size_t block_size):block_size_{block_size} {}

    QueueCommand(const QueueCommand&)   = default;
    QueueCommand(QueueCommand&&)        = default;
    ~QueueCommand()                     = default;

    void addCommand(std::string cmd);
    void subscribe(std::unique_ptr<Observer>&& obs);
    void notify();
private:
	std::string getUnixTime();
private:
    std::vector<std::unique_ptr<Observer>> subs;
    std::queue<std::pair<std::string, std::string>> commands;
    size_t block_size_;
};

class CommandHandler{
public:
	CommandHandler(std::unique_ptr<QueueCommand> qCmd):
        queueCmd_(std::move(qCmd)),state( (size_t)cmdState::regular) {}
	~CommandHandler();
	void process(std::string&& cmd);
	void Run(std::istream& is);

private:
	bool isNested() {
		return state == (size_t)cmdState::nested;
	}

private:
	std::unique_ptr<QueueCommand> queueCmd_;
	std::string braces;
	size_t state;
};

