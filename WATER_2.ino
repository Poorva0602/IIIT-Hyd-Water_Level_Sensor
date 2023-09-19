#include<ESP8266WiFi.h>
#include<MQTTClient.h>

WiFiClient net;
MQTTClient client;

char ssid[]="RAHUL4G";
char password[]="poorva2000";

const char *broker = "mqtt.thingspeak.com";
char mqttUserName[] = "water level sensor";
char mqttPassword[] = "U38LJPAI3JQQWZIC";

char publish_writeAPIKey[] = "KEOET2BJNAXSMHSI";
long publishChannelID = 1371990;
String publishTopic = "channels/" + String( publishChannelID ) + "/publish/" + String( publish_writeAPIKey );

void connect()
{
  Serial.print("/n Connecting to Wifi");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print("...");
    delay(5000);
  }
  char clientID[] = "ESP8266 00000000000";
  for(int i=8;i<18;i++)
  clientID[i]=char(48+random(10));

  Serial.print("\nConnecting to broker");
  while(!client.connect(clientID, mqttUserName, mqttPassword))
  {
    Serial.print("..");
    delay(5000);
  }
  Serial.print("\nMQTT broker Connected");
}

void setup(){
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  WiFi.begin(ssid, password);
  client.begin(broker, net);
  connect();
  }

 void loop(){
  client.loop();
  delay(10);
  if(!client.connected()){
    connect();
    return;
  }

  int sensor_value = analogRead(A0);
  String dataText = String("field1="+String(sensor_value));
  boolean result = client.publish(publishTopic, dataText);

  if(result)Serial.println("data has been sucessfully published");
  else Serial.println("Unable to publish data");
  delay(1000);
}
