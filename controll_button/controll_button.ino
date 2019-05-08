#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define NUM_BUTTONS  3
//Constants
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//-----------------------------
//---------Must be hardware implemented

const uint8_t button1 = 2;
const uint8_t button2 = 3;
const uint8_t button3 = 4;

const int intensityPot = 0;

const uint8_t buttons[NUM_BUTTONS] = {button1, button2, button3};
//-----------------------------

//Variables
boolean button_state [NUM_BUTTONS] = {0, 0, 0};
uint8_t message_old[6] = {0, 0, 0, 0, 0, 0};
uint8_t message[6] = {0, 0, 0, 0, 0, 0};



uint8_t intensity;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT_PULLUP);
  //pinMode(potPin, INPUT);

  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}

void loop() {
  readButtons();
  readIntensity();
  if(updateMessage() == true){
    for(int i=0; i<6; i++){
      message_old[i] = message[i];
    }
    printMessage();
  }

}

/*
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
    radio.write(&button_state3, sizeof(button_state));  //Sending the message to receiver
    }

  if(pot_value != pot_value_old)
  {
    pot_value_old = pot_value;
    radio.write(&pot_value, sizeof(pot_value));   //Sending potentiometer value to GLOWBAll
  }
  delay(100);
}
*/

void printMessage(){
  for(int i = 0; i < 6; i++){
    Serial.print(message[i]);//Sending the message to receiver
  }
  Serial.println("");

}


boolean updateMessage(){
  for(int i = 0; i < 6 ; i++){
    if(message[i] != message_old[i]){
      return true;
    }
  }
  return false;
}




void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == LOW)
    {
      /*
      Serial.print("button ");
      Serial.print(i);//Sending the message to receiver
      Serial.println(" pressed." );
      */
      //bitWrite(pressedButtons, i, 1);
      button_state[i] = 1;
      message[i] = 1;
    }
    else{
      button_state[i] = 0;
      message[i] = 0;
    }
  }
  delay(50);

}

void readIntensity()
{
  int val = analogRead(intensityPot);
  intensity = (uint8_t) (map(val, 0, 1023, 0, 255));
  uint8_t intensity_001 = intensity % 10;
  uint8_t intensity_010 = ((intensity-intensity_001)%100)/10;
  uint8_t intensity_100 = (intensity-intensity_010-intensity_001)/100;
  message[3] = intensity_100;
  message[4] = intensity_010;
  message[5] = intensity_001;
  //Serial.print("fader intensity: ");
  //Serial.println(intensity);//Sending the message to receiver
}
