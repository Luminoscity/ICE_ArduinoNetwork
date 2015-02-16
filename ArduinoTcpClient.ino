#include <stdio.h>
#include <stdint.h>
#include <SPI.h>
#include <Ethernet.h>

#define SERVER_PORT 63314

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x47, 0x26 };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,0,114);
//IPAddress server(169,254,176,70);

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,2,4);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

char type,
     checksum;
char temperature[2],
     humidity[2];

int checksumCheck(uint16_t temp, uint16_t humid, char checksum) {
   char sum = 0;
   int i;
   
   for (i = 0; i < 2; ++i)
      sum += (temp >> i * 8) & 0xFF;
   for (i = 0; i < 2; ++i)
      sum += (humid >> i * 8) & 0xFF;
   
   return checksum == sum;
}

void ProcessSerial1(char where) {
   Serial1.readBytes(&type, 1);
   if (type == 'T') {
      Serial1.readBytes(temperature, 2);
   }
   else if (type == 'H') {
      Serial1.readBytes(humidity, 2);
   }
   else if (type == 'C') {
      Serial1.readBytes(&checksum, 1);
      if (checksumCheck(*(uint16_t *)temperature, *(uint16_t *)humidity, checksum)) {
         if (where == 's') {
            client.print("Temperature1: ");
            client.print((*(uint16_t *)temperature) / 10.0, 1);
            client.print(" °C\tHumidity1: ");
            client.print((*(uint16_t *)humidity) / 10.0, 1);
            client.print("%\n\r");
         }
         else {
            Serial.print("Temperature1: ");
            Serial.print((*(uint16_t *)temperature) / 10.0, 1);
            Serial.print(" °C\tHumidity1: ");
            Serial.print((*(uint16_t *)humidity) / 10.0, 1);
            Serial.print("%\n\r");
         }
      }
      else {
         Serial.println("BAD");
      }
   }
}

void ProcessSerial2(char where) {
   Serial2.readBytes(&type, 1);
   if (type == 'T') {
      Serial2.readBytes(temperature, 2);
   }
   else if (type == 'H') {
      Serial2.readBytes(humidity, 2);
   }
   else if (type == 'C') {
      Serial2.readBytes(&checksum, 1);
      if (checksumCheck(*(uint16_t *)temperature, *(uint16_t *)humidity, checksum)) {
         if (where == 's') {
            client.print("Temperature2: ");
            client.print((*(uint16_t *)temperature) / 10.0, 1);
            client.print(" °C\tHumidity2: ");
            client.print((*(uint16_t *)humidity) / 10.0, 1);
            client.print("%\n\r");
         }
         else {
            Serial.print("Temperature2: ");
            Serial.print((*(uint16_t *)temperature) / 10.0, 1);
            Serial.print(" °C\tHumidity2: ");
            Serial.print((*(uint16_t *)humidity) / 10.0, 1);
            Serial.print("%\n\r");
         }
      }
      else {
         Serial.println("BAD");
      }
   }
}

void ProcessSerial3(char where) {
   Serial3.readBytes(&type, 1);
   if (type == 'T') {
      Serial3.readBytes(temperature, 2);
   }
   else if (type == 'H') {
      Serial3.readBytes(humidity, 2);
   }
   else if (type == 'C') {
      Serial3.readBytes(&checksum, 1);
      if (checksumCheck(*(uint16_t *)temperature, *(uint16_t *)humidity, checksum)) {
         if (where == 's') {
            client.print("Temperature3: ");
            client.print((*(uint16_t *)temperature) / 10.0, 1);
            client.print(" °C\tHumidity3: ");
            client.print((*(uint16_t *)humidity) / 10.0, 1);
            client.print("%\n\r");
         }
         else {
            Serial.print("Temperature3: ");
            Serial.print((*(uint16_t *)temperature) / 10.0, 1);
            Serial.print(" °C\tHumidity3: ");
            Serial.print((*(uint16_t *)humidity) / 10.0, 1);
            Serial.print("%\n\r");
         }
      }
      else {
         Serial.println("BAD");
      }
   }
}

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

  // start the Ethernet connection:
  if (!Ethernet.begin(mac)) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, SERVER_PORT)) {
    Serial.println("connected");
//    client.print("Temperature: 20.9 °C\tHumidity: 60%\0");
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
   if (Serial1.available()) {
      ProcessSerial1('s');
   }
   if (Serial2.available()) {
      ProcessSerial2('s');
   }
   if (Serial3.available()) {
      ProcessSerial3('s');
   }
   // if there are incoming bytes available 
   // from the server, read them and print them:
   if (client.available()) {
     char c = client.read();
     Serial.print(c);
   }

   // if the server's disconnected, stop the client:
   if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();

      // do nothing forevermore:
      while(true) {
         if (Serial1.available()) {
            ProcessSerial1('t');
         }
         if (Serial2.available()) {
            ProcessSerial2('t');
         }
         if (Serial3.available()) {
            ProcessSerial3('t');
         }
      }
   }
}
