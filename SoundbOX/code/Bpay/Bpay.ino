#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(D5, D6); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

#define WIFI_SSID "#######"
#define WIFI_PASSWORD "#########"
#define API_KEY "***************************************************"
#define DATABASE_URL "************************************************" 

String parent ="smslist";
String child="7396151573";
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
String priData="";

int ones_place[] = {0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int tens_place[] = {0,0,21,22,23,24,25,26,27,28};

void setup(){
  Serial.begin(115200);
  mySoftwareSerial.begin(9800);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    while (true);
  }
  
  Serial.println(F("DFPlayer Mini online."));
  
  // Set volume (0 to 30)
  
  myDFPlayer.volume(30);
  
   myDFPlayer.play(31);
   delay(3000);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    myDFPlayer.play(32);
    delay(3000);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  myDFPlayer.play(33);
  delay(3500);
}

void loop(){

   while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    myDFPlayer.play(32);
    delay(3000);
  }


  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    // Read data from Firebase
    String path = "/" + parent + "/" + child;
    if (Firebase.RTDB.getString(&fbdo, path)){
      if (fbdo.dataType() == "string"){
         String data = fbdo.stringData();
         
        if (data.length() >= 4) {
      data = data.substring(2);
      data = data.substring(0, data.length() - 2   );
      data.replace(".00", "");
      
      if(data != priData){
        priData=data;
        
        if (data.equals("0")) {
     // Serial.println("0");
       myDFPlayer.play(2);
       delay(800);
    } else {
      myDFPlayer.play(34);
      delay(1300);
      numberToWords(data);
    }
        }else{
          Serial.print("dont talk");
        }

    }   
    } else {
      Serial.print("Failed to get data: ");
      Serial.println(fbdo.errorReason());
    }
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
       delay(800);
    //  Serial.println(" 1000");
       myDFPlayer.play(30);
       delay(800);
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
       delay(800);
     // Serial.println(" 100");
       myDFPlayer.play(29);
       delay(800);
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
       delay(800);
      return;
    } else if (digit > 1) {
     // Serial.print(tens_place[digit]);
      myDFPlayer.play(tens_place[digit]);
       delay(800);
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
       delay(800);
    }
  }
  myDFPlayer.play(35);
   delay(4500);
  myDFPlayer.play(1);
   delay(5300);
}
