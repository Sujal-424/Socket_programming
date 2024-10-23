#ifndef CLIENT_CLI_HPP_INCLUDE
#define CLIENT_CLI_HPP_INCLUDE

#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/detail/concurrentqueue.h>
#include <cli/loopscheduler.h>

#include <string>
#include <vector>

#include "hotel_client.hpp"

class ClientCLI {
public:
    ClientCLI(HotelClient& client);
    ~ClientCLI();

    void run();

private:
    HotelClient& client_;
    cli::Cli* cli_;
    cli::CliLocalSession* session_;
    std::vector<cli::CmdHandler> loginMenu_;
    std::vector<cli::CmdHandler> userMenu_;
    concurrent::Queue<std::pair<cli::detail::KeyType, char>>* inputQueue_;

    void setupCLI();
    std::unique_ptr<cli::Menu> createMainMenu();
    std::unique_ptr<cli::Menu> createBookMenu(const std::string& parentName);
    std::unique_ptr<cli::Menu> createCancelMenu(const std::string& parentName);
    std::unique_ptr<cli::Menu> createPassDayMenu(const std::string& parentName);
    std::unique_ptr<cli::Menu> createLeaveRoomMenu(const std::string& parentName);
    std::unique_ptr<cli::Menu> createRoomsMenu(const std::string& parentName);

    bool getIntInput(std::ostream& out, const std::string& inputMsg, int& input);
    std::string getInput(std::ostream& out, const std::string& inputMsg, bool hidden = false, char mask = '*');
    void checkMainMenuItems();
    bool inputPassword(std::ostream& out, std::string& password, bool check = true);
};

#endif // CLIENT_CLI_HPP_INCLUDE
