#include "DFRobot_EC.h"
#include <EEPROM.h>

#define EC_PIN A1
float voltage,ecValue,temperature = 25;
DFRobot_EC ec;
float calibration = 20.91; //change this value to calibrate
const int analogInPin = A0;
int sensorValue = 0;
unsigned long int avgValue;
float b;
int buf[10],temp;
void setup() {
  Serial.begin(9600);
  ec.begin();
}
void loop() {
static unsigned long timepoint = millis();
//time interval: 1s
if(millis()-timepoint>1000U)  {
  for(int i=0;i<10;i++){
  buf[i]=analogRead(analogInPin);
  delay(30);
  }
  for(int i=0;i<9;i++){
  for(int j=i+1;j<10;j++){
  if(buf[i]>buf[j]){
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
  }
  }
  }
  avgValue=0;
  for(int i=2;i<8;i++)
  avgValue+=buf[i];
  float pHVol=(float)avgValue*5.0/1024/6;
  float phValue = -5.70 * pHVol + calibration;
  timepoint = millis();
  voltage = analogRead(EC_PIN)/1024.0*5000;   // read the voltage
  //temperature = readTemperature();          // read your temperature sensor to execute temperature compensation
  ecValue =  ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
  Serial.print(String(millis()) + ",");
  Serial.print(String(phValue)+",");
  Serial.println(String(ecValue));
  }
  ec.calibration(voltage,temperature);  
}
