#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include "DHT.h"
#include "Relay.h"
//#if defined(ESP8266) && !defined(D5)
#define DHTPIN 16 // pin for temperatureand humidity sensor board pin
                 // gnd - gnd: signal pin - D0: vcc-3V with 10kohm pull up resister

DHT dht(DHTPIN);
# define water_level_power 5 //power pin for water level sensor
# define water_level_pin 4   // input pin showing the reading of water level sensor board pin
                              // - - gnd: + - D1- pull up : signal - D2

# define soil_moisture_pin A0   //pin from soil moisture sensor board pin
                              // vcc -3v: gnd-gnd : signal -A0

# define relay_signal 14      // signal pin to relay board pin
                             // gnd- gnd: + - 5v: signal -D5

Relay relayModule(relay_signal);

int upperThreshold = 520;
int val=0;

// defines variables
int waterlevel;
int soil_moisture;
float temp;
int humidity;
WiFiClient client;
void callback(char* topic, byte* payload, unsigned int payloadLength);
// CHANGE TO YOUR WIFI CREDENTIALS
const char* ssid = "**********";
const char* password = "***********";
// CHANGE TO YOUR DEVICE CREDENTIALS AS PER IN Things Speak
unsigned long channelid=********;
const char* myWriteAPIKey="**********";

void setup()
{

   Serial.begin(115200);
   dht.begin();
   
   pinMode(soil_moisture_pin,INPUT); 
   pinMode(DHTPIN,INPUT);
   pinMode(water_level_power,OUTPUT);
   pinMode(water_level_pin,INPUT);
   pinMode(relay_signal,INPUT);
   Serial.println("Connecting to");
   WiFi.begin(ssid,password);
   while(WiFi.status()!=WL_CONNECTED)
   {
     delay(500);
     Serial.print(".");
   }
   Serial.print(" ");
   Serial.print("WiFi Connected ");
   ThingSpeak.begin(client);
   
}


int i=0;
int c=0;
int hours=0;
void loop()
{
  int days[]= {26,11,15,33,25,23,18};
  int low_temp[] = {16,16,16,25,20,20,20};
  int high_temp[] = {20,20,20,30,25,25,25};
  int low_level[] = {0,2,0,2,2,2,0};
  int high_level[] ={2,5,0,5,5,5,0}; 
  // put your main code here, to run repeatedly:
  c = days[i];
  //hours= c*24;  
  hours=3;  
  
//  char pump_on[200] = "pump on";
//  char pump_off[200] = "pump off";
 // char motor_on[200] = "motor on";
 // char motor_off[200] = "motor off";
  //char irri[200] = "irrigation";
  //char stop_irri[200] = "stop irrigation";
  waterlevel= 1;
  humidity=50;
  temp=30;
  while(hours >0){
   //humidity= dht.readHumidity();
    Serial.print("stage:");
    Serial.println(i);
    Serial.println(hours);
    
   //waterlevel= water_level();

   
   //temp= temperature();

   
   Serial.println(humidity);
   Serial.println(waterlevel);
   Serial.println(temp);
    if (humidity> 80){
      Serial.println("high humidity");
     
    }
    else if (humidity<60){
      Serial.println("low humidity");
      
    }
    
    if (i==2 || i==6){
      //int soil_moisture = analogRead(A0);
      soil_moisture = 20;
      
      if (soil_moisture< 30){
        Serial.print("irrigation");
       
  
        while(soil_moisture < 50){
          //soil_moisture = analogRead(A0);
          delay(1000);
          soil_moisture = 50;
        } 
        Serial.print("stop irrigation");   
     
     
      }
      if (soil_moisture> 50){
        Serial.print("stop irrigation");
               
  
      }
      if (waterlevel> 0){
        Serial.print("pump on");
        while(waterlevel >0){
          relayModule.on(); 
          //waterlevel= water_level();
          delay(1000);
          water_level=0;                // pump on 
        } 
        Serial.print("pump off");
        relayModule.off();                  // pump off
        
  
      }
      
    }
    else
    {
      if (waterlevel> high_level[i]){
        Serial.print("pump on");
        waterlevel= water_level();
        
        while(waterlevel>low_level[i]){
          waterlevel= water_level();
          relayModule.on();              //to switvh on the pump
        } 
        Serial.print("pump off");
        relayModule.off();              // switch off the pump
 
  
        
      }
      if (waterlevel<= low_level[i]){
        Serial.print("motor on");
   
  
        while(waterlevel < high_level[i]){
          waterlevel= water_level();
          relayModule.on();             // motor on
          delay(2000);
          waterlevel=5;
        } 
        
        Serial.print("motor off"); 
        relayModule.off();              // motor off
     
  
      }
    }
    if (temp> high_temp[i]){
      Serial.print("high temp");
    }
    if (temp< low_temp[i]){
      Serial.print("low temp");
    }
    //delay(3600000);
    //delay(2000);
  ThingSpeak.setField(1,soil_moisture);
  ThingSpeak.setField(2,humidity);
  ThingSpeak.setField(3,temp);
  ThingSpeak.setField(4,waterlevel);  
  ThingSpeak.writeFields(channelid,myWriteAPIKey);
  ThingSpeak.writeFields(channelid,myWriteAPIKey);
  ThingSpeak.writeFields(channelid,myWriteAPIKey);
  ThingSpeak.writeFields(channelid,myWriteAPIKey);
  delay(2000);
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
