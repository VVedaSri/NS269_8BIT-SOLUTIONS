#include "DHT.h"
#include "SoilMoisture.h"
#define DHTPIN 4 // pin for temperatureand humidity sensor

DHT dht(DHTPIN);
# define water_level_power 16 //power pin for water level sensor
# define water_level_pin 17   // input pin showing the reading of water level sensor

# define soil_moisture_pin 12 //pin from soil moisture sensor

SoilMoisture soilMoisture_3v3(soil_moisture_pin);

int upperThreshold = 520;
int val=0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();    // temp sensor starts
  pinMode(water_level_power,OUTPUT); 
  

}
int i=0;
int c=0;
int hours=0;
void loop() {
  int days[]= {26,11,15,33,25,23,18};
  int low_temp[] = {16,16,16,25,20,20,20};
  int high_temp[] = {20,20,20,30,25,25,25};
  int low_level[] = {0,2,0,2,2,2,0};
  int high_level[] ={2,5,0,5,5,5,0}; 
  // put your main code here, to run repeatedly:
  c = days[i];
  hours= c*24;
 
  while(hours >=0){
   float h= dht.readHumidity();
   int a= water_level();
   float b= temperature();
    if (h> 80){
      Serial.print("high humidity");
    }
    else if (h<60){
      Serial.print("low humidity");
    }
    if (i==2 && i==6){
      int soil_moist = soilMoisture_3v3.read();
      if (soil_moist> 80){
        Serial.print("stop irrigation");
      }
      if (soil_moist<60){
        Serial.print("irrigation");
      }
    }
    else
    {
      if (a> high_level[i]){
        Serial.print("pump on");
      }
      if (a< low_level[i]){
        Serial.print("motor on");
      }
    }
    if (b> high_temp[i]){
      Serial.print("high temp");
    }
    if (b< low_temp[i]){
      Serial.print("low temp");
    }
    delay(3600000);
    hours = hours -1;
  }
  i=i+1;   
}

int water_level(){
  int value=0;
  digitalWrite(water_level_power, HIGH);
  delay(10);
  value= analogRead(water_level_pin);
  value = map(value,520,0,6,0);
  digitalWrite(water_level_power, LOW);
  return value;
}

float temperature(){
  
  float t= dht.readTemperature();
  return t;
}






