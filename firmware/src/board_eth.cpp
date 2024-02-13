#define BOARD_ETH_CHIP_SELECT	10
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "board_eth.h"

//Ethernet controller's MAC address
byte mac[] = {
    0x98, 0x76, 0xB6, 0x12, 0xC2, 0xCD
};

unsigned int localPort = 8888;		// local port to listen on
char tx_buffer[] = "ack\n";		// rx_buffer declared in main.cpp
EthernetUDP Udp;			// EtherUDP instance that does all the work

void init_board_eth() {
    Ethernet.init(BOARD_ETH_CHIP_SELECT);
    if (Ethernet.begin(mac) == 0) { // init with DHCP, try to get an IP
        Serial.println("Failed to configure Ethernet using DHCP");
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
            Serial.println("Ethernet controller was not found.");
        } else if (Ethernet.linkStatus() == LinkOFF) {
            Serial.println("Ethernet cable is not connected.");
        }
        // broken. die.
        while (true) {
            delay(1);
        }
    }
    Serial.print("Local IP address: ");
    Serial.println(Ethernet.localIP());

    Udp.begin(localPort);
}

void maintain_eth() {
    switch (Ethernet.maintain()) {
      case 1:  //renewed fail
          Serial.println("Error: renewed fail");
          break;
      case 2:  //renewed success
          Serial.println("Renewed success");
          Serial.print("Local IP address: ");
          Serial.println(Ethernet.localIP());
          break;
      case 3:  //rebind fail
          Serial.println("Error: rebind fail");
          break;
      case 4:  //rebind success
          Serial.println("Rebind success");
          Serial.print("Local IP address: ");
          Serial.println(Ethernet.localIP());
          break;
      default:  //nothing happened
          break;
  }
}

int recv_packet_from_eth(char rx_buffer[]) {
    // if there's data available, read a packet
    int packet_size = Udp.parsePacket();
    if (packet_size) {
        //Serial.print("Received packet of size ");
        //Serial.println(packet_size);
        Serial.print("From: ");
        IPAddress remote = Udp.remoteIP();
        for (int i=0; i < 4; i++) {
            Serial.print(remote[i], DEC);
            if (i < 3) {
                Serial.print(".");
            }
        }
        Serial.print(":");
        Serial.print(Udp.remotePort());
        Serial.print(" -> ");

        // read the packet into receive buffer
        Udp.read(rx_buffer, UDP_TX_PACKET_MAX_SIZE);
        //Serial.println("Contents:");
        Serial.println(rx_buffer);

        // send a reply to the IP address and port that sent us the packet we received
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(tx_buffer);
        Udp.endPacket();
    }
    return packet_size;
}


