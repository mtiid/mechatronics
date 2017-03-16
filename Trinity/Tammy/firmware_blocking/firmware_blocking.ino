
// Authors: Ajay Kapur and Eric Heep 
// 2014 Redo of Robot Code for KarmetiK Instruments
// Serial Input in conjunction with 

#define LED_POWER 12
#define LED_STATUS 11

#include <avr/interrupt.h>
#include <avr/io.h>

// ID number of the arduino, each robot must have a different one

// Marimbot is 0
// For Lydia... TrimpSpin is 2 and TrimpBeat is 1
// SnapperBots are 3-8
// StringThing is 9
// RattleTron is 10
// Tammy is 11
#define arduinoID 11

// stores our incoming values
char bytes[2];
int handshake = 0;

short notes[20];
int num_pwmpins = 20;
int pwmpins[] = {
  22,24,26,28,30,32,34,36,38,40,23,25,27,29,31,33,35,37,39,41}; 
const int NUM_SOLENOIDS = 20;
int statustimer = 0;

void setup() {
  // Serial  open
  Serial.begin(57600);

  // Power and status LEDS
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  digitalWrite(LED_POWER, HIGH); 

  for (int i = 0; i <  num_pwmpins; i++) {
    pinMode(pwmpins[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < 20; i++){
    digitalWrite(pwmpins[i], HIGH);
    delay(100);
    digitalWrite(pwmpins[i], LOW);
    delay(500);
  }
}
 

