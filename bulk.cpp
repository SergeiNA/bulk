#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <memory>


enum class cmdState{
    regular = 0,
    nested
};

class Observer {
public:
    virtual void bulk(std::string) = 0;
    virtual void create()=0;
    virtual void end()=0;
    virtual ~Observer() =default;
};

class QueueCommand{
public:
    QueueCommand():block_size_{2} {};
    QueueCommand(size_t block_size):block_size_{block_size} {}

    QueueCommand(const QueueCommand&) = default;
    QueueCommand(QueueCommand&&)=default;
    ~QueueCommand(){
        notify();
        for(auto&& s:subs)
            s.release();
    }

    void addCommand(const std::string cmd){
        commands.push(cmd);
        if (commands.size() == block_size_){
            notify();
        }
    }

    void subscribe(std::unique_ptr<Observer>&& obs) {
        subs.push_back(std::move(obs));
    }

    void notify() {
        for (size_t i = 0; i < subs.size(); i++)
            subs[i]->create();
        while(!commands.empty()){
            for (size_t i = 0; i < subs.size(); i++)
                subs[i]->bulk(commands.front());
            commands.pop();
        }
        for (size_t i = 0; i < subs.size(); i++)
            subs[i]->end();

    }


private:
    std::vector<std::unique_ptr<Observer>> subs;
    std::queue<std::string> commands;
    size_t block_size_;
};


class ui_observer : public Observer {
public:
    ui_observer(std::shared_ptr<QueueCommand> qcmd):isBegin(true) {
        qcmd.get()->subscribe(std::unique_ptr<Observer>(this));
    }

    void create() override{
        isBegin =true;
        std::cout << "UI bulk: ";
    }

    void end() override{
        std::cout << std::endl;
    }


    void bulk(std::string cmd) override {
        if(!isBegin)
            std::cout<<", ";
        isBegin =false;
        std::cout<<cmd;
    }

private:
    bool isBegin;
};

class log_observer : public Observer {
public:
    log_observer(std::shared_ptr<QueueCommand> qcmd):isBegin(true) {
        qcmd.get()->subscribe(std::unique_ptr<Observer>(this));
    }
    void create() override {
        isBegin =true;
        std::cout << "LOG bulk: ";
    }

    void end() override{
        std::cout << std::endl;
    }

    void bulk(std::string cmd) override {
        if(!isBegin)
            std::cout<<", ";
        isBegin =false;
        std::cout<<cmd;
    }
private:
    bool isBegin;
};

int main(){
    auto q_cmd_ptr = std::make_shared<QueueCommand>();
    ui_observer ui_obs_ptr(q_cmd_ptr);
    //log_observer lg_obs(q_cmd_ptr);
    for (size_t i = 0; i < 11; i++)
    {
        q_cmd_ptr->addCommand("cmd"+ std::to_string(i));
    }
    return 0;

}