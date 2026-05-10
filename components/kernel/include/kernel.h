#pragma once

#include <string>
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

namespace Aefera {
namespace Kernel {

    enum class ProcessState {
        READY,
        RUNNING,
        BLOCKED,
        TERMINATED
    };

    struct Process {
        uint32_t pid;
        std::string name;
        TaskHandle_t handle;
        ProcessState state;
        uint32_t cpu_usage;
        uint32_t memory_usage;
    };

    class TaskManager {
    public:
        static TaskManager& getInstance();
        
        void init();
        // Removed coreId since ESP8266 is single-core
        uint32_t createProcess(const std::string& name, TaskFunction_t task, void* params, uint32_t stackDepth, UBaseType_t priority);
        void killProcess(uint32_t pid);
        std::vector<Process> getProcessList();
        
        void printSysInfo();
        
    private:
        TaskManager();
        ~TaskManager() = default;

        std::vector<Process> processes;
        uint32_t next_pid = 1;
        
        static void systemMonitorTask(void* pvParameters);
    };

} // namespace Kernel
} // namespace Aefera
