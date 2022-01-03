
#include "MIDIUSB.h"
int ldr_pin = A0; // the analog pin via which the ldr is connected to the arduino
int ldr_value = 0;
int last_midi;
int midi_from_ldr = 0;

// MIDI Note on function, sends MIDI note on signal to send buffer
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

// MIDI Note off function, sends MIDI note off signal to send buffer
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
   pinMode(ldr_pin, INPUT);
   Serial.begin(9600);
}

void loop() {
  // Reading the ldr value from ldr_pin
  ldr_value = analogRead(ldr_pin);
  // Setting the previous MIDI note so we can compare later
  last_midi = midi_from_ldr;

  // Setting MIDI note based on ldr value
  // I divided the 0-1027 range into 7 segments because I have 7 scenes.
  if(ldr_value <= 147) {
    midi_from_ldr = 64;
  } else if(ldr_value <= 294) {
    midi_from_ldr = 65;
  } else if(ldr_value <= 441) {
    midi_from_ldr = 66;
  } else if(ldr_value <= 588) {
    midi_from_ldr = 67;
  } else if(ldr_value <= 735) {
    midi_from_ldr = 68;
  } else if(ldr_value <= 882) {
    midi_from_ldr = 69;
  } else {
    midi_from_ldr = 70;
  }

  // checking if the new MIDI note is the same as the last one that was sent
  if(midi_from_ldr != last_midi) {
    // If it isn't send the note on signal
    noteOn(0, midi_from_ldr, 64);
    // Flush the buffer to make sure everything is sent
    MidiUSB.flush();
    delay(50);
    // Send the note off signal to make sure the key isn't unnecessarily pressed.
    noteOff(0, midi_from_ldr, 64);
    // Flush the buffer again to make sure everything is sent
    MidiUSB.flush();    
  }
  // Wait a bit before checking everything again.
  // It isn't necessary to check it too often
  delay(500);
}
