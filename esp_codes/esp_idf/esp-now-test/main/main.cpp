extern "C" {
    #include "esp_now.h"
    #include "esp_wifi.h"
    #include "esp_log.h"
    #include "nvs_flash.h"
    #include "esp_event.h"
    #include "esp_mac.h"
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
}
#include <cstring>

static const char* TAG = "ESP_NOW_BROADCAST";

// Define message structure
typedef struct struct_message {
    int id;
    float temperature;
} __attribute__((packed)) struct_message;

struct_message msg = {1, 23.5f};

// Broadcast MAC (special address)
uint8_t broadcast_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Callback: when data is sent
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    ESP_LOGI(TAG, "Broadcast sent -> %s", status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Callback: when data is received
void on_data_recv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    struct_message incoming;
    if (len == sizeof(incoming)) {
        memcpy(&incoming, data, len);

        const uint8_t *mac = recv_info->src_addr;
        ESP_LOGI(TAG, "Received from %02X:%02X:%02X:%02X:%02X:%02X -> ID: %d, Temp: %.2f",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                 incoming.id, incoming.temperature);
    } else {
        ESP_LOGW(TAG, "Unexpected data size: %d", len);
    }
}


extern "C" void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Set Wi-Fi channel to 1 (or the channel your receiver listens to)
    ESP_ERROR_CHECK(esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(on_data_sent));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(on_data_recv));

    // Add broadcast peer explicitly (for compatibility)
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcast_mac, 6);
    peerInfo.channel = 1;  // same channel as set above
    peerInfo.encrypt = false;
    esp_err_t ret = esp_now_add_peer(&peerInfo);
    if (ret == ESP_ERR_ESPNOW_EXIST) {
        ESP_LOGI(TAG, "Broadcast peer already added");
    } else if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add broadcast peer: %d", ret);
        return;
    }

    ESP_LOGI(TAG, "ESP-NOW initialized in broadcast mode");

    while (true) {
        msg.temperature += 0.1f;
        esp_err_t send_ret = esp_now_send(broadcast_mac, (uint8_t*)&msg, sizeof(msg));
        if (send_ret != ESP_OK) {
            ESP_LOGE(TAG, "Send failed: %d", send_ret);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
