#pragma once

#include <string>

namespace Aefera {
namespace Apps {

    class Terminal {
    public:
        Terminal();
        void start();
        void processInput(const std::string& input);

    private:
        void printPrompt();
        void handleCommand(const std::string& cmd);
        void cmdHelp();
        void cmdPs();
        void cmdSysInfo();
        void cmdAscii();
        void cmdMatrix();
    };

} // namespace Apps
} // namespace Aefera
