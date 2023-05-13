#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
 
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
 
#define lamp 2
 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
 
 
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
   Serial.println();
 
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]); // Print payload content
  }
    char addr = (char)payload[62]; // read the led address
    Serial.print("Address: ");
    Serial.println(addr);
    char led = (char)payload[64]; // Eread the following command (1 for turn on 0 for turn off)
    Serial.print("Command: ");
    Serial.println(led);
 
  if ((led == 49) && (addr == 97)) // 49 is the ASCI value of 1
  {
    digitalWrite(lamp, HIGH);
    Serial.println("Lamp_A changed to HIGH");
  }
  else if ((led == 48) && (addr == 97)) // 48 is the ASCI value of 0
  {
    digitalWrite(lamp, LOW);
    Serial.println("Lamp_A changed to LOW");
  }
  if ((led == 49) && (addr == 98)) // 49 is the ASCI value of 1
  {
    digitalWrite(lamp, HIGH);
    Serial.println("Lamp_B changed to HIGH");
  }
  else if ((led == 48) && (addr == 98)) // 48 is the ASCI value of 0
  {
    digitalWrite(lamp, LOW);
    Serial.println("Lamp_B changed to LOW");
  }
  Serial.println();
}
 
void setup()
{
  Serial.begin(115200);
  connectAWS();
  pinMode (lamp, OUTPUT);
  digitalWrite(lamp, LOW);
 
}
 
void loop()
{
  client.loop();
  delay(1000);
}
