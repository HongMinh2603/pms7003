#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "nvs_flash.h"   // Add this header
#include "esp_log.h"
#include "pms7003.h"

static const char *TAG = "main";

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the PMS7003 sensor
    if (pms7003_init(UART_NUM_1) == ESP_OK) {
        pms7003_data_t data;
        while (1) {
            if (pms7003_read_data(UART_NUM_1, &data) == ESP_OK) {
                ESP_LOGI(TAG, "PM1.0: %d, PM2.5: %d, PM10: %d", data.pm1_0, data.pm2_5, data.pm10);
            } else {
                ESP_LOGE(TAG, "Failed to read data from PMS7003");
            }
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    } else {
        ESP_LOGE(TAG, "Failed to initialize PMS7003");
    }
}
