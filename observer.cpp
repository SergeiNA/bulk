/**
 * @file observer.cpp
 * @author SergeiNA (you@domain.com)
 * @brief log_observer and terminal_observer method definition
 * @version 1.0
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "observer.h"

terminal_observer::terminal_observer(std::ostream& ofs):
    isBegin(true),os(ofs) {}
//string_view
void terminal_observer::create([[maybe_unused]]const std::string&) {
    isBegin = true;
    os << "bulk: ";
}

void terminal_observer::end() {
    os << std::endl;
}
//string_view
void terminal_observer::bulk(const std::vector<std::string>& cmd)  {
    for(const auto& c:cmd){
        if (!isBegin)
            os << ", ";
        isBegin = false;
        os << c;
    }
}

log_observer::log_observer() {}
// string_view
void log_observer::create(const std::string& t){
    std::ofstream toFile("bulk" + t + ".log");
    File_ = std::move(toFile);
}

void log_observer::end(){
    File_.close();
}
//string_view
void log_observer::bulk(const std::vector<std::string>& cmd){
    for(const auto& c:cmd)
        File_ << c << std::endl;
}
