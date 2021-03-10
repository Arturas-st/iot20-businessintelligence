 void initSerial(){
  Serial.begin(115200);
  delay(2000);
  Serial.println("Serial initialized.");
}

void initWifi(){
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
  }
  Serial.print("WiFi initialized.");
}

void initDHT(){
  dht.begin();
  Serial.println("DGT Initialized.");
}
