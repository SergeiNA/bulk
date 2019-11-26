#define BOOST_TEST_MODULE regular_commnds

#include "observer.h"
#include "command_handler.h"
#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(bulk_suite)

BOOST_AUTO_TEST_CASE(test_terminal_1)
{
    std::string ans = R"(bulk: 1
bulk: 2
bulk: 3
)";

    std::istringstream iss{R"(1
{
2
}
3)"};

    std::ostringstream oss;
    auto q_command = std::make_unique<QueueCommand>(5);
    q_command->subscribe(std::make_unique<terminal_observer>(oss));
    CommandHandler cmdHandler(std::move(q_command));
    cmdHandler.Run(iss);
    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_terminal_2)
{
    std::string ans = R"(bulk: 1, 2
bulk: 3
)";

   std::istringstream iss{R"(1
2
{
3
})"};

    std::ostringstream oss;
    auto q_command = std::make_unique<QueueCommand>(5);
    q_command->subscribe(std::make_unique<terminal_observer>(oss));
    CommandHandler cmdHandler(std::move(q_command));
    cmdHandler.Run(iss);

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_terminal_3)
{
    std::string ans = R"(bulk: 1, 2, 3
)";

   std::istringstream iss{R"(1
2
3
{)"};

    std::ostringstream oss;

    auto q_command = std::make_unique<QueueCommand>(5);
    q_command->subscribe(std::make_unique<terminal_observer>(oss));
    CommandHandler cmdHandler(std::move(q_command));
    cmdHandler.Run(iss);

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_terminal_4)
{
    std::string ans = R"(bulk: 1, 2, 3
)";

   std::istringstream iss{R"(1
2
3
{
4)"};

    std::ostringstream oss;

    auto q_command = std::make_unique<QueueCommand>(5);
    q_command->subscribe(std::make_unique<terminal_observer>(oss));
    CommandHandler cmdHandler(std::move(q_command));
    cmdHandler.Run(iss);

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_terminal_5)
{
    std::string ans = R"(bulk: 1
bulk: 2, 3, 4
)";

   std::istringstream iss{R"(1
{
2
{
3
}
4
})"};

    std::ostringstream oss;

    auto q_command = std::make_unique<QueueCommand>(5);
    q_command->subscribe(std::make_unique<terminal_observer>(oss));
    CommandHandler cmdHandler(std::move(q_command));
    cmdHandler.Run(iss);

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_SUITE_END()