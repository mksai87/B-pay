#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(D5, D6); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  // Initialize DFPlayer Mini
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer Mini initialization failed!");
    while(true);
  }
  Serial.println("DFPlayer Mini ready.");

  myDFPlayer.volume(30); // Set volume to a reasonable level (0-30)
}

void loop() {
  if (Serial.available()) {
    String fileName = Serial.readStringUntil('\n');
    fileName.trim();

    if (fileName.length() > 0) {
      playFile(fileName);
    }
  }
}

void playFile(String fileName) {
  int fileNumber = fileName.toInt();
  if (fileNumber > 0) {
    myDFPlayer.play(fileNumber);
    Serial.print("Playing file: ");
    Serial.println(fileNumber);
  } else {
    Serial.println("Invalid file number.");
  }
}
