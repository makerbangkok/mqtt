#include "esp_system.h"
#include "kidbright32.h"
#include "blink.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"

#include <map>
#include <vector>
#include <string>
#include "string.h"

#include <sys/time.h>

using namespace std;

static const char *TAG = "MQTT_SAMPLE";

#define pin 4

#define CONFIG_MQTT_PROTOCOL_311

static EventGroupHandle_t wifi_event_group;
const static int CONNECTED_BIT = BIT0;

static esp_mqtt_client_handle_t mqtt_client;
static bool _mqtt_is_connected;

static std::map<std::string, Event> dict_sub = {};

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;

    //global in header file
    mqtt_client = client;


    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:

            _mqtt_is_connected = true;

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            // ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            _mqtt_is_connected = false;
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:

            //get topic
            char topic[30];
            sprintf(topic,"%.*s", event->topic_len, event->topic);
            //save data to dictionary
            char data[50];
            sprintf(data,"%.*s", event->data_len, event->data);
            dict_sub[topic].data = data;

            //play callback data
            if (dict_sub[topic].callback) {
                dict_sub[topic].callback();
            }


            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
    } 
    return ESP_OK; 
}

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);

            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

static void wifi_init(void)
{
    // tcpip_adapter_init();
    // wifi_event_group = xEventGroupCreate();
    // esp_event_loop_init(wifi_event_handler, NULL);
    // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // esp_err_t err = esp_wifi_init(&cfg);
    // esp_wifi_set_storage(WIFI_STORAGE_RAM);

    // esp_wifi_set_mode(WIFI_MODE_STA);
    // printf("xxx");

    //xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);

    //ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    //ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    /*wifi_config_t wifi_config = { };
strcpy((char*)wifi_config.sta.ssid, "IOT");
strcpy((char*)wifi_config.sta.password, "1234567890");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_LOGI(TAG, "start the WIFI SSID:[%s] password:[%s]", "IOT", "1234567890");
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Waiting for wifi");
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);*/
}

void app_main()
{
    //xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    // nvs_flash_init();
    // wifi_init();
}

// void ultrasonic_test(void *pvParamters)
// {
//     printf("AAAAAAA");

//     ultrasonic_sensor_t sensor;
//     memset(&sensor, 0, sizeof(sensor));

//     sensor.trigger_pin = TRIGGER_GPIO;
//     sensor.echo_pin = ECHO_GPIO;

//     // ultrasonic_sensor_t sensor = {
//     //     .trigger_pin = TRIGGER_GPIO,
//     //     .echo_pin = ECHO_GPIO
//     // };

//     ultrasonic_init(&sensor);

//     printf("HEFKEJHFEOIJFEL:IFJKLEFJE");

//     while (true)
//     {
//         uint32_t distance;
//         esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance);
//         if (res != ESP_OK)
//         {
//             printf("Error: ");
//             switch (res)
//             {
//                 case ESP_ERR_ULTRASONIC_PING:
//                     printf("Cannot ping (device is in invalid state)\n");
//                     break;
//                 case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
//                     printf("Ping timeout (no device found)\n");
//                     break;
//                 case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
//                     printf("Echo timeout (i.e. distance too big)\n");
//                     break;
//                 default:
//                     printf("%d\n", res);
//             }
//         }
//         else
//             printf("Distance: %d cm, %.02f m\n", distance, distance / 100.0);

//         vTaskDelay(500 / portTICK_PERIOD_MS);
//     }
// }

BLINK::BLINK(void) {
	//
}

void BLINK::init(void) {
    // gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT);

    // xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
	//app_main();
	// gpio_config_t io_conf;


	// // outputs init
	// io_conf.intr_type = GPIO_INTR_DISABLE; // disable interrupt
	// io_conf.mode = GPIO_MODE_OUTPUT; // set as output mode
	// io_conf.pin_bit_mask = (1ULL << GPIO_NUM_8); // pin bit mask
	// io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // disable pull-down mode
	// io_conf.pull_up_en = GPIO_PULLUP_DISABLE; // disable pull-up mode
	// blink_status = 1;
	// gpio_set_level(GPIO_NUM_8, 1); // active low
	// gpio_config(&io_conf);

	// blink_flag = false;
	// state = s_detect;

	// printf("xxx");
 //    _mqtt_is_connected = false;
}

int BLINK::prop_count(void) {
	// not supported
	return 0;
}

bool BLINK::prop_name(int index, char *name) {
	// not supported
	return false;
}

bool BLINK::prop_unit(int index, char *unit) {
	// not supported
	return false;
}

bool BLINK::prop_attr(int index, char *attr) {
	// not supported
	return false;
}

bool BLINK::prop_read(int index, char *value) {
	// not supported
	return false;
}

bool BLINK::prop_write(int index, char *value) {
	// not supported
	return false;
}

void BLINK::process(Driver *drv) {
	switch (state) {
		case s_detect:
			// clear error flag
			error = false;
			// set initialized flag
			initialized = true;

			// go to blink state
			state = s_blink;
			break;

		case s_blink:
			if (blink_flag) {
				if (is_tickcnt_elapsed(tickcnt, 500)) {
					blink_status ^= 0x01; // toggle status
					// gpio_set_level(4, false);
					// get current tickcnt
					tickcnt = get_tickcnt();
				}
			}
			break;
	}
}


void BLINK::connect_wifi(char *ssid, char *password) {

    wifi_ssid = ssid;
    wifi_password = password;

    // wifi_config_t wifi_config = { };
    // strcpy((char*)wifi_config.sta.ssid, ssid);
    // strcpy((char*)wifi_config.sta.password, password);
    // esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    // esp_err_t status = esp_wifi_start();
    // printf(status);
    // printf("connected_wifi\n");

}

char* BLINK::ssid(void) {
    return wifi_ssid;
}

char* BLINK::password(void) {
    return wifi_password;
}

void BLINK::mqtt_start(char *ip) {

    mqtt_ip = ip;
    esp_mqtt_client_config_t mqtt_cfg;
    memset(&mqtt_cfg, 0, sizeof(mqtt_cfg));
    mqtt_cfg.uri = ip;
    mqtt_cfg.event_handle = mqtt_event_handler;
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);

    while(_mqtt_is_connected == false) {
        printf("%s\n","connecting mqtt");
        vTaskDelay(10 / portTICK_RATE_MS);
    }

    printf("%s\n","connected");

}

void BLINK::mqtt_pub(char *topic, float data) {
    while(_mqtt_is_connected == false) {
        vTaskDelay(10 / portTICK_RATE_MS);
    }
    char string[8];
    snprintf(string, sizeof(string) - 1, "%f", data);
    esp_mqtt_client_publish(mqtt_client, topic, string, 0, 0, 0);
}

void BLINK::mqtt_sub(char *topic, Callback statement) {
    while(_mqtt_is_connected == false) {
        vTaskDelay(10 / portTICK_RATE_MS);
    }

    Event event;
    event.data = dict_sub[topic].data;
    event.callback = statement;
    dict_sub[topic] = event;

    esp_mqtt_client_subscribe(mqtt_client, topic, 0);

}

bool BLINK::mqtt_is_connected(void) {
    return _mqtt_is_connected;
}

char* BLINK::subscribe_data(char *topic) {
    char *data = "NULL";
    if (dict_sub[topic].data) {
        data = dict_sub[topic].data;
    }
    return data;
}
