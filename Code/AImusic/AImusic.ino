#include <Adafruit_NeoPixel.h>
#define PIN   7
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

int hall1;
int hall2;
int hall3;
int hall4;
int hall5;
int hall6;
int hall7;
int hall8;

int brightnessR = 20;
int brightnessG = 20;
int brightnessB = 20;

unsigned long currentMillis;
long previousMillis = 0;    // set up timers
long interval = 10;        // time constant for timer
long previousSafetyMillis;    // timer to check the data is there or stop on safties

int stepFlag = 0;
long previousStepMillis = 0;
int stepStartFlag = 0;
int top;

int noteFlag1 = 0;
int noteFlag2 = 0;
int noteFlag3 = 0;
int noteFlag4 = 0;
int noteFlag5 = 0;

int trigger1Value = 400;
int pitchShift = 0;

int controller1;
int controller1Prev;
int controller2;
int controller2Prev;

void setup() {
  Serial.begin(115200);
  pixels.begin();

  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {

  currentMillis = millis();
  if (currentMillis - previousMillis >= 10) {  // start timed event
    
      previousMillis = currentMillis;

      if (currentMillis - previousStepMillis > 100) {            
            previousStepMillis = currentMillis;
            if (top == 0) {
              stepFlag = stepFlag + 1;
            }
            else if (top == 1) {
              stepFlag = stepFlag - 1;
            }
            pixels.clear();
            pixels.setPixelColor(stepFlag, pixels.Color(brightnessR, brightnessG, brightnessB)); 
            pixels.setPixelColor(stepFlag + 4, pixels.Color(brightnessR, brightnessG, brightnessB));
            pixels.setPixelColor(stepFlag + 8, pixels.Color(brightnessR, brightnessG, brightnessB));  
            pixels.setPixelColor(stepFlag - 4, pixels.Color(brightnessR, brightnessG, brightnessB)); 
            pixels.setPixelColor(stepFlag - 8, pixels.Color(brightnessR, brightnessG, brightnessB));
            pixels.setPixelColor(stepFlag - 16, pixels.Color(brightnessR, brightnessG, brightnessB));
            pixels.setPixelColor(stepFlag + 16, pixels.Color(brightnessR, brightnessG, brightnessB));
            pixels.show();
            if (stepFlag == 30) {
              top = 1;            
            }
            else if (stepFlag == 0) {
              top = 0;
            }
      }   
    
      hall1 = analogRead(A8);
      hall2 = analogRead(A9);
      hall3 = analogRead(A10);
      hall4 = analogRead(A11);
      hall5 = analogRead(A12);
      hall6 = analogRead(A13);
      hall7 = analogRead(A14);
      hall8 = analogRead(A15);

      /*
      Serial.print(hall1);
      Serial.print(" , ");
      Serial.print(hall2);
      Serial.print(" , ");
      Serial.print(hall3);
      Serial.print(" , ");
      Serial.print(hall4);
      Serial.print(" , ");
      Serial.print(hall5);
      Serial.print(" , ");
      Serial.print(hall6);
      Serial.print(" , ");
      Serial.print(hall7);
      Serial.print(" , ");
      Serial.println(hall8);
      */


      // pitch shift

      if (hall8 > 516) {
        pitchShift = 0;
        brightnessR = 20;
        brightnessG = 20;
        brightnessB = 20;  
      }

      else if (hall8 < 520 && hall8 > 510) {
        pitchShift = 12;
        brightnessR = 60;
        brightnessG = 0;
        brightnessB = 0;        
      }
      else if (hall8 < 509 && hall8 > 490) {
        pitchShift = 24;
        brightnessR = 0;
        brightnessG = 60;
        brightnessB = 0;
      }
      else if (hall8 < 489 && hall8 > 0) {
        pitchShift = 36;
        brightnessR = 0;
        brightnessG = 0;
        brightnessB = 60;
      }

      // controllers

      controller1 = map(hall6,250,520,0,127);
      controller1 = constrain(controller1,0,127);
      if (controller1 != controller1Prev) {      
          MIDI.sendControlChange(2, controller1, 1);
      }
      controller1Prev = controller1;



      controller2 = map(hall7,265,520,0,127);
      controller2 = constrain(controller2,0,127);
      if (controller2 != controller2Prev) {      
          MIDI.sendControlChange(3, controller2, 1);
      }
      controller2Prev = controller2;



      // note 1

      if (hall4 < trigger1Value && noteFlag1 == 0) {
        MIDI.sendNoteOn(30 + pitchShift, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
        noteFlag1 = 1;
      }
      else if (hall4 > trigger1Value+1 && noteFlag1 == 1) {
        MIDI.sendNoteOff(30, 0, 1);     // Stop the note
        MIDI.sendNoteOff(30 + 12, 0, 1);     // Stop the note
        MIDI.sendNoteOff(30 + 24, 0, 1);     // Stop the note
        MIDI.sendNoteOff(30 + 36, 0, 1);     // Stop the note
        noteFlag1 = 0;
      }

      // note 2

      if (hall1 < trigger1Value && noteFlag2 == 0) {
        MIDI.sendNoteOn(33 + pitchShift, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
        noteFlag2 = 1;
      }
      else if (hall1 > trigger1Value+1 && noteFlag2 == 1) {
        MIDI.sendNoteOff(33, 0, 1);     // Stop the note
        MIDI.sendNoteOff(33 + 12, 0, 1);     // Stop the note
        MIDI.sendNoteOff(33 + 24, 0, 1);     // Stop the note
        MIDI.sendNoteOff(33 + 36, 0, 1);     // Stop the note

        noteFlag2 = 0;
      }

      // note 3

      if (hall5 < trigger1Value && noteFlag3 == 0) {
        MIDI.sendNoteOn(35 + pitchShift, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
        noteFlag3 = 1;
      }
      else if (hall5 > trigger1Value+1 && noteFlag3 == 1) {
        MIDI.sendNoteOff(35, 0, 1);     // Stop the note
        MIDI.sendNoteOff(35 + 12, 0, 1);     // Stop the note
        MIDI.sendNoteOff(35 + 24, 0, 1);     // Stop the note
        MIDI.sendNoteOff(35 + 36, 0, 1);     // Stop the note

        noteFlag3 = 0;
      }

      // note 4

      if (hall2 < trigger1Value && noteFlag4 == 0) {
        MIDI.sendNoteOn(36 + pitchShift, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
        noteFlag4 = 1;
      }
      else if (hall2 > trigger1Value+1 && noteFlag4 == 1) {
        MIDI.sendNoteOff(36, 0, 1);     // Stop the note
        MIDI.sendNoteOff(36 + 12, 0, 1);     // Stop the note
        MIDI.sendNoteOff(36 + 24, 0, 1);     // Stop the note
        MIDI.sendNoteOff(36 + 36, 0, 1);     // Stop the note

        noteFlag4 = 0;
      }
      
      // note 5

      if (hall3 < trigger1Value && noteFlag5 == 0) {
        MIDI.sendNoteOn(37 + pitchShift, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
        noteFlag5 = 1;
      }
      else if (hall3 > trigger1Value+1 && noteFlag5 == 1) {
        MIDI.sendNoteOff(37, 0, 1);     // Stop the note
        MIDI.sendNoteOff(37 + 12, 0, 1);     // Stop the note
        MIDI.sendNoteOff(37 + 24, 0, 1);     // Stop the note
        MIDI.sendNoteOff(37 + 36, 0, 1);     // Stop the note

        noteFlag5 = 0;
      }
      
  } // end of timed loop


        
  
}
