#include "observer.h"
#include <fstream>
#include <iostream>

terminal_observer::terminal_observer(std::ostream& ofs):
    isBegin(true),os(ofs) {}

void terminal_observer::create([[maybe_unused]]std::string_view) {
    isBegin = true;
    os << "bulk: ";
}

void terminal_observer::end() {
    os << std::endl;
}

void terminal_observer::bulk(std::string_view cmd)  {
    if(!isBegin)
        os << ", ";
    isBegin =false;
    os << cmd;
}

log_observer::log_observer() {}
void log_observer::create(std::string_view t){
    std::ofstream toFile("bulk" + std::string(t) + ".log");
    File_ = std::move(toFile);
}

void log_observer::end(){
    File_.close();
}

void log_observer::bulk(std::string_view cmd){
    File_ << cmd << std::endl;
}