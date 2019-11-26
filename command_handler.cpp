/**
 * @file command_handler.cpp
 * @author SergeiNA (you@domain.com)
 * @brief Method definitoin of QueueCommand and CommandHandler
 * @version 1.0
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "command_handler.h"

void QueueCommand::addCommand(std::string cmd)
{
    commands.push(std::make_pair(std::move(cmd), getUnixTime()));
    if (commands.size() == block_size_)
    {
        notify();
    }
}

void QueueCommand::subscribe(std::unique_ptr<Observer> &&obs)
{
    subs.emplace_back(std::move(obs));
}
/**
 * @brief Sent cmd message to all subscribers
 * 
 * First call initiate function (create)
 * then send all commands in the queue
 * until it becomes empty
 * At the end call ending function (end)
 * 
 * Note: If command queue is empty 
 * "queuery is empty" - will be send as command
 * and "empty" as a timestamp
 * 
 */
void QueueCommand::notify()
{
    if(commands.empty())
        commands.emplace("queuery is empty","empty");
    for (size_t i = 0; i < subs.size(); i++)
        subs[i]->create(commands.front().second);

    while (!commands.empty())
    {
        for (size_t i = 0; i < subs.size(); i++)
            subs[i]->bulk(commands.front().first);
        commands.pop();
    }
    for (size_t i = 0; i < subs.size(); i++)
        subs[i]->end();
}

std::string QueueCommand::getUnixTime()
{
    using namespace std::chrono;
    return std::to_string(duration_cast<milliseconds>
        (system_clock::now().time_since_epoch()).count());
}

/**
 * @brief handle all type of input commands
 * 
 * If process get block type command ('{' or '}')
 * depends on current state It will operate differently
 * 
 * - Current state is 'regular':
 *      get usual command -> call addCommand for current command
 *      get '{' block start indicator -> call notify, switch state to 
 *          'nested' and  increment braces_count
 *      get '}' block end indicator -> throw exception
 * 
 * - Current state is 'nested':
 *      get usual command -> call addCommand for current command
 *      get '{' block start indicator -> increment braces_count
 *      get '}' block end indicator -> decrement braces_count
 *          if braces_count == 0 -> switch state to regular and call notify
 * 
 * 
 * @param cmd input command
 */

void CommandHandler::process(std::string &&cmd)
{
    if (cmd == "{")
    {
        if (!isNested())
        {
            state = (size_t)cmdState::nested;
            queueCmd_->notify();
        }
        ++braces_count;
    }
    else if (cmd == "}")
    {
        if (!braces_count)
            throw std::invalid_argument("} can not be first bracket");
        --braces_count;
        if (!braces_count)
        {
            state = (size_t)cmdState::regular;
            queueCmd_->notify();
        }
    }
    else
        queueCmd_->addCommand(cmd);
}

/**
 * @brief Read all commands from stdin and call process on them
 * 
 * @param is input stdin stream
 */
void CommandHandler::Run(std::istream &is = std::cin)
{
    while (is && !is.eof())
    {
        std::string temp;
        std::getline(is, temp);
        process(std::move(temp));
    }
    dumpRemains();
}

void CommandHandler::dumpRemains(){
    if (!isNested()&&!queueCmd_->empty())
        queueCmd_->notify();
}