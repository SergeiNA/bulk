/**
 * @file observer.h
 * @author SergeiNA (you@domain.com)
 * @brief observer pattern implementation
 * 
 * log_observer - logging packages info to files
 * terminal_observer - display packages to stdout
 * create - initialize file or display "bulk"
 * end - close file or add endl
 * bukl - dump package of commands
 * @version 1.0
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * @brief Observer interface
 * 
 * Usage: create/fill/end logs
 */
class Observer {
public:
//string_view
    virtual void bulk(const std::vector<std::string>&) = 0;
    virtual void create(const std::string&)=0;
    virtual void end()=0;
    virtual ~Observer() =default;
};
/**
 * @brief Display packages to stdout
 * 
 * ex: bulk: cmd1,cmd2
 *     bulk: cmd3
 */
class terminal_observer : public Observer {
public:
    terminal_observer(std::ostream& ofs = std::cout);
    /**
     * @brief display bulk: to stdout
     * 
     */
    // string_view
    void create([[maybe_unused]]const std::string&) override;
    void end() override;
    /**
     * @brief display command to stdout
     * 
     * @param cmd command to display
     */
    // string_view
    void bulk(const std::vector<std::string>& cmd) override;
private:
    bool isBegin; ///< indicate start of printing to stdout
    std::ostream& os;
};


/**
 * @brief Create and wtite to file package of commands
 * name files as bulk<unixtime>.log
 */
class log_observer : public Observer {
public:
    log_observer();
    /**
     * @brief Create log file 
     * 
     * @param t unix time in miliseconds
     */
    void create(const std::string& t) override;
    void end() override;
    /**
     * @brief write commands to file
     * 
     * @param cmd written command
     */
    // string_view
    void bulk(const std::vector<std::string>& cmd) override;
private:
    std::ofstream File_;
};


