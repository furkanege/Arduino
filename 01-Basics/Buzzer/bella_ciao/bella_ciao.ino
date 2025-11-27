/*
  Project: Bella Ciao Buzzer Melody
  File: bella_ciao.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Plays the "Bella Ciao" melody using a piezo buzzer connected to pin 11.
    Demonstrates tone generation, note sequencing and basic music playback
    using Arduino’s built-in tone() function.

  Wiring:
    Buzzer + → D11
    Buzzer − → GND

  Libraries:
    - (No external libraries required — uses built-in tone())

  Serial Baud:
    Not used

  Example Serial Output:
    (No serial output for this project)

  Notes:
    - tone() uses PWM to generate square-wave audio signals.
    - Some buzzers may require a current-limiting resistor.
    - The melody plays only once on startup.

  Real-World Applications:
    - Alarms
    - Audio notifications
    - Embedded system feedback tones

  License: GPL-3.0
*/


int buzzer = 11;  // Buzzer connected to Arduino digital pin 11

// ------------------------------------------------------------
// NOTE FREQUENCIES (Standard Equal-Temperament A4 = 440 Hz)
// ------------------------------------------------------------
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494

#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880

// ------------------------------------------------------------
// BELLA CIAO MELODY (Sequence of Notes)
// ------------------------------------------------------------
int melody[] = {
  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_E4, NOTE_D4, NOTE_D4,

  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_D4, NOTE_C4, NOTE_C4
};

// ------------------------------------------------------------
// NOTE DURATIONS
// 4 = quarter note, 2 = half note, etc.
// ------------------------------------------------------------
int noteDurations[] = {
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,2,

  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,2,2
};

// ------------------------------------------------------------
// SETUP: Plays the Bella Ciao melody once
// ------------------------------------------------------------
void setup() {
  for (int i = 0; i < 29; i++) {
    int noteDuration = 1000 / noteDurations[i];  // Convert note type to ms
    tone(buzzer, melody[i], noteDuration);       // Play note

    // Small pause between notes for clarity
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(buzzer); // Stop buzzer
  }
}

// ------------------------------------------------------------
// LOOP: Empty — melody plays only once on startup
// ------------------------------------------------------------
void loop() {
  // Bella Ciao does not repeat
}