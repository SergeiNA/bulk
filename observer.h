#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
class Observer {
public:
    virtual void bulk(std::string_view) = 0;
    virtual void create(std::string_view)=0;
    virtual void end()=0;
    virtual ~Observer() =default;
};

class terminal_observer : public Observer {
public:
    terminal_observer(std::ostream& ofs = std::cout);
    void create([[maybe_unused]]std::string_view) override;
    void end() override;
    void bulk(std::string_view cmd) override;
private:
    bool isBegin;
    std::ostream& os;
};



class log_observer : public Observer {
public:
    log_observer();
    void create(std::string_view t) override;
    void end() override;
    void bulk(std::string_view cmd) override;
private:
    std::ofstream File_;
};


