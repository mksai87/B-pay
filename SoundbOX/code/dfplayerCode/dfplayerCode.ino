#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Define connections
SoftwareSerial mySoftwareSerial(D5, D6); // RX, TX

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  // Start serial communication for debugging
  
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  // Initialize the DFPlayer
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    while (true);
  }
  
  Serial.println(F("DFPlayer Mini online."));
  
  // Set volume (0 to 30)
  myDFPlayer.volume(30);  
}

void loop() {
  for (int i = 1; i <= 10; i++) { // Assuming you have 10 audio files named 1.mp3 to 10.mp3
    myDFPlayer.play(i);
    Serial.print(F("Playing track "));
    Serial.println(i);

    // Wait for the track to finish
    delay(1000); // Adjust this delay according to the length of your audio files
  }

  // Optionally, you can add a delay or loop the playback continuously
  delay(2000); // Delay before looping back to the first track
}
