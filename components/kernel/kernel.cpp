#include "kernel.h"
#include "esp_system.h"
// esp_heap_caps.h is typically ESP32 specific, ESP8266 just uses esp_get_free_heap_size()

namespace Aefera {
namespace Kernel {

    static const char* TAG = "AeferaKernel";

    TaskManager* taskManagerInstance = nullptr;
    TaskManager& TaskManager::getInstance() {
        if (!taskManagerInstance) {
            taskManagerInstance = new TaskManager();
        }
        return *taskManagerInstance;
    }

    TaskManager::TaskManager() {
    }

    void TaskManager::init() {
        ESP_LOGI(TAG, "Initializing Aefera Kernel (ESP8266)...");
        // Start system monitor task with smaller stack
        xTaskCreate(systemMonitorTask, "sys_monitor", 2048, this, 1, NULL);
        ESP_LOGI(TAG, "Kernel Initialized.");
    }

    // Single-core process creation (xTaskCreate instead of xTaskCreatePinnedToCore)
    uint32_t TaskManager::createProcess(const std::string& name, TaskFunction_t task, void* params, uint32_t stackDepth, UBaseType_t priority) {
        Process p;
        p.pid = next_pid++;
        p.name = name;
        p.state = ProcessState::READY;
        p.cpu_usage = 0;
        p.memory_usage = stackDepth;

        xTaskCreate(task, name.c_str(), stackDepth, params, priority, &p.handle);
        
        if (p.handle != NULL) {
            processes.push_back(p);
            ESP_LOGI(TAG, "Process created: %s (PID: %lu)", name.c_str(), (unsigned long)p.pid);
            return p.pid;
        } else {
            ESP_LOGE(TAG, "Failed to create process: %s (Memory full?)", name.c_str());
            return 0;
        }
    }

    void TaskManager::killProcess(uint32_t pid) {
        for (auto it = processes.begin(); it != processes.end(); ++it) {
            if (it->pid == pid) {
                vTaskDelete(it->handle);
                ESP_LOGI(TAG, "Process killed: %s (PID: %lu)", it->name.c_str(), (unsigned long)pid);
                processes.erase(it);
                return;
            }
        }
        ESP_LOGW(TAG, "Process not found (PID: %lu)", (unsigned long)pid);
    }

    std::vector<Process> TaskManager::getProcessList() {
        return processes;
    }

    void TaskManager::printSysInfo() {
        uint32_t free_heap = esp_get_free_heap_size();
        ESP_LOGI(TAG, "--- System Info ---");
        ESP_LOGI(TAG, "Free Heap: %lu bytes", (unsigned long)free_heap);
        ESP_LOGI(TAG, "Active Processes: %d", (int)processes.size());
    }

    void TaskManager::systemMonitorTask(void* pvParameters) {
        while (true) {
            // Simulated monitoring task
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }

} // namespace Kernel
} // namespace Aefera
