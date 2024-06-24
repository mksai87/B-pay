#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Define connections
SoftwareSerial mySoftwareSerial(D5, D6); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int ones_place[] = {0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int tens_place[] = {0,0,21,22,23,24,25,26,27,28};

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9700);

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
  if (Serial.available()) {
    String value = Serial.readStringUntil('\n');
    value.trim();
    
    if (value.equals("0")) {
     // Serial.println("0");
       myDFPlayer.play(2);
       delay(700);
    } else {
      numberToWords(value);
    }
  }
}

void numberToWords(String num) {
  int length = num.length();

  // Handle thousands place
  if (length == 4) {
    int digit = num.charAt(0) - '0';
    if (digit != 0) {
     // Serial.print(ones_place[digit]);
      myDFPlayer.play(ones_place[digit]);
       delay(700);
    //  Serial.println(" 1000");
       myDFPlayer.play(30);
       delay(700);
    }
    num = num.substring(1);
    length--;
  }

  // Handle hundreds place
  if (length == 3) {
    int digit = num.charAt(0) - '0';
    if (digit != 0) {
     // Serial.print(ones_place[digit]);
       myDFPlayer.play(ones_place[digit]);
       delay(700);
     // Serial.println(" 100");
       myDFPlayer.play(29);
       delay(700);
    }
    num = num.substring(1);
    length--;
  }

  // Handle tens and ones place
  if (length == 2) {
    int digit = num.charAt(0) - '0';
    if (digit == 1) { // Handle numbers between 10 and 19
      int teen = num.toInt();
     // Serial.println(ones_place[teen]);
       myDFPlayer.play(ones_place[teen]);
       delay(700);
      return;
    } else if (digit > 1) {
     // Serial.print(tens_place[digit]);
      myDFPlayer.play(tens_place[digit]);
       delay(700);
      if (num.charAt(1) != '0') {
       // Serial.print("-");
      } else {
       // Serial.println();
        return;
      }
    }
    num = num.substring(1);
    length--;
  }

  // Handle ones place
  if (length == 1) {
    int digit = num.charAt(0) - '0';
    if (digit != 0 || (digit == 0 && num.length() == 1)) {
     // Serial.println(ones_place[digit]);
      myDFPlayer.play(ones_place[digit]);
       delay(700);
    }
  }
}
