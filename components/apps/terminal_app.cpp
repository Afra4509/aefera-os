#include "terminal_app.h"
#include "kernel.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace Aefera {
namespace Apps {

    static const char* TAG = "Terminal";

    Terminal::Terminal() {}

    void Terminal::start() {
        ESP_LOGI(TAG, "Starting Terminal...");
        printPrompt();
    }

    void Terminal::printPrompt() {
        printf("\n\033[1;32mroot@aefera\033[0m:\033[1;34m~#\033[0m ");
    }

    void Terminal::processInput(const std::string& input) {
        if (!input.empty()) {
            handleCommand(input);
        }
        printPrompt();
    }

    void Terminal::handleCommand(const std::string& cmd) {
        if (cmd == "help") {
            cmdHelp();
        } else if (cmd == "ps" || cmd == "top") {
            cmdPs();
        } else if (cmd == "sysinfo" || cmd == "neofetch") {
            cmdSysInfo();
        } else if (cmd == "ascii") {
            cmdAscii();
        } else if (cmd == "matrix") {
            cmdMatrix();
        } else if (cmd == "clear") {
            printf("\033[2J\033[H");
        } else {
            printf("command not found: %s\n", cmd.c_str());
        }
    }

    void Terminal::cmdHelp() {
        printf("\nAefera OS Shell v1.0.0 (ESP8266 Edition)\n");
        printf("Available commands:\n");
        printf("  help     - Show this menu\n");
        printf("  clear    - Clear screen\n");
        printf("  ps       - List running processes\n");
        printf("  sysinfo  - System information (neofetch style)\n");
        printf("  ascii    - Show Aefera logo\n");
        printf("  matrix   - Matrix rain simulation\n");
    }

    void Terminal::cmdPs() {
        printf("\nPID\tNAME\t\tSTATE\tCPU%%\tMEM(B)\n");
        printf("--------------------------------------------------\n");
        auto procs = Kernel::TaskManager::getInstance().getProcessList();
        for (const auto& p : procs) {
            printf("%lu\t%s\t\tREADY\t%lu\t%lu\n", (unsigned long)p.pid, p.name.c_str(), (unsigned long)p.cpu_usage, (unsigned long)p.memory_usage);
        }
        printf("\n");
    }

    void Terminal::cmdSysInfo() {
        printf("\n\033[1;36m");
        printf("    /\\    \033[0m OS: Aefera OS 1.0 (ESP8266)\n\033[1;36m");
        printf("   /  \\   \033[0m Kernel: Aefera RTK 1.0 (Single-Core)\n\033[1;36m");
        printf("  /____\\  \033[0m Uptime: %lu ms\n\033[1;36m", (unsigned long)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        printf(" /      \\ \033[0m CPU: Tensilica L106 80/160MHz\n\033[1;36m");
        printf("/________\\\033[0m Theme: Cyberpunk Dark\n");
        printf("\n");
        Kernel::TaskManager::getInstance().printSysInfo();
    }

    void Terminal::cmdAscii() {
        printf("\033[1;35m");
        printf("\n");
        printf("   ###    ######## ######## ######## ########     ###       #######   ######  \n");
        printf("  ## ##   ##       ##       ##       ##     ##   ## ##     ##     ## ##    ## \n");
        printf(" ##   ##  ##       ##       ##       ##     ##  ##   ##    ##     ## ##       \n");
        printf("##     ## ######   ######   ######   ########  ##     ##   ##     ##  ######  \n");
        printf("######### ##       ##       ##       ##   ##   #########   ##     ##       ## \n");
        printf("##     ## ##       ##       ##       ##    ##  ##     ##   ##     ## ##    ## \n");
        printf("##     ## ######## ##       ######## ##     ## ##     ##    #######   ######  \n");
        printf("\033[0m\n");
    }

    void Terminal::cmdMatrix() {
        printf("\033[1;32m");
        for(int i=0; i<20; ++i) {
            printf("101001 0101 110 00101010 110101 0011 01\n");
            printf(" 100 11010 011100 101 00101 111001 010 \n");
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        printf("\033[0m");
    }

} // namespace Apps
} // namespace Aefera
