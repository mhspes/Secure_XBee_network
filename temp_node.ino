#include <XBee.h>
#include <DHT.h>

#define DHTTYPE DHT22 // Temp&hum sensor
#define dataPin 10    // Pin for reading sensor data
#define powerPin 8    // Pin for powering the sensor

XBee xbee = XBee();   // Initialize XBee, DHT sensor
DHT dht(dataPin,DHT22);
uint8_t data[8];

// Packet for transmitting (address 0x1000 - base station)
uint16_t addr16 = 0x1000;
Tx16Request tx = Tx16Request(addr16,data,sizeof(data));

//Setup
void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  dht.begin();
  pinMode(powerPin,OUTPUT);
}


void loop() {
  // Temperature & humidity -sensor is activated
  // Sensor is powered only during the measuring
  digitalWrite(powerPin,HIGH);
  delay(1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  digitalWrite(powerPin,LOW);
  delay(1000);
  
  //Convert digits into one uint8_t array
  // 36.80 %, 26.50 *C --> 3680, 2650 (int) 
  int temp1 = ((double)t)*100;
  int temp2 = ((double)h)*100;
  for(int i = 3; i >=0; i--){
    data[i+4]= ((temp1 / (int)pow(10,i)) % 10);
    data[i]= ((temp2 /(int)pow(10,i)) % 10);
  }

  //Print address and transmitted data and send them
  //(Serial.print is just for testing, prints locally on temperature sensor)
  Serial.print("Sending data packet to: ");
  Serial.println(addr16);
  for(int i = 7; i>=0 ; i--){
    Serial.print(data[i]); // printf("d",data[i]);
  }
  Serial.print("\n");
  xbee.send(tx);  // Send dataframe
  delay(1000);
}

