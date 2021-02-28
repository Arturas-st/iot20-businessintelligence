#include <WiFi.h>
#include "AzureIotHub.h"
//#include "Esp32MQTTClient.h"
#include <Esp32MQTTClient.h>
#include <RH_ASK.h>
#include <SPI.h>
//#include "ArduinoJson.h"
#include <ArduinoJson.h>
#include <DHT.h>
#include "time.h"

#define INTERVAL 5000
#define DEVICE_ID "esp32"
#define MESSAGE_LEN_MAX 256
#define DHT_TYPE DHT11
#define DHT_PIN 21

 char * ssid = "TN_wifi_F45E33";
 char *pass = "PD4XEEYPMP";
 char *connectionString = "HostName=IoT20-hub.azure-devices.net;DeviceId=device4;SharedAccessKey=Src3XkaU+PyxnUnrWSr5tedpUCiqY51T9WuNQ29ncus=";
 bool messagePending = false;

 const char* ntpServer = "pool.ntp.org";
 const long  gmtOffset_sec = 3600;
 const int   daylightOffset_sec = 3600;

 void initWifi(){
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
  }
 }
 void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result){
  if(result == IOTHUB_CLIENT_CONFIRMATION_OK){
    Serial.println("Confirmed");
    messagePending = false;
  }
 }

DHT dht(DHT_PIN, DHT_TYPE);
 
void setup() {
  Serial.begin(115200);
  dht.begin();
  initWifi();

  Esp32MQTTClient_Init((uint8_t *)connectionString, true);
  Esp32MQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop() {
 if(!messagePending){
   messagePending = true;

  delay(2000);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
   char payload[MESSAGE_LEN_MAX];
   char timeStamp[24];
   strftime(timeStamp, 24, "%F" "%R", &timeinfo);

  DynamicJsonDocument doc(sizeof(payload));
  doc["device"] = DEVICE_ID;
  doc["temperature"] = dht.readTemperature();
  doc["humidity"] = dht.readHumidity();
  doc["time"] = timeStamp;
  serializeJson(doc, payload);
  Serial.println(payload);
  
  EVENT_INSTANCE *message = Esp32MQTTClient_Event_Generate(payload, MESSAGE);
  Esp32MQTTClient_SendEventInstance(message);
  

  delay (INTERVAL);
 }

}
