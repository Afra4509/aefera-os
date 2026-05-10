#include <stdio.h>
#include <string.h>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "kernel.h"
#include "ui_core.h"
#include "terminal_app.h"

static const char *TAG = "AeferaOS_Main";

using namespace Aefera;

// Terminal task - reads UART0 directly (proper ESP8266 method)
void terminalTask(void *pvParameters) {
    Apps::Terminal terminal;
    terminal.start();

    static char line[128];
    int idx = 0;

    while(1) {
        uint8_t c = 0;
        if (uart_read_bytes(UART_NUM_0, &c, 1, pdMS_TO_TICKS(20)) > 0) {
            if (c == '\n' || c == '\r') {
                if (idx > 0) {
                    line[idx] = '\0';
                    printf("\n");
                    terminal.processInput(std::string(line));
                    idx = 0;
                }
            } else if (c == 127 || c == 8) { // backspace
                if (idx > 0) {
                    idx--;
                    printf("\b \b");
                }
            } else if (idx < 127) {
                line[idx++] = c;
                printf("%c", c); // echo char
            }
        }
    }
}

// UI Task
void uiTask(void *pvParameters) {
    UI::WindowManager::getInstance().showBootAnimation();
    UI::WindowManager::getInstance().renderDesktop();
    
    while(1) {
        UI::WindowManager::getInstance().update();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "=========================================");
    ESP_LOGI(TAG, "   BOOTING AEFERA OS v1.0.0 (ESP8266)    ");
    ESP_LOGI(TAG, "=========================================");

    // Configure UART for terminal input
    uart_config_t uart_config;
    uart_config.baud_rate  = 74880;
    uart_config.data_bits  = UART_DATA_8_BITS;
    uart_config.parity     = UART_PARITY_DISABLE;
    uart_config.stop_bits  = UART_STOP_BITS_1;
    uart_config.flow_ctrl  = UART_HW_FLOWCTRL_DISABLE;
    uart_config.rx_flow_ctrl_thresh = 122;
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);

    // 1. Initialize Kernel / Task Scheduler
    Kernel::TaskManager::getInstance().init();

    // 2. Initialize UI Window Manager
    UI::WindowManager::getInstance().init();

    // 3. Create OS Processes
    Kernel::TaskManager::getInstance().createProcess(
        "ui_renderer", 
        uiTask, 
        NULL, 
        4096, 
        5
    );

    Kernel::TaskManager::getInstance().createProcess(
        "terminal_shell", 
        terminalTask, 
        NULL, 
        4096, 
        3
    );

    // Initial system report
    Kernel::TaskManager::getInstance().printSysInfo();

    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#include <pthread.h>
extern "C" {
    int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr) { return 0; }
    int pthread_cond_broadcast(pthread_cond_t *cond) { return 0; }
    int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) { return 0; }
}
