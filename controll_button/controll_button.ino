#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//Constants
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
const int button_pin = 4;

//-----------------------------
//---------Must be hardware implemented

const int button_pin2 = 5;
const int button_pin3 = 6;

const int potPin = A0;
//-----------------------------

//Variables
boolean button_state = 0;
boolean button_state2 = 0;
boolean button_state3 = 0;
int     pot_value_old = 0;
int     pot_value = 0;  //Save analog value from fader



void setup() {
  Serial.begin(9600);
  
  pinMode(button_pin, INPUT);
  pinMode(button_pin2, INPUT);
  pinMode(button_pin3, INPUT);
  pinMode(potPin, INPUT);
  
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}
void loop()
{
  button_state = digitalRead(button_pin);
  button_state2 = digitalRead(button_pin2);
  button_state3 = digitalRead(button_pin3);
  pot_value = analogRead(potPin);          //Read and save analog value from potentiometer
  pot_value = map(pot_value, 0, 1023, 0, 255); //
  
  if (button_state == HIGH)
  {
    const char text[] = "Your Button1 State is HIGH";
    Serial.println("button1 pressed");//Sending the message to receiver
    radio.write(&button_state, sizeof(button_state));  //Sending the message to receiver
    radio.write(&button_state2, sizeof(button_state2));  //Sending the message to receiver
    radio.write(&button_state3, sizeof(button_state3));  //Sending the message to receiver

  }
  if (button_state2 == HIGH)
  {
    const char text[] = "Your Button2 State is HIGH";
    radio.write(&text, sizeof(text));      
    Serial.println("button2 pressed");//Sending the message to receiver
    radio.write(&button_state, sizeof(button_state));  //Sending the message to receiver
    radio.write(&button_state2, sizeof(button_state2));  //Sending the message to receiver
    radio.write(&button_state3, sizeof(button_state3));  //Sending the message to receiver
  }
  if (button_state3 == HIGH)
  {
    const char text[] = "Your Button3 State is HIGH";
    radio.write(&text, sizeof(text));      
    Serial.println("button3 pressed");//Sending the message to receiver
    radio.write(&button_state, sizeof(button_state));  //Sending the message to receiver
    radio.write(&button_state2, sizeof(button_state));  //Sending the message to receiver
    radio.write(&button_state3, sizeof(button_state));  //Sending the message to receiver  }
  if(pot_value != pot_value_old)
  {
    pot_value_old = pot_value;
    radio.write(&pot_value, sizeof(pot_value));   //Sending potentiometer value to GLOWBAll
  }
  delay(100);
}
