#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

const char* ssid     = "wtfudp";
const char* password = "asdfrewq1234";

const char* host = "192.168.4.1";
//
int AxelX = 0;
int AxelXx = 0;
int AxelY = 0;
int AxelZ = 0;
int Temper =0;
int GeroX = 0;
int GeroY = 0;
int GeroZ = 0;
char AxelXxC[17];
char* AxelXxS = "dfghjk";
unsigned int localPort = 4120;      // local port to listen on
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged\r\n";       // a string to send back
WiFiUDP Udp;

void setup() { 
  Serial.begin(115200);
  Serial.println("Подключаемся к WiFi... ");
  Serial.println(ssid);

  // подключаемся к локальной wi-fi сети
  WiFi.begin(ssid, password);

  // проверить, подключился ли wi-fi модуль к wi-fi сети
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Подключен");
  Serial.print("IP: ");  Serial.println(WiFi.localIP());

 /////////////////////////////////////////////
 Serial.println("");
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(50);
    }
  }
   mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
/////////////////////////////////////////////
  Udp.begin(localPort);
//Udp.remoteIP() = 192.168.4.1;
//Udp.remotePort() = 4120;
/////////////////////////
analogWrite(D8, 1023);
}

void loop() {
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);
AxelX = (a.acceleration.x)*100;
AxelY = (a.acceleration.y)*100;
AxelZ = (a.acceleration.z)*100;
 // Serial.println(" m/s^2");
GeroX = (g.gyro.x)*100;
GeroY = (g.gyro.y)*100;
GeroZ = (g.gyro.z)*100;
//  Serial.println(" rad/s");
Temper = (temp.temperature)*100;
//  Serial.println(" degC");
if(AxelX > 1020){AxelX = 1024;}
if(AxelX < -1020){AxelX = -1024;}
if(AxelX < 0){AxelXx = AxelX * (-1);}
else{AxelXx = AxelX;}

////////////////////////////////////////


String str = String(AxelX)+":"+String(AxelY)+":"+String(AxelZ);
str.toCharArray(AxelXxC,17);
//AxelXxS = char(AxelXx);
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket("192.168.4.1", localPort);
    Udp.write(AxelXxC);
    Udp.endPacket();
    delay(10);
analogWrite(LED_BUILTIN, AxelXx);
}
         
