/*
 * pubsubclient version 2.8.0 by Nic O'leary 
 * when can not connect to server led on board will turn on
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
/*buffer data pubish*/
#define Buffer 240

#define endln   Serial.println();
#define show    Serial.print
#define showln  Serial.println

#define ssid_WiFi  "CTU_IOTLAB"
#define pass_WiFi  "02710271"
#define mqtt_server  "192.168.137.1"

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);
  Serial.begin(115200);
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

void SerialRead(){
  String DataBuffer = "";
  while(Serial.available())
  {
    DataBuffer = Serial.readString();
    if(DataBuffer != "")
    {
        DataPublish("MQTT_DEBUG", DataBuffer);
        delay(50);
        DataPublish("MQTT_calib", DataBuffer);
    }
  }
}

/* -----------------------Program Function-----------------------*/

void WIFI_SETUP()
{
  digitalWrite(BUILTIN_LED, 0);
  delay(10);
//  show("Connecting to " + String(ssid_WiFi));
//  endln

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_WiFi, pass_WiFi);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    show(".");
  }

//  showln("\nWiFi Connected \nIP address: ");
//  showln(WiFi.localIP());
}

void DataPublish(char topic[], String DataUser)
{
  uint8_t value = 0;
  char msg[Buffer];
  char payload[DataUser.length()+1];
  for(int i=0; i<= DataUser.length(); ++i)
    payload[i] = DataUser[i];
  snprintf(msg, Buffer, payload);
  boolean boolPub = client.publish(topic, msg);
//  if(!boolPub)
    //showln("mistake to send data to server.");
}

void callback(char* topic, byte* payload, unsigned int length)
{
  String mess_payload = "";
  String topic_convert = String(topic);
  char msg_process[length+1];
  /* message for MQTT*/
  //show("Message [" + topic_convert + "]");
  for (int i = 0; i < length; i++)
    mess_payload += (char)payload[i];
  //endln
  //showln(mess_payload);

  if(topic_convert == "MQTT_read_data_sub")
    client.publish("mqtt_status", "request read");
  if(topic_convert == "MQTT_write_data_sub")
    client.publish("mqtt_status", "request write");
  if(topic_convert == "MQTT_click_config_sub")
    client.publish("mqtt_status", "request show configuration");
  Serial.print(mess_payload);
} /* end callback */

void MQTT_Reconnect()
{
  while (!client.connected())
  {
//    show("Connecting to MQTT Server...");
    if(WiFi.status() != WL_CONNECTED)
      WIFI_SETUP();
    if(client.connect("ESP"))
    {
      digitalWrite(BUILTIN_LED, 1);
//      showln("connected MQTT");
      client.publish("mqtt_status", "ESP connected");
      client.subscribe("MQTT_click_config_sub");
      client.subscribe("MQTT_write_data_sub");
      client.subscribe("MQTT_read_data_sub");
    }
    else{
//      showln("failed, rc = " + String(client.state()) + " try again in 5 seconds");
      delay(5000);
    }
  } /* end while */
} /* end reconnect MQTT*/
