#include <WiFi.h>
#include "time.h"
#include "Esp32MQTTClient.h"
#include "AzureIotHub.h"
#include <ArduinoJson.h>
#include <DHT.h>

#define INTERVAL 1000 *5
#define DHT_PIN 21
#define DEVICE_TYPE "dht"
#define DHT_TYPE DHT11

char * ssid = "TN_wifi_F45E33";
char *pass = "PD4XEEYPMP";
char *connectionString = "HostName=IoT20-hub.azure-devices.net;DeviceId=esp32;SharedAccessKey=G7t14A1Tyf4K6AGdD/ALY4UY6eph+lPE53bybeqsNIA=";

bool messagePending = false;
unsigned long prevMillis = 0;
unsigned long epochTime;
float prevTemp = 0;
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  initSerial();
  initWifi();
  initDHT();
  initDevice();
  delay(2000);

  configTime(3600,3600,"pool.ntp.org");
}
void loop() {
  unsigned long currentMillis = millis();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if(!messagePending){
    if((currentMillis - prevMillis) > INTERVAL){
      prevMillis = currentMillis;
      epochTime = time(NULL);

      if(!(std::isnan(temperature)) && !(std::isnan(humidity))){

        if(temperature > (prevTemp +1) || temperature < (prevTemp - 1)){
           prevTemp = temperature;
           char payload[256];
      
           DynamicJsonDocument doc(1024);
           doc["deviceType"] = DEVICE_TYPE;
           doc["temp"] = temperature;
           doc["hum"] = humidity;
           doc["ts"] = epochTime;
           
           
           
           serializeJson(doc, payload);
           SendMessage(payload);

        }
      }     
    }
  }
  Esp32MQTTClient_Check();
  delay(10);
}
