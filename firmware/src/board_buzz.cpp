#define NUM_BUZZERS	4
#include <Arduino.h>
#include "board_buzz.h"
#include "Tone.h"

uint8_t hex_char_to_int(char ch);
void dump_buzz_state();

struct buzzer_state {
    uint8_t freq;
    uint8_t power;
    Tone tone;
};

struct buzzer_state bz[NUM_BUZZERS];
const int bz_pins[NUM_BUZZERS] = {3,5,6,9}; 

const int note_map[16] = {NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5,
                          NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5,
                          NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
                          NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6};


void init_buzz() {
    for (int i = 0; i < NUM_BUZZERS; i++) {
        bz[i].freq = 0;
        bz[i].power = 0;
        bz[i].tone.begin(bz_pins[i]);
    }
}

/*
 * Current scheme: Input to each buzzer is two 4-bit numbers (0-f)
 * First number is the frequency of the tone and the second is the loudness
 *
 * rx_buffer of "a0a0a0a0" means:
 *       set each of the 4 buzzers to freq "a" and loudness "0"
 */ 
void parse_buzz(char rx_buffer[], int len) {
    if(len != (2*NUM_BUZZERS)) {
        Serial.print(2*NUM_BUZZERS);
        Serial.println(" byte payload length please");
        return;
    }

    for (int i = 0; i < NUM_BUZZERS; i++) {
        uint8_t freq = hex_char_to_int(rx_buffer[i*2]);
        uint8_t power = hex_char_to_int(rx_buffer[i*2+1]);
        if(freq < 0 || power < 0 || freq > 15 || power > 15) {
            Serial.println("Thanks for the garbage input");
	    return;
        }	
	bz[i].freq = note_map[freq];
        bz[i].power = power;
    }
}

void make_buzz() {
    dump_buzz_state();
    for (int i = 0; i < NUM_BUZZERS; i++) {
        if(bz[i].power == 0) {
            bz[i].tone.stop();
        } else {
            bz[i].tone.play(bz[i].freq);
        }
    }
}

void dump_buzz_state() {
    for (int i = 0; i < NUM_BUZZERS; i++) {
        Serial.print("buzz#");
        Serial.print(i);
	Serial.print(": freq -> ");
        Serial.print(bz[i].freq);
	Serial.print(", power -> ");
        Serial.println(bz[i].power);
    }
}

uint8_t hex_char_to_int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

