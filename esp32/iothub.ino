void initDevice(){
  Esp32MQTTClient_Init((uint8_t *)connectionString, true);
  Esp32MQTTClient_SetSendConfirmationCallback(SendCallback);
}

void SendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result){
  if(result == IOTHUB_CLIENT_CONFIRMATION_OK){
    Serial.println("Message confirmed.");
    messagePending = false;
  }
}

void SendMessage (char *payload){
  messagePending = true;
  EVENT_INSTANCE *message = Esp32MQTTClient_Event_Generate(payload, MESSAGE);
  Esp32MQTTClient_Event_AddProp(message, "School", "Nackademin");
  Esp32MQTTClient_Event_AddProp(message, "Name", "Arturas");
  Esp32MQTTClient_SendEventInstance(message);
}
