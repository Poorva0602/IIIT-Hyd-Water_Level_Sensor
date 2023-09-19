#include<ESP8266WiFi.h>
#include<MQTTClient.h>
WiFiClient net;
MQTTClient client;
const char*ssid="RAHUL 4G";//wifi ssid
const char*pass="poorva2000";//wpa2 key

//MQTT Broker Settings
const char* broker = "mqtt.thingspeak.com";
char mqttUserName[] = "Water Level Sensing";
char mqttPass[] = "U38LJPAI3JQQWZIC";

//Published settings
char publish_writeAPIKey[] = "KEOET2BJNAXSMHSI";
long publishChannelID = 1371990;
String publishTopic = "channels/" + String( publishChannelID ) + "/publish/"+String(publish_writeAPIKey);

// subscribe channel 1 commands
long subscribeChannelID_1 = 1372002;
String subscribeTopicFor_Command_1 ="channels/"+String(subscribeChannelID_1)+"/subscribe/fields/field1";

const unsigned long postingInterval = 20L * 1000L;//post data every 20 seconds


//------variables------
unsigned long lastUploadedTime = 0;

//----connect
void connect(){
  Serial.print("\nconnecting to wifi!");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(5000);
  }
  Serial.print("\nConnected to wifi!");
  //----MQTT CLIENT ID
  char clientID[] ="ESP8266_0000000000";//for random operation of client ID
  for (int i = 8; i<18; i++) clientID[i]= char(48+random(10));

  Serial.print("\nconnecting to broker!");

  while (!client.connect(clientID,mqttUserName,mqttPass)){
    Serial.print(".");
    delay(15000);
  }
  Serial.println("\nMQTT broker connected!");
  client.subscribe(subscribeTopicFor_Command_1);
  
  }


//=========message REcieved
void messageReceived(String &topic, String &payload){
  Serial.println("incoming: "+ topic + "-" + payload);
  //--check for vituino Command
  if (topic==subscribeTopicFor_Command_1){
    Serial.println("Command 1 = "+payload);
    int v =payload.toInt();
    if (v>0) digitalWrite(LED_BUILTIN,HIGH);
    else digitalWrite(LED_BUILTIN,LOW);
  }
}
//======Setup


  
void setup(){
  Serial.begin(9600);
  pinMode(A0,INPUT);
  
  pinMode(LED_BUILTIN,OUTPUT);
  WiFi.begin(ssid, pass);
  client.begin(broker, net);
  client.onMessage(messageReceived);
  connect();
}
//======LOOP

void loop(){
  client.loop();
  delay(10);
  if(!client.connected()){
    connect();
    return;
  }
int sensorValue_1 = analogRead(A0);;//Reading water sensor analog values connected at digital pin D1
  Serial.print(sensorValue_1);
  Serial.println("\n");
  int sensorValue_2 = analogRead(A0)/2;;
  Serial.print(sensorValue_2);
  String dataText = String("field1=" + String(sensorValue_1)+ "&field2=" + String(sensorValue_2));

  boolean result = client.publish(publishTopic, dataText);
  if (result) Serial.println("\nData has been published succesfully");
  else Serial.println("\nUnable to publish data");

  lastUploadedTime = millis();

}
