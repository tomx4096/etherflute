#include <Arduino.h>
#include <Ethernet.h>
#include "board_led.h"
#include "board_eth.h"
#include "board_buzz.h"

// storage for incoming packets
char rx_buffer[UDP_TX_PACKET_MAX_SIZE];

void setup() {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    init_status_LED();
    init_board_eth();
    init_buzz();
}

void loop() {
    maintain_eth();
    int bytes_len = recv_packet_from_eth(rx_buffer);
    if(bytes_len) {
        parse_buzz(rx_buffer, bytes_len);
        make_buzz();
    }
    update_status_LED();
}


