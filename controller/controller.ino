/*
  Arduino Wireless Communication Tutorial
      Example 1.1 - Transmitter Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int button_1 = 4;
int button_1_status = 0;
boolean color_mode_1_active = false;
boolean color_mode_2_active = false;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  Serial.println("Setup started");
  pinMode(button_1, INPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.println("Setup finished");

}
void loop() {
  //Serial.print("Loop started");

  button_1_status = digitalRead(button_1);
  if (button_1_status == HIGH) {
    const char text[] = "Your Button State is HIGH";
    radio.write(&text, sizeof(text));         
    color_mode_1_active = true;
    color_mode_2_active = false;
  }
  if (color_mode_1_active == true) {
    Serial.println("color_mode_1_active");

    //SentMessage[0] = 111;
   // radio.write(SentMessage, 1); // Send value through NRF24L01
    delay(3000);
  }
  color_mode_1_active = false;
  Serial.println("color_mode_1_not_active_anymore");

}
