#include <ESP8266WiFi.h> //ESP8266 Arduino library with built in functions
#include <ESP8266HTTPClient.h> 
#include<SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>
#include <dht11.h>
#include <Servo.h> // including servo library.

// Giving name to servo.

Servo servo1;
Servo servo2;

SoftwareSerial nodeMcu(D6,D5);

#define light1Pin D0
#define light2Pin D7
#define buzzerPin D1
#define servoPin D2
#define servoPin1 D8
#define lightWarningPin D3
#define DHT11PIN D4

#define ssid "HackedNetwork" // Replace with your network name
#define password "Wae@Hacker!1992" // Replace with your network password
const char* serverGetAll = "http://192.168.1.72:8080/NewRestAPI_war_exploded/api/devices";
const char* serverUpdateTemp = "http://192.168.1.72:8080/NewRestAPI_war_exploded/api/devices/temperature/";
const char* serverUpdateHumidity= "http://192.168.1.72:8080/NewRestAPI_war_exploded/api/devices/humidity/";

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 1000;

dht11 DHT11;
float temperature;
float humidity;

void setup() {
  // Attaching Servo
  servo1.attach(servoPin);
  servo2.attach(servoPin1);
  nodeMcu.begin(115200);
  pinMode(light1Pin, OUTPUT);
  pinMode(light2Pin, OUTPUT);
  pinMode(lightWarningPin,OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // put your setup code here, to run once:
Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

 String payload = "";
  DynamicJsonDocument doc(1024);
 if ((millis() - lastTime) > timerDelay) {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
   
    HTTPClient http ;
    http.begin(serverGetAll);
     // Send HTTP GET request
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
         payload = http.getString();
        // Free resources
        http.end();
      }
        else{
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      const char* msg = payload.c_str();
      Serial.println("as String");
      
        Serial.println(payload);
        
        Serial.println("as char");
        Serial.println(msg);
        
        JsonObject obj = doc.to<JsonObject>();
        // ArduinoJson 6
        Serial.println("as json");
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            // if the file didn't open, print an error:
            Serial.println(F("Error parsing JSON "));
            Serial.println(error.c_str());
            // create an empty JSON object
            obj = doc.to<JsonObject>();
        } else {
            // GET THE ROOT OBJECT TO MANIPULATE
            obj = doc.as<JsonObject>();
        }
        for (JsonPair p : obj) {
          const char* key = p.key().c_str();
          JsonVariant value = p.value();
          Serial.println(key);
       }
        int temp = obj["temperature"];
        int humidity = obj["humidity"];
        String lights = obj[F("lights")];
        String doors = obj[F("doors")];
        String windows = obj[F("windows")];
        boolean light1 = obj[F("lights")][0]["lightStatus"];
        boolean light2 = obj[F("lights")][1]["lightStatus"];
        boolean light3 = obj[F("lights")][2]["lightStatus"];
        boolean door = obj[F("doors")][0]["doorStatus"];
        boolean window = obj[F("windows")][0]["windowStatus"];
        Serial.println("//////////TEST/////////");
        Serial.println(lights);
        Serial.println(doors);
        Serial.println(windows);
        Serial.println(light1);
        Serial.println(light2);
        Serial.println(light3);
        Serial.println(window);
        Serial.println(door);
          int chk = DHT11.read(DHT11PIN);
        temperature = (float)DHT11.temperature;
        humidity = (float)DHT11.humidity;
        Serial.print("Humidity (%): ");
        Serial.println(humidity);
        Serial.print("Temperature (C): ");
        Serial.println(temperature);
          String dataToSend = String(temperature);
          nodeMcu.println (dataToSend);
          nodeMcu.println();
          String dataToSend1 = String(humidity);
          nodeMcu.println(dataToSend1);
        if (light1){
           digitalWrite(light1Pin,1);
          }else digitalWrite(light1Pin,0);
        if (light2){
           digitalWrite(light2Pin,1);
          }else digitalWrite(light2Pin,0);
        if (door){
           openDoor();
          }else closeDoor();
         if (window){
           openW();
          }
          else closeW();
        if(temperature>25&&light3)
            { 
              digitalWrite(lightWarningPin,1);
              tone(buzzerPin,5000,3000);
              }else{
                digitalWrite(lightWarningPin,0);
                noTone(buzzerPin);
            }
            
        int t=(int)temperature;
        String te = (String)t;
        String h=(String)humidity;
         Serial.println(t);
         Serial.println(h);
        
          HTTPClient http1 ;
    http1.begin(serverUpdateTemp+te);
    http1.addHeader("Content-Type", "text/plain");
     // Send HTTP GET request
      int httpResponseCode1 = http1.PUT(te);
      if (httpResponseCode1>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode1);
         payload = http1.getString();
         Serial.println(payload);
        // Free resources
        http1.end();
      }
        else{
        Serial.print("Error code: ");
        Serial.println(httpResponseCode1);
      }
              HTTPClient http2 ;
    http2.begin(serverUpdateHumidity+h);
    http2.addHeader("Content-Type", "text/plain");
     // Send HTTP GET request
      int httpResponseCode2 = http2.PUT(h);
      if (httpResponseCode2>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode2);
         payload = http2.getString();
         Serial.println(payload);
        // Free resources
        http2.end();
      }
        else{
        Serial.print("Error code: ");
        Serial.println(httpResponseCode2);
      }
       
       lastTime = millis();
      }
    else {
      Serial.println("WiFi Disconnected");
    }
    
  }
  delay(2000);
  
}
 void openDoor(){
    servo1.write (180); // Servo will move to degree angle.
    }
 void closeDoor(){
    servo1.write (0); // Servo will move to degree angle.
  }
    void closeW(){
    servo2.write (90); // Servo will move to degree angle.
    }
    void openW(){
    servo2.write (0); // Servo will move to degree angle.
    }
