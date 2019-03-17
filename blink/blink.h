#ifndef __BLINK_H__
#define __BLINK_H__

#include "driver.h"
#include "device.h"
#include "driver/gpio.h"
#include "mqtt_client.h"

typedef void(*Callback)();

typedef struct Event {
    Callback callback;
    char *data;
};

class BLINK : public Device {
	private:
		enum {
			s_detect, s_blink
		} state;
		int blink_status;
		bool blink_flag;
		char *wifi_ssid, *wifi_password, *mqtt_ip;

	public:
		TickType_t tickcnt;
		// constructor
		BLINK(void);
		// override
		void init(void);
		void process(Driver *drv);
		int prop_count(void);
		bool prop_name(int index, char *name);
		bool prop_unit(int index, char *unit);
		bool prop_attr(int index, char *attr);
		bool prop_read(int index, char *value);
		bool prop_write(int index, char *value);
		// method
		void connect_wifi(char *ssid, char *password);
		void stop(void);
		char* ssid(void);
		char* password(void);
		void mqtt_start(char *ip);
		void mqtt_pub(char *topic, float data);
		void mqtt_sub(char *topic, Callback statement);
		bool mqtt_is_connected(void);
		char* subscribe_data(char *topic);
};

#endif
