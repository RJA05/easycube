#include <Adafruit_BMP085.h>
#include <Adafruit_INA219.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include "index.h"
WebServer server(80);
const char* ssid = "Planta Alta1";
const char* password = "nico3009";

Adafruit_INA219 ina219;
Adafruit_BMP085 bmp;

#define LIGHTSENSORPIN A0

byte s_check_timer =10;
bool ina_config_set = false;
bool ina_online=false;
bool temt_online=false;
bool bmp_online=false;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

float temperature;
float pressure;

int light_reading=0;



void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleLight() {
 server.send(200, "text/plane", String(light_reading)); //Send ADC value only to client ajax request
}
void handleTemp() {
 server.send(200, "text/plane", String(temperature));
}
void handlePressure() {
 server.send(200, "text/plane", String(pressure));
}

void setup() {
  Serial.begin(115200);
  // while (!Serial) {
  //     // will pause Zero, Leonardo, etc until serial console opens
  //   delay(1);
  // }
  pinMode(LIGHTSENSORPIN, INPUT); 
  
//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  server.on("/", handleRoot);      //This is display page
  server.on("/readLightSensor", handleLight);//To get update of ADC Value only
  server.on("/readTemperature", handleTemp);
  server.on("/readPressure", handlePressure);
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}



void sensor_check(){

  //sensor check
  s_check_timer += 1;
  if (s_check_timer >= 5){//this defines how many loops go through before we check the sensors are online
    if (! ina219.begin()) { //CHECK INA----------------------------------------------
      Serial.println("Failed to find INA219 chip");
      ina_online=false;//set the sensor state to appear offline
    }else if (!ina_config_set) {//if it is online and it is the first time it goes online
      ina219.setCalibration_16V_400mA();//ina calibration
      Serial.println("setup ina config ");
      ina_config_set =true;//set the config set to true since the sensor has been configurated
    }else {ina_online=true;}//if sensor is online set the online state to true

    if (!bmp.begin()) {//CHECK BMP ---------------------------------------------------
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    bmp_online=false;
    }else {bmp_online=true;}//if sensor is online set the online state to true


    if (light_reading ==0){//CHECK IF LIGHT SENSOR IS OFFLINE OR SEEING NO LIGHT------------
      Serial.println("light sensor offline");
      temt_online=false;//set the sensor state to appear offline
    }else {temt_online=true;}//if sensor is online set the online state to true

    s_check_timer = 0;//reset timer
  }

}

void sensor_read(){

  // check which sensors are online to display their readings
  if (ina_online){//ina219 readings----------------
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    Serial.println("----------------");
    Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  }
  if (bmp_online){//bmp180 readings-----------------
    temperature =bmp.readTemperature();
    pressure=bmp.readPressure();
    Serial.println("----------------");
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
   Serial.print(pressure);
   Serial.println(" Pa");
  }

  //read the light sensor and calculate values
  light_reading = analogRead(LIGHTSENSORPIN); //Read light level
  float square_ratio = light_reading / 1023.0; //Get percent of maximum value (1023)
  square_ratio = pow(square_ratio, 2.0); //idk lmao
  if (temt_online){//temt6000 readings --------------
    Serial.println("----------------");
    Serial.print("Light level: ");
    Serial.println(light_reading); 
  }
}

void loop() {


  sensor_check();
  sensor_read();



  delay(200);//wait as to not destroy the cpu lmao
  Serial.println("*");//idk space every time the loop goes
  server.handleClient();

}
//cacaaaaaaaaaaaaaaaaaaaaaa