#include<ESP8266WiFi.h>
#include<MQTTClient.h>
#include<ThingSpeak.h>

WiFiClient net;
MQTTClient client;

char ssid[] = "RAHUL 4G";
char password[] = "poorva2000";

const char* broker = "mqtt.thingspeak.com";
char mqttUserName[] = "MQTT";
char mqttPass[] = "EE9KGADM8XATJO0Q";

char publish_writeAPIKey[] = "KEOET2BJNAXSMHSI";
long publishChannelID =   1371990;
String publishTopic = "channels/" + String( publishChannelID ) + "/publish/" + String( publish_writeAPIKey );


void connect()
{
  Serial.print("\nconnecting to wifi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("....");
    delay(5000);
  }
Serial.print("\nMQTT broker Connected\n");
}

void setup()
{
  Serial.begin(9600);
  pinMode(A0,OUTPUT);
  WiFi.begin(ssid, password);
  client.begin(broker, net);
  connect();
}

void loop()
{
  client.loop();
  delay(10);
  if(!client.connected())
  {
    connect();
    return;
  }


  int sensor_Value = random(A0);
  String dataText = String("field1=" + String(sensor_Value));
  boolean result = client.publish(publishTopic, dataText);
  
  if (result) Serial.println("data has been published succesfully");
  else Serial.println("unable to publish data");
  delay(10000);
}
