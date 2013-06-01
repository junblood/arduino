/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <math.h>

#define APIKEY "547d0d14f0d8bcbc0f5d6167ea5e0275" //更换 yeelink api key
#define DEVICEID 3137 // 更换设备IDreplace your device ID
#define SENSORID 4424 // 更换传感器IDreplace your sensor ID

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x7C,0xD1,0xC3,0xE7,0x39,0xE3 };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "api.yeelink.net"; 

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,103);
// initialize the library instance:
EthernetClient client;
unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 30*1000; // delay between 2 datapoints, 30s

void setup() {
  Wire.begin();
  // start serial port:
  Serial.begin(57600);
  // start the Ethernet connection with DHCP:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
      ;
  }
  else {
    Serial.println("Ethernet configuration OK");
  }
}

void loop() {
  
  if(millis()>135000)
  {
     software_Reset();
  }
  
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:

  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    // read sensor data, replace with your code
    float sensorReading = readTemperatureSensor();
    //send data to server
    sendData(sensorReading);
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");  
} 

// this method makes a HTTP connection to the server:
void sendData(float thisData) {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("POST /v1.0/device/");
    client.print(DEVICEID);
    client.print("/sensor/");
    client.print(SENSORID);
    client.print("/datapoints");
    client.println(" HTTP/1.1");
    client.println("Host: api.yeelink.net");
    client.print("Accept: *");
    client.print("/");
    client.println("*");
    client.print("U-ApiKey: ");
    client.println(APIKEY);
    client.print("Content-Length: ");

    // calculate the length of the sensor reading in bytes:
    // 10 bytes for {"value":} + number of digits of the data:
    int thisLength = 13 + getLength(thisData);
    client.println(thisLength);

    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println();

    // here's the actual content of the PUT request:
    client.print("{\"value\":");
    client.print(thisData);
    client.println("}");
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
   // note the time that the connection was made or attempted:
  lastConnectionTime = millis();
}

// This method calculates the number of digits in the
// sensor reading.  Since each digit of the ASCII decimal
// representation is a byte, the number of digits equals
// the number of bytes:
int getLength(int someValue) {
  // there's at least one byte:
  int digits = 1;
  // continually divide the value by ten,
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = someValue /10;
  while (dividend > 0) {
    dividend = dividend /10;
    digits++;
  }
  // return the number of digits:
  return digits;
}

///////////////////////////////////////////////////////////////////////////
// get data from Temperature sensor
// you can replace this code for your sensor
float readTemperatureSensor()
{
  float val;//定义变量
  float dat;//定义变量
  val=analogRead(0);// 读取传感器的模拟值并赋值给val
  //dat=(125*val)>>8;//温度计算公式
   dat = (val * 500) /1024;       // Convert Kelvin to Celcius
  
  Serial.print("Sensor value is: ");
  Serial.println((float)dat);

  return dat;
}

