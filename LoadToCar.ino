#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef APSSID
#define APSSID "wtfudp"

#define APPSK  "asdfrewq1234"
#endif
const char* hostAP = "esp8266-webupdate";
const char *ssidap = APSSID;
const char *passwordap = APPSK;
int ping = 0;
unsigned int localPort = 4120;      // local port to listen on
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged\r\n";       // a string to send back
String packetBufferS = "";
String cont = "";

WiFiUDP Udp;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<meta charset=\"UTF-8\"/><h1>Акселерометр</h1><p>:1234:3456:5678:6789:86243:834245:</p><a href='/index.php'>index.php</a>");

}
int AxelX = 0;
int AxelXx = 0;
int AxelY = 0;
int AxelZ = 0;
int Temper =0;
int GeroX = 0;
int GeroY = 0;
int GeroZ = 0;
String output[17];
//int Axel[15];
//int AxelX = "";
//String AxelY = "";
//String AxelZ = "";
//String Temper = "";
//String GeroX = "";
//String GeroY = "";
//String GeroZ = "";



void setup() { 
  Serial.begin(115200);
 

WiFi.softAP(ssidap, passwordap);
IPAddress myIP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(myIP);
server.on("/", handleRoot);
server.begin();
//  Serial.println("HTTP server started");
MDNS.begin(hostAP);
//httpUpdater.setup(&server, update_path, update_username, update_password);
MDNS.addService("http", "tcp", 80);
//  Serial.println(WiFi.localIP());
//  Serial.printf("HTTPUpdateServer ready! login with username '%s' and password '%s'\n", update_username, update_password);
analogWrite(LED_BUILTIN, 1024);


/////////////////////////////////////////////
  Udp.begin(localPort);

/////////////////////////
//pinMode(LED_BUILTIN, OUTPUT);
pinMode(D1, OUTPUT);
pinMode(D2, OUTPUT);
pinMode(D3, OUTPUT);
pinMode(D4, OUTPUT);
//analogWrite(LED_BUILTIN, 512);
analogWrite(D8, 1024);

}

void loop() {
server.handleClient();
MDNS.update();
int packetSize = Udp.parsePacket();
        if (packetSize) {
          int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
          packetBuffer[n] = 0;
          packetBufferS = String(packetBuffer);
          int maxIndex = 16;
          int y = 0;
          for(int i=0; i<=maxIndex ;i++){output[i]="";}
          output[17] = "";
          for(int i=0; i <= maxIndex; i++){
              cont = packetBufferS.charAt(i);
              if(cont != ":"){
                //Serial.print(packetBufferS.charAt(i));
                output[y]= output[y] + packetBufferS.charAt(i);
              }
              else{
                y++;
              }
  
           }
           for(int i=0; i<2; i++){
              Serial.println(output[i]);
           }
           Serial.println();
         }

AxelX = output[0].toInt();
  if(AxelX > 1023) AxelX = 1023;
  if(AxelX < -1023)AxelX = -1023;
AxelY = output[1].toInt();
AxelZ = output[2].toInt();
AxelX = (AxelX)/2;
if(AxelX > (-50) && AxelX < 50){AxelX = 0;digitalWrite(D3,LOW);digitalWrite(D4,LOW);}
       
       if(AxelX <= -50){
        digitalWrite(D4,LOW);
        digitalWrite(D3,HIGH);
        AxelX = AxelX + 49;
      //  Serial.println("НАЗАД");
        }
        
       if(AxelX >= 50){
        digitalWrite(D3,LOW);
        digitalWrite(D4,HIGH);
        AxelX = AxelX - 49;
       // Serial.println("ВПЕРЕД");
          }
       
       
        if(AxelX < 0){AxelXx = AxelX*(-1);}
        else{AxelXx = AxelX;}

        if(AxelY <= 300 && AxelY >= -300){digitalWrite(D1,LOW);digitalWrite(D2,LOW);}
        
        if(AxelY>300){
          digitalWrite(D2,LOW);
          delay(50);
          digitalWrite(D1,HIGH);
          }
     
        if(AxelY<-300){
          digitalWrite(D1,LOW);
          delay(50);
          digitalWrite(D2,HIGH);
          }
analogWrite(D8, AxelXx);

         
}
         
