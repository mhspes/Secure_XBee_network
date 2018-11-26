#include <XBee.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();

uint8_t sensor_data[8];
uint16_t addr16;
uint16_t addr_list[] = {4097,4098,4099}; // List of allowed addresses

void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop() { 
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()){
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE){
      
     xbee.getResponse().getRx16Response(rx16);
     addr16 = rx16.getRemoteAddress16();

      if(addr16 == addr_list[0] || addr16 == addr_list[1] || addr16 == addr_list[2]){
        Serial.print("Packet received from ");
        switch(addr16){
        case 4097:
          Serial.print(addr16);
          Serial.println(" - Node 1.");
          break;
        case 4098:
          Serial.print(addr16);
          Serial.println(" - Node 2.");
          break;
        case 4099:
          Serial.print(addr16);
          Serial.println(" - Node 3.");
          break;
        default:
          Serial.println("unknown address.");
          break;
         }
     
        //Fetch the temperature data
      for (int i =rx16.getDataLength()-1; i >= 0;i--){
        sensor_data[i] = rx16.getData(i);
        }
      print_data(sensor_data);
        //Print measurements 
      } else {
        Serial.println("Unknown address - ignoring packet."); 
      }
    } else {
      Serial.println("Wrong addressing format - 16bit expected.");
    }
    delay(1000);
    
    } else if (xbee.getResponse().isError()) {
      Serial.println("Error reading packet.");     
    } 
}

// Routine for printing data from
// temperature & humidity sensor
void print_sensordata(uint8_t data){

  Serial.print("Temperature: ");
  for (int i = sizeof(data)-1; i >= 0; i-- ){
    Serial.print(data[i]); 
    if (i==4){
      Serial.print(" C Humidity: ");
      }
    if (i==0){
      Serial.print(" %");
      }
    if (i==6 || i==2){
      Serial.print(".");
      }
    }
      Serial.println();
}

