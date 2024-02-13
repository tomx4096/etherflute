#include <Arduino.h>
#define STATUS_LED_PIN		13
#define STATUS_UPDATE_DT_MS	1000

unsigned long now_time;
unsigned long start_time;
bool status_led_on;

void init_status_LED() {
    pinMode(STATUS_LED_PIN, OUTPUT);
    status_led_on = false;
    start_time = millis();
}

void update_status_LED() { 
    now_time = millis();
    if( (now_time - start_time) >= STATUS_UPDATE_DT_MS ) {
        if (status_led_on) {
            digitalWrite(STATUS_LED_PIN, LOW);
        } else {
            digitalWrite(STATUS_LED_PIN, HIGH);
        }
        status_led_on = !status_led_on;
        start_time = now_time;
    }
}
