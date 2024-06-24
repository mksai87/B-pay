#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "#mksai"
#define WIFI_PASSWORD "2444666666"
#define API_KEY "AIzaSyBGBn0z5_3Qo6OUrK3xARB75ttcE1oiuFg"
#define DATABASE_URL "https://smsbp-a5303-default-rtdb.firebaseio.com/" 

String parent ="smslist";
String child="7396151573";
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
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
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    // Read data from Firebase
    String path = "/" + parent + "/" + child;
    if (Firebase.RTDB.getString(&fbdo, path)){
      if (fbdo.dataType() == "string"){
        Serial.print("Data from ");
        Serial.print(path);
        Serial.print(": ");
        Serial.println(fbdo.stringData());
      }
    } else {
      Serial.print("Failed to get data: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
