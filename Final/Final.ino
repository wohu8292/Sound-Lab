// The button send NOTE_ON/NOTE_OFF notes on MIDI channels 1, 2, 3 // The potentiometer/controller selects the MIDI note 
// The blue button only sends NOTE_ON for sustained sounds 
// Board: "Sparkfun Pro Micro 
// ** caution! Select correct voltage in the IDE or you will brick the microcontroller! ** 
// Tools ... Processor ... (3.3v or 5v) 
// see: 
https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/troubleshooting -and-faq 
#include <MIDIUSB.h> 
// Define MIDI Notes 
#define NOTE_C1 33 
#define NOTE_C2 65 
#define NOTE_C3 131 
#define NOTE_C4 262 
#define NOTE_D4 294 
#define NOTE_E4 330 
// LED connections 
#define RED_LED_PIN 16 
#define GREEN_LED_PIN 15 
#define BLUE_LED_PIN 14 
// Button connections 
#define RED_BUTTON 9 
#define GREEN_BUTTON 8 
#define BLUE_BUTTON 7 
// Joystick connections 
#define JOY_VRX A2 
#define JOY_VRY A3 
#define JOY_SW 3 
#define SPEAKER 2 
#define NOTEDURATION 200 // milliseconds 
// Potentiometer 
#define POT_PIN A0 
int controller = 0; 
int controller_prev = 0; 
#define CONTROLLER_DELTA 1
// MIDI Channels 
#define MIDI_CHANNEL1 0 
#define MIDI_CHANNEL2 1 
#define MIDI_CHANNEL3 2 
// MIDI Notes 
#define RED_NOTE 64 
#define GREEN_NOTE 62 
#define BLUE_NOTE 60 
// MIDI Controller Numbers 
#define MIDI_CONTROLLER_A 15 
#define MIDI_CONTROLLER_X 16 // Additional MIDI controller for joystick X #define MIDI_CONTROLLER_Y 17 // Additional MIDI controller for joystick Y 
void setup() { 
Serial.begin(9600); 
// Initialize LED pins as output 
pinMode(RED_LED_PIN, OUTPUT); 
pinMode(GREEN_LED_PIN, OUTPUT); 
pinMode(BLUE_LED_PIN, OUTPUT); 
// Initialize button pins as inputs 
pinMode(RED_BUTTON, INPUT_PULLUP); 
pinMode(GREEN_BUTTON, INPUT_PULLUP); 
pinMode(BLUE_BUTTON, INPUT_PULLUP); 
// Initialize joystick pins 
pinMode(JOY_VRX, INPUT); 
pinMode(JOY_VRY, INPUT); 
pinMode(JOY_SW, INPUT_PULLUP); 
// Blink an LED 
digitalWrite(RED_LED_PIN, HIGH); 
delay(1000); 
digitalWrite(RED_LED_PIN, LOW); 
Serial.println(F(__FILE__)); 
} 
void loop() { 
// Handle button presses 
// Existing button handling code here...
// Read joystick values 
int joyX = analogRead(JOY_VRX); 
int joyY = analogRead(JOY_VRY); 
bool joyPressed = digitalRead(JOY_SW) == LOW; 
int midiX = map(joyX, 0, 1023, 0, 127); 
int midiY = map(joyY, 0, 1023, 0, 127); 
// Send MIDI control change for joystick X and Y 
controlChange(MIDI_CHANNEL1, MIDI_CONTROLLER_X, midiX); 
controlChange(MIDI_CHANNEL1, MIDI_CONTROLLER_Y, midiY); 
// Handle joystick button press 
if (joyPressed) { 
sendNoteOn(BLUE_NOTE, MIDI_CHANNEL3); // Example note 
} else { 
sendNoteOff(BLUE_NOTE, MIDI_CHANNEL3); 
} 
// Existing MIDI handling code... 
} 
// Rest of your existing functions (noteOn, noteOff, sendNoteOn, sendNoteOff, controlChange) here... 
void noteOn(byte channel, byte pitch, byte velocity) { 
Serial.print("Receive Note On: "); 
Serial.print(pitch); 
Serial.print(", ch="); 
Serial.print(channel); 
Serial.print(", vel="); 
Serial.print(velocity); 
Serial.println(""); 
switch (pitch) { 
case RED_NOTE: 
digitalWrite(RED_LED_PIN, HIGH); 
tone(SPEAKER, NOTE_E4, NOTEDURATION); 
break; 
case GREEN_NOTE: 
digitalWrite(GREEN_LED_PIN, HIGH); 
tone(SPEAKER, NOTE_D4, NOTEDURATION); 
break; 
case BLUE_NOTE:
digitalWrite(BLUE_LED_PIN, HIGH); 
tone(SPEAKER, NOTE_C4, NOTEDURATION); 
break; 
} 
} 
void noteOff(byte channel, byte pitch, byte velocity) { 
Serial.print("Receive Note Off: "); 
Serial.print(pitch); 
Serial.print(", ch="); 
Serial.print(channel); 
Serial.print(", vel="); 
Serial.println(velocity); 
switch (pitch) { 
case RED_NOTE: 
digitalWrite(RED_LED_PIN, LOW); 
break; 
case GREEN_NOTE: 
digitalWrite(GREEN_LED_PIN, LOW); 
break; 
case BLUE_NOTE: 
digitalWrite(BLUE_LED_PIN, LOW); 
break; 
} 
} 
void sendNoteOn(byte pitch, byte channel) { midiEventPacket_t tx; 
//note on 
tx = { 0x09, 0x90 | channel, pitch, 127 }; MidiUSB.sendMIDI(tx); 
// send MIDI now 
MidiUSB.flush(); 
} 
void sendNoteOff(byte pitch, byte channel) { midiEventPacket_t tx; 
//note off 
tx = { 0x08, 0x80 | channel, pitch, 127 }; MidiUSB.sendMIDI(tx); 
// send MIDI now 
MidiUSB.flush(); 
}
// First parameter is the event type (0x0B = control change). // Second parameter is the event type, combined with the channel. // Third parameter is the control number number (0-119). // Fourth parameter is the control value (0-127). 
void controlChange(byte channel, byte control, byte value) { midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value}; MidiUSB.sendMIDI(event); 
}
