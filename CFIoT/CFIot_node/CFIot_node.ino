#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
const int potPin = 34;
float potValue = 0;

// This setting is for deep sleep
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */


// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 1

RTC_DATA_ATTR int bootCount;
int retry;
// Digital pin connected to the DHT sensor
#define DHTPIN 15  

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// REPLACE WITH YOUR RECEIVER MAC AddressFC:F5:C4:19:77:D0
uint8_t broadcastAddress[] = {0xFC, 0xF5, 0xC4, 0x19, 0x77, 0xD0};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id;
    float temp;
    float hum;
    float soil;
    int readingId;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 3000;        // Interval at which to publish sensor readings

unsigned int readingId = 0;

esp_now_peer_info_t peerInfo;

// Insert your SSID
constexpr char WIFI_SSID[] = "Lazarus";

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}

float readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }
  else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }
  else {
    Serial.println(h);
    return h;
  }
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  esp_sleep_wakeup_cause_t wakeup_reason;
  if (status == ESP_NOW_SEND_SUCCESS) {
  Serial.println("Go to sleep");
  delay(500);
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");}
  else {
  retry++;
  Serial.println("Not Going to sleep");
  Serial.print("try : ");
  Serial.println(retry);
  delay(2000); 
  if(retry >= 10){
    Serial.println("System Reboot");
    delay(500);
    ESP.restart();
  }}

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

// esp_now_peer_info_t peerInfo; 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(potPin, INPUT);
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  // print_wakeup_reason();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up every 5 seconds
  */
  int TIME_TO_SLEEP;
  if (bootCount == 1) {
    // TIME_TO_SLEEP = 11600; //once in 6 Hours
    TIME_TO_SLEEP = 60;
  }
  if (bootCount == 2) {
    // TIME_TO_SLEEP = 1800; //Once in 30 minutes
    TIME_TO_SLEEP = 5;
  }
  if (bootCount == 3) {
    // TIME_TO_SLEEP = 300; //Once in 10 minutes 
    TIME_TO_SLEEP = 2;
  }
  if (bootCount == 4) {
    // TIME_TO_SLEEP = 10800; //once in 3 Hours
    TIME_TO_SLEEP = 20;}
  if (bootCount == 0) {
    // TIME_TO_SLEEP = 10800; //once in 3 Hours
    TIME_TO_SLEEP = 2;}
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");

 
  dht.begin();
 
  // Set device as a Wi-Fi Station and set channel
  WiFi.mode(WIFI_STA);

  int32_t channel = getWiFiChannel(WIFI_SSID);

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    //Set values to send
    float analValue = analogRead(potPin);
    myData.id = BOARD_ID;
    myData.temp = readDHTTemperature();
    myData.hum = readDHTHumidity();
    myData.soil = potValue = ( 100 - ( (analValue/4095.00) * 100 ) );
    myData.readingId = readingId++;
     
    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    if (result == ESP_OK) {
      potValue = ( 100 - ( (analValue/4095.00) * 100 ) );
      Serial.println("Sent with success");
      Serial.print("soil Value : ");
      Serial.println(potValue);
      if(potValue >= 80){
        // bootCount==0;
        bootCount = 1;
        Serial.println("boot1 Soil Wet");
      }
      if(potValue >40 && potValue <80 ){
        bootCount = 2;
        // digitalWrite(ledPin, HIGH);  // Turn on the LED
        Serial.println("boot2 Watering");
      }
      if(potValue >0 && potValue <20){
        // bootCount==2;
        bootCount = 3;
        // digitalWrite(ledPin, HIGH);  // Turn on the LED
        Serial.println("boot3 Soil Dry");
      }
    }
    else {
      Serial.println("Error sending the data");
    }
  }
}