#include <esp_now.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h> 
#include <PubSubClient.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include "WiFi.h"
// #include "config.h"


WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
LiquidCrystal_I2C lcd(0x27,16,2); //LCD definaiton
uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t water[8] = {0x2, 0x3, 0x2, 0xE, 0x1E, 0xC, 0x0};

// Replace with your network credentials (STATION)
const char* ssid = "Lazarus";
const char* password = "95135728";

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float temp;
  float hum;
  float soil;
  unsigned int readingId;
} struct_message;

struct_message incomingReadings;

JSONVar board;
// String jsonString;
// Variables for HTTP POST request data.
String postData = ""; //--> Variables sent for HTTP POST request data.
String payload = "";  //--> Variable for receiving response from HTTP POST.

//i Defines the Digital Pin of the "On Board LED".
#define ON_Board_LED 13 

// Defines GPIO 13 as LED_1.
#define LED_01 2 

// Defines GPIO 12 as LED_2.
#define LED_02 12

char tempStr[10];
char humStr[10];
char soilStr[10];
int soil_lvl;
String send_Status_Read_DHT11 = "SUCCEED";


float send_Temp;
float send_Humd;
float send_Soil;

//........................................ The process of sending the DHT11 sensor data and the state of the LEDs to the database.
String LED_01_State = "";
String LED_02_State = "";
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
    if(connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  // Copies the sender mac address to a string
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
  sprintf(tempStr, "%.1f", incomingReadings.temp);
  sprintf(humStr, "%.1f", incomingReadings.hum);
  sprintf(soilStr, "%.1f", incomingReadings.soil);
  soil_lvl=incomingReadings.soil;
  lcd.setCursor(0, 0);
  lcd.print("H: ");
  lcd.print(humStr);
  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(tempStr);
  lcd.setCursor(9, 0);
  lcd.print("S: ");
  lcd.print(soilStr);
  lcd.setCursor(9, 1);
  lcd.write((byte)4); 
  Serial.printf("Board ID %u: %u bytes\n", incomingReadings.id, len);
  // if(soil_lvl<=20 || strcmp(myObject["LED_01"], "ON") == 0)
  if(soil_lvl<=20){
    digitalWrite(LED_01, HIGH);
    Serial.println("LED 01 ON");
    LED_01_State = "ON";
    lcd.setCursor(12, 1);
    lcd.write((byte)1); 
  }
  // if(soil_lvl>=80 && strcmp(myObject["LED_01"], "OFF") == 0)
  if(soil_lvl>=60){
    digitalWrite(LED_01, LOW);
    Serial.println("LED 01 OFF");
    LED_01_State = "OFF";
    lcd.setCursor(12, 1);
    lcd.write((byte)2); 
  }
  if(soil_lvl >20){    
    digitalWrite(LED_01, LOW);
    delay(2000);
    if(soil_lvl > 60){
      digitalWrite(LED_01,LOW);
    }
    if(soil_lvl >20 && soil_lvl < 30){
      digitalWrite(LED_01, HIGH);
    }
  }
}

void control_LEDs() {
  Serial.println();
  Serial.println("---------------control_LEDs()");
  JSONVar myObject = JSON.parse(payload);

  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    Serial.println("---------------");
    return;
  }
  
  if(strcmp(myObject["LED_02"], "ON") == 0)   {digitalWrite(LED_02, HIGH);  Serial.println("LED 02 ON"); }
  if(strcmp(myObject["LED_02"], "OFF") == 0)  {digitalWrite(LED_02, LOW);   Serial.println("LED 02 OFF");}

  Serial.println("---------------");

  if (myObject.hasOwnProperty("LED_01")) {
    Serial.print("myObject[\"LED_01\"] = ");
    Serial.println(myObject["LED_01"]);
  }

  if (myObject.hasOwnProperty("LED_02")) {
    Serial.print("myObject[\"LED_02\"] = ");
    Serial.println(myObject["LED_02"]);
  }
}

void get_DHT11_sensor_data() { 
  sprintf(tempStr, "%.1f", incomingReadings.temp);
  sprintf(humStr, "%.1f", incomingReadings.hum);
  sprintf(soilStr, "%.1f", incomingReadings.soil);
  
  send_Temp = incomingReadings.temp;
  send_Humd = incomingReadings.hum;
  send_Soil = incomingReadings.soil;
}

void setup() {
  
  digitalWrite(LED_01, LOW);
  // Initialize Serial Monitor
  Serial.begin(115200);
  //Setting up hardware paripheral

  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output.
  pinMode(LED_01,OUTPUT); //--> LED_01 port Direction output.
  pinMode(LED_02,OUTPUT); //--> LED_02 port Direction output.
  // pinMode(PUMP,OUTPUT); //--> LED_02 port Direction output.
  
  digitalWrite(ON_Board_LED, HIGH); //--> Turn on Led On Board.
  digitalWrite(LED_01, HIGH); //--> Turn on LED_01.
  digitalWrite(LED_02, HIGH); //--> Turn on LED_02.
  // digitalWrite(PUMP, HIGH); //--> Turn on LED_02.

  digitalWrite(ON_Board_LED, LOW); //--> Turn off Led On Board.
  digitalWrite(LED_01, LOW); //--> Turn off Led LED_01.
  digitalWrite(LED_02, LOW); //--> Turn off Led LED_02.
  // digitalWrite(PUMP, HIGH); //--> Turn on LED_02.

  lcd.init(); // initialize the lcd 
  lcd.backlight();
  lcd.createChar(1, check);
  lcd.createChar(2, cross);
  lcd.createChar(3, bell);
  lcd.createChar(4, water);
  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  
  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
    if(connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}
 

void loop() {
  if(WiFi.status()== WL_CONNECTED) {
  Serial.print("soil level is :" );
  Serial.println(soil_lvl);
  // delay(1000);
  HTTPClient http;  //--> Declare object of class HTTPClient.
  int httpCode;     //--> Variables for HTTP return code.
  
  //........................................ Process to get LEDs data from database to control LEDs.
  postData = "id=esp32_01";
  payload = "";
  Serial.println();
  Serial.println("---------------getdata.php");
  // Example : http.begin("http://192.168.0.0/ESP32_MySQL_Database/Final/getdata.php");
  http.begin("http://13.55.247.22/cfiot/Final/getdata.php");  //--> Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");        //--> Specify content-type header
  
  httpCode = http.POST(postData); //--> Send the request
  payload = http.getString();     //--> Get the response payload

  Serial.print("httpCode : ");
  Serial.println(httpCode); //--> Print HTTP return code
  Serial.print("payload  : ");
  Serial.println(payload);  //--> Print request response payload
  
  http.end();  //--> Close connection
  Serial.println("---------------");
  digitalWrite(ON_Board_LED, LOW);
  //........................................ 

  // Calls the control_LEDs() subroutine.
  control_LEDs();
  
  // delay(5000);

  // Calls the get_DHT11_sensor_data() subroutine.
  get_DHT11_sensor_data();

  //........................................ The process of sending the DHT11 sensor data and the state of the LEDs to the database.
  // String LED_01_State = "";
  // String LED_02_State = "";talRead(LED_02) == 1) LED_02_State = "ON";
  // if (digitalRead(LED_02) == 0) LED_02_State = "OFF";
  
  // static unsigned long lastEventTime = millis();
  // static const unsigned long EVENT_INTERVAL_MS = 60000;
  // if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {

  Serial.printf("t value: %4.2f \n", incomingReadings.temp);
  Serial.printf("h value: %4.2f \n", incomingReadings.hum);
  Serial.printf("s value: %4.2f \n", incomingReadings.soil);
  Serial.printf("readingID value: %d \n", incomingReadings.readingId);
  Serial.println();
  // lastEventTime = millis();

  postData = "id=esp32_01";
  postData += "&temperature=" + String(send_Temp);
  postData += "&humidity=" + String(send_Humd);
  postData += "&soil=" + String(send_Soil);
  postData += "&status_read_sensor_dht11=" + send_Status_Read_DHT11;
  postData += "&led_01=" + LED_01_State;
  postData += "&led_02=" + LED_02_State;
  payload = "";

  digitalWrite(ON_Board_LED, HIGH);
  Serial.println();
  Serial.println("---------------updateDHT11data_and_recordtable.php");
  // Example : http.begin("http://192.168.0.0/ESP32_MySQL_Database/Final/updateDHT11data_and_recordtable.php");
  http.begin("http://13.55.247.22/cfiot/Final/updateDHT11data_and_recordtable.php");  //--> Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //--> Specify content-type header

  httpCode = http.POST(postData); //--> Send the request
  payload = http.getString();  //--> Get the response payload

  Serial.print("httpCode : ");
  Serial.println(httpCode); //--> Print HTTP return code
  Serial.print("payload  : ");
  Serial.println(payload);  //--> Print request response payload
  
  http.end();  //Close connection
  Serial.println("---------------");
  digitalWrite(ON_Board_LED, LOW);
  // delay(4000);
    // }
  }
}
