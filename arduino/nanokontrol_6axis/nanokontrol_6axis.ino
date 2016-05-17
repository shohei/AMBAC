#include <Arduino.h>
#include <String.h>
#include <ArduinoJson.h>
#include "position.h"
#include "Command.h"
#include "Configuration.h"
#include "Preference.h"
#include "HAL.h"

void setupStepperPin(Preference* pref, int motorNumber, int step, int dirPin, int stepPin, int enablePin){
  pref->motor[motorNumber-1].dirPin = dirPin;
  pref->motor[motorNumber-1].stepPin = stepPin;
  pref->motor[motorNumber-1].enablePin = enablePin;
  pinMode(enablePin,OUTPUT);
  digitalWrite(enablePin,LOW);
}

void setupSteppers(){
  Preference *pref = Preference::getInstance();
  setupStepperPin(pref,1,200,62,63,48);//X
  setupStepperPin(pref,2,200,64,65,46);//Y->XX
  setupStepperPin(pref,3,200,66,67,44);//Z->Y
  setupStepperPin(pref,4,200,28,36,42);//E0->YY
  setupStepperPin(pref,5,200,41,43,39);//E1->Z
  setupStepperPin(pref,6,200,47,32,45);//E2->ZZ
}

void setup()
{
  Serial.begin(115200);    // the GPRS baud rate
  while (!Serial) {
    // wait serial port initialization
  }
  Serial.println("Serial initialized.");
  delay(500);
  pinMode(13,OUTPUT);//for debug
  setupSteppers();
  HAL::setupTimer();
  HAL::startTimer();
}

void loop()
{
  String response;
  bool begin = false;
  bool end = false;

  while (!end) {
    if (Serial.available() > 0)
    {
      begin = true;
      response = Serial.readStringUntil('\n');
      // Serial.println(response);
      end = true;
    }
  }

  const char *charBuf = response.c_str();
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(charBuf);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

   const char* dest1 = root["dest1"];
   const char* dest2 = root["dest2"];
   const char* dest3 = root["dest3"];
   const char* dest4 = root["dest4"];
   const char* dest5 = root["dest5"];
   const char* dest6 = root["dest6"];

   Position *pos = Position::getInstance();
   pos->cur1 = pos->dest1;
   pos->cur2 = pos->dest2;
   pos->cur3 = pos->dest3;
   pos->cur4 = pos->dest4;
   pos->cur5 = pos->dest5;
   pos->cur6 = pos->dest6;
   pos->dest1 = atoi(dest1);
   pos->dest2 = atoi(dest2);
   pos->dest3 = atoi(dest3);
   pos->dest4 = atoi(dest4);
   pos->dest5 = atoi(dest5);
   pos->dest6 = atoi(dest6);

   //dump destination when received command
   showDestination();
 }

void showDestination(){
  Position *pos = Position::getInstance();
  Serial.print("destination1: ");
  Serial.println(pos->dest1);
  Serial.print("destination2: ");
  Serial.println(pos->dest2);
  Serial.print("destination3: ");
  Serial.println(pos->dest3);
  Serial.print("destination4: ");
  Serial.println(pos->dest4);
  Serial.print("destination5: ");
  Serial.println(pos->dest5);
  Serial.print("destination6: ");
  Serial.println(pos->dest6);
}
