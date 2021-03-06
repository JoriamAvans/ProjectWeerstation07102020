/*
Documents used for programming:
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#how-to-write-a-wi-fi-application

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#esp32-wi-fi-ap-general-scenario

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_client.html

http://httpbin.org/
*/

extern "C" void app_main();

#include "nvs_flash.h"
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "http_Client.h"
#include "espWiFiConnect.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "sensor.h"
void init_NVS();

void init_esp();
void config_esp();
void connect_esp();

std::string temperatureValue;
std::string humidityValue;
EventGroupHandle_t eventgroupvariable;
espWiFiConnect wificonnect;
http_Client http;
sensor Sensor;

void app_main(void)
{
    //endless loop killer

    init_NVS();

    // Init Phase
    wificonnect.init_WiFi();
    // Configuration Phase
    wificonnect.config_WiFi("ESP", "Welkom@67");
    // Connection Phase
    wificonnect.conn_WiFi();

    //Sensor.sensorRead(humidityValue, temperatureValue);
    const char *humValChar = "25.30";//humidityValue.c_str();
    const char *tempValChar = "25.30";//temperatureValue.c_str();
    const char *strurl = "http://173.249.25.181/weatherStation/measurement.php?humidity=%s&temperature=%s";
    char buffer[256];
    sprintf(buffer, strurl, humValChar, tempValChar);
    http.http_handle(buffer);

    //http.http_handle("http://173.249.25.181/weatherStation/measurement.php?humidity=20&temperature=30");
}

void init_NVS()
{
    // Initialize non-volatile storage
    esp_err_t nvsflash = nvs_flash_init();
    if (nvsflash == (ESP_ERR_NVS_NO_FREE_PAGES || ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        nvsflash = nvs_flash_init();
    }
    ESP_ERROR_CHECK(nvsflash);
}
