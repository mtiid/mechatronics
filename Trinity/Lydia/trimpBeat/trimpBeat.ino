
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
#define arduinoID 1

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

  // Interrupt timer parameters 
  TCCR2A = 1;
  TCCR2B = 3;
  TIMSK2 = 1;

  // Power and status LEDS
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  digitalWrite(LED_POWER, HIGH); 

  for (int i = 0; i <  num_pwmpins; i++) {
    pinMode(pwmpins[i], OUTPUT);
  }
  // when done booting, make sound
  notes[0] = 100;
  notes[10] = 100;
}

ISR(TIMER2_OVF_vect){
  // Solenoid Control 
  for (int idx = 0; idx < num_pwmpins; idx++){
    if (notes[idx]>0) {
      digitalWrite(pwmpins[idx],HIGH); 
      notes[idx]--;
    } 
    else {
      digitalWrite(pwmpins[idx],LOW);
    }
  }
  // Status Timer
  if (statustimer > 0) {
    digitalWrite(LED_STATUS, HIGH);
    statustimer--;
  } 
  else{
    digitalWrite(LED_STATUS, LOW);
  } 
}

void testSolenoid(int delay_time, int note, int velocity) {
  statustimer = 120;
  notes[note] = (velocity * 0.5);  
  delay(delay_time);
}

void loop() {
  // Read Serial Data
  if (Serial.available()) {
    // parity byte
    if (Serial.read() == 0xff) {
      // reads in a two index array from ChucK
      Serial.readBytes(bytes, 2);

      // reads the first six bits for the note number
      // then reads the last ten bits for the note velocity
      int note = byte(bytes[0]) >> 2;
      int velocity = (byte(bytes[0]) << 8 | byte(bytes[1])) & 1023;

      // message required for "handshake" to occur
      // happens once per Arduino at the start of the ChucK serial code
      // unnecessary if only using one Meepo at a time
      if (note == 63 && velocity == 1023 && handshake == 0) {
        Serial.write(arduinoID);
        handshake = 1;
        // when handshake works they make a sound
        notes[5] = 100;
        notes[15] = 100;
      }
      if (note >= 0 && note <= NUM_SOLENOIDS) {
        statustimer = 120;
        notes[note] = (velocity * 0.5);
      }
    }
  }
}


