#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define NUM_BUTTONS  3
#define PIN 3

//#define NUM_LEDS 228
#define NUM_LEDS 100
#define BRIGHTNESS 30      //needed?


//Constants
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//-----------------------------
//---------Must be hardware implemented

const uint8_t button1 = 2;
const uint8_t button2 = 6;
const uint8_t button3 = 4;

const int intensityPot = 0;

const uint8_t buttons[NUM_BUTTONS] = {button1, button2, button3};
//-----------------------------

//Variables
boolean button_state [NUM_BUTTONS] = {0, 0, 0};
uint8_t message_old[6] = {0, 0, 0, 0, 0, 0};
uint8_t message[6] = {0, 0, 0, 0, 0, 0};

uint8_t intensity = 0;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);


boolean blackout = false;
boolean scene1_active = false;
boolean scene2_active = false;

//scene1
boolean redfull = false;
boolean redempty = true;
boolean bluefull = false;
boolean blueempty = true;
boolean yellowfull = false;
boolean yellowempty = true;


int gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };



uint8_t red_[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //64
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //128
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //192
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //224
  0,  0,  0,  0};                                                   //228 Values for Pixel Color

uint8_t green_[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //64
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //128
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //192
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //224
    0,  0,  0,  0};                                                   //228 Values for Pixel Color

uint8_t blue_[] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //64
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //128
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //192
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    //224
      0,  0,  0,  0};                                                   //228 Values for Pixel Color



void setup() {
  Serial.begin(115200);

  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT_PULLUP);
  //pinMode(potPin, INPUT);

  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter

  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.begin();
  strip.show();
}

void loop() {
  strip.setBrightness(100);

  if(scene1_active == true && redfull != true){
    Serial.println("Redgoes up ");

    for(int i = 0; i < NUM_LEDS; i++){
      red_[i]= 255;
      Serial.println(red_[i]);
      uint32_t act_color = strip.Color(red_[i], green_[i], blue_[i]);
      strip.setPixelColor(i, act_color);         //  Set pixel's color (in RAM)
      strip.show();
      delay(20);

    }
    if(red_[0] >= 255){
      redfull = true;
    }
    strip.show();
    delay(20);
    makeRoutine();
  }
  if(scene2_active == true){
    Serial.println("start colorwipe ");

    colorWipe(strip.Color(255, 0, 0), 50); // Red

  }
  if(blackout == true){

        for(uint16_t i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        }
          strip.show();

  }
  delay(20);
  makeRoutine();

}       // for transmit

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


void makeRoutine(){
  readButtons();
  setFlags();
  readIntensity();
  if(updateMessage() == true){
    for(int i=0; i<6; i++){
      message_old[i] = message[i];
    }
  printFlagStatus();
  printMessage();
  //sendMessage();
  }
}

void printFlagStatus(){
  Serial.print("BLACKOUT is ");
  Serial.println(blackout);
  Serial.print("SCENE1 is ");
  Serial.println(scene1_active);
  Serial.print("SCENE2 is ");
  Serial.println(scene2_active);


}

void setFlags(){
  if(button_state[0] == 1){
    blackout = true;
    scene1_active = false;
    scene2_active = false;
  }
  if(button_state[1] == 1){
    blackout = false;
    scene1_active = true;
    scene2_active = false;
  }
  if(button_state[2] == 1){
    blackout = false;
    scene1_active = false;
    scene2_active = true;
  }

}

void sendMessage(){
  radio.write(&message, sizeof(message));
}


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


void readButtons(){
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
