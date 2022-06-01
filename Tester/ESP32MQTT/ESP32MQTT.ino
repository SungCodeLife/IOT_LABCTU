#include "user_cfg.h"

// prototype function
void MQTT_Reconnect();
void WIFI_SETUP();
void SerialRead();
void DataPublish(char topic[], String DataUser);
inline void DebugLog_Show(String content);

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);
    LED(OFF);
    Serial.begin(BAUDRATE2);
    WIFI_SETUP();
    /* setup Server */
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{
    if (!client.connected())
      MQTT_Reconnect();
    client.loop();
    SerialRead();
}

void SerialRead()
{
    String DataBuffer = "";
    while(Serial.available())
    {
        DataBuffer = Serial.readString();
        if(DataBuffer != "")
        {
            DataPublish("MQTT_DEBUG", DataBuffer);
            delay(100);
            DataPublish("MQTT_Data_Pub", DataBuffer);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length)
{
    String mess_payload = "";
    char msg_process[length+1];

    DebugLog_Show("Message [" + (String)topic + "]");
    for (int i = 0; i < length; i++)
        mess_payload += (char)payload[i];
    DebugLog_Show("");
    DebugLog_Show(mess_payload);

} /* end callback */

/* -----------------------Program Function-----------------------*/
void WIFI_SETUP()
{
    LED(OFF);
    delay(10);
    DebugLog_Show("Connecting to " + String(ssid_WiFi));
    DebugLog_Show("");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_WiFi, pass_WiFi);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        show(".");
    }
    show("\nWiFi Connected \nIP address: ");
    showln(WiFi.localIP());
}

void DataPublish(char topic[], String DataUser)
{
    uint8 value = 0;
    char msg[Buffer];
    char payload[DataUser.length()+1];
    for(int i=0; i<= DataUser.length(); ++i)
        payload[i] = DataUser[i];
    snprintf(msg, Buffer, payload);
    boolean boolPub = client.publish(topic, msg);
    if(!boolPub)
        DebugLog_Show("mistake to send data to server.");
}

void MQTT_Reconnect()
{
  while (!client.connected())
  {
    DebugLog_Show("Connecting to MQTT Server...");
    if(WiFi.status() != WL_CONNECTED)
        WIFI_SETUP();
    if(client.connect("ESP"))
    {
        LED(ON);
        DebugLog_Show("connected MQTT");
        client.publish("mqtt_status", "ESP connected");
        client.subscribe("MQTT_Data_Sub");
    }
    else{
        DebugLog_Show("failed, rc = " + String(client.state()) + " try again in 5 seconds");
        delay(5000);
    }
  } /* end while */
} /* end reconnect MQTT*/

inline void DebugLog_Show(String content)
{
    #if DEBUG_LOG
        showln(content);
    #endif
}
