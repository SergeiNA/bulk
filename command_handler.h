/**
 * @file command_handler.h
 * @author SergeiNA (you@domain.com)
 * @brief Declaration of commands container and commands handler
 * QueueCommand is able to bulk commands if condition reached
 * CommandHandler handle on blocks {...}
 * @version 1.0
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include "observer.h"
#include <vector>
#include <utility>
#include <chrono>
/// Possible commands states
enum class cmdState{
    regular = 0, ///< not in a block {...}
    nested ///< nested in block {...
};
/**
 * @brief Contains commands, if limit is reached notify subscribers
 * Subscribers have to be inhareted from Observer
 */
class QueueCommand{
public:
    QueueCommand():block_size_{100} {};
    QueueCommand(size_t block_size):block_size_{block_size},nested(false) {}

    QueueCommand(const QueueCommand&)   = default;
    QueueCommand(QueueCommand&&)        = default;
    ~QueueCommand()                     = default;
    /**
     * @brief Add pair of command and timestamp in unixtime to container
     * 
     * @param cmd added command
     */
    void addCommand(std::string cmd);
    /**
     * @brief Add subscriber as observer
     * 
     * @param obs 
     */
    void subscribe(std::unique_ptr<Observer>&& obs);
    /**
     * @brief Notify all subscribers to handle package of commands
     * 
     */
    void notify();
    void set_nested(bool nested_);
    bool empty(){
        return commands.empty();
    }
private:
/**
 * @brief Return timestamp of each command in unixtime
 * 
 * @return std::string returned timestamp
 */
	std::string getUnixTime();

private:
    std::vector<std::unique_ptr<Observer>> subs; ///< contains subscribers
    std::vector<std::string> commands; ///< contsains commands
    std::vector<std::string> timestamps; ///< contsains timestamps
    size_t block_size_;///< max free block size (if not in block)
    bool nested;///< indicate if commands in block
};

/**
 * @brief Handle on all input commands
 * 
 * Able to handle with blocks:
 * {
 * cmd1
 * cmd2
 * {
 * cmd3
 * }
 * }
 * 
 * Throw exception if no '{' matches for '}'
 */
class CommandHandler{
public:
	CommandHandler(std::unique_ptr<QueueCommand> qCmd):
        queueCmd_(std::move(qCmd)),state( (size_t)cmdState::regular),braces_count{0} {}
	~CommandHandler() = default;
    /**
     * @brief handle on input command
     * 
     * @param cmd input command
     */
	void process(std::string&& cmd);
    /**
     * @brief Start to read commands form input
     * 
     * @param is input stream
     */
	void Run(std::istream& is);

private:
/**
 * @brief Check if current state Nested
 * 
 * @return true 
 * @return false 
 */
	bool isNested() {
		return state == (size_t)cmdState::nested;
	}
/**
 * @brief If in regular state dump all remaind commands in queueCmd_
 * 
 */
    void dumpRemains();
private:
	std::unique_ptr<QueueCommand> queueCmd_; ///< pointer to QueueCommand object
	size_t state; ///< contain current state
    size_t braces_count; ///< contain current degree of nesting
};

