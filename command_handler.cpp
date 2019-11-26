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


void CommandHandler::process(std::string &&cmd)
{
    if (cmd == "{")
    {
        if (!isNested())
        {
            state = (size_t)cmdState::nested;
            queueCmd_->notify();
        }
        braces.push_back('{');
    }
    else if (cmd == "}")
    {
        if (cmd.empty())
            throw std::invalid_argument("} can not be first bracket");
        braces.pop_back();
        if (braces.empty())
        {
            state = (size_t)cmdState::regular;
            queueCmd_->notify();
        }
    }
    else
        queueCmd_->addCommand(cmd);
}

CommandHandler::~CommandHandler()
{
    if (!isNested())
        queueCmd_->notify();
}

void CommandHandler::Run(std::istream &is = std::cin)
{
    while (is && !is.eof())
    {
        std::string temp;
        std::getline(is, temp);
        process(std::move(temp));
    }
}