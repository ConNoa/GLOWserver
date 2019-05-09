/*
Created by Yvan / https://Brainy-Bits.com

This code is in the public domain...
You can: copy it, use it, modify it, share it or just plain ignore it!
Thx!

*/


// NRF24L01 Module Tutorial - Code for Receiver using Arduino UNO

//Include needed Libraries at beginning
#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "FastLED.h" // FastLED library for WS2812 RGB Stick http://fastled.io/

#define NUM_LEDS 8 // Number of leds on stick
#define LED_PIN 8 // Digital In (DI) of RGB Stick connected to pin 8 of the UNO

CRGB leds[NUM_LEDS]; // FastLED Library Init

int ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01

RF24 radio(9,10); // NRF24L01 used SPI pins + Pin 9 and 10 on the UNO

const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01


void setup(void){

FastLED.addLeds<NEOPIXEL,LED_PIN>(leds, NUM_LEDS); // Setup FastLED Library
FastLED.clear(); // Clear the RGB Stick LEDs

// Light up starting LED's
for(int x = 0; x != NUM_LEDS; x++) {
leds[x] = CRGB::Red;
}

FastLED.setBrightness(50);
FastLED.show();

radio.begin(); // Start the NRF24L01

radio.openReadingPipe(1,pipe); // Get NRF24L01 ready to receive

radio.startListening(); // Listen to see if information received

pinMode(LED_PIN, OUTPUT); // Set RGB Stick UNO pin to an OUTPUT
}

void loop(void){

while (radio.available())
{
radio.read(ReceivedMessage, 1); // Read information from the NRF24L01

if (ReceivedMessage[0] == 111) // Indicates switch is pressed
{
for(int x = 0; x != NUM_LEDS; x++)
{
leds[x] = CRGB::Green;
FastLED.show();
}
}
else
{
for(int x = 0; x != NUM_LEDS; x++)
{
leds[x] = CRGB::Red;
FastLED.show();
}
}
delay(10);
}
}
