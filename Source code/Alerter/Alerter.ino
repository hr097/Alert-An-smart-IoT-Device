#include<ESP8266WiFi.h> // we need to include library file for inbuilt functions access

const char* ssid = "Winter & Stark Automation";  // station SSID visible on devices as wifi connections
const char* password = "Jonsnow#1008";  // password for connecting to station 

WiFiServer server(80);// port 80 is default for wifi in devices for we need to communicate to server using port 80

const int gpio_pir_sensor = 16 ;  // Digital pin 8
char Motion_Status;

void setup() {
  
  WiFi.softAP(ssid, password); // start interacting & create wifi signal for Devices which need to be connected 
  server.begin(); // server stated with station(192.168.4.1) in Your case it might be different
  pinMode(gpio_pir_sensor, INPUT);
  Serial.begin(115200);
  Motion_Status = 'N';
}

void loop() {
  
  WiFiClient client = server.available(); // if server is available on device then module will create 1 client of that server
  if (!client) // if client not found then exit this loop and search again for client
  {
    return;
  }

  while(!client.available()) // if available then wait for 1 microSeconds
  {
    delay(1);
  }
  
  if (digitalRead(gpio_pir_sensor)==HIGH) {
    Motion_Status = 'Y';
    delay(1000);
  }
  else {
    Serial.println("No motion detected.");
    Motion_Status = 'N';
    delay(1000);
  }
  
  String HTML = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><title>ALERT !!!</title><center><b>Winter & Stark Automation</b></center>";
  if(Motion_Status=='Y')
  HTML += "<meta http-equiv='refresh' content='3'></meta>";
  else
  HTML += "<meta http-equiv='refresh' content='1'></meta>";
  
  HTML += "</head><body><center><br><h1></h1>MOTION STATUS : <hr><br><h1>";
  
  if(Motion_Status=='Y')
  HTML += "MOTION DETECTED !!";
  else
  HTML += "NO MOTION DETECTED";
  
  HTML += "</h1></center></body></html>";
  client.print(HTML); // make webpage means it will print all HTML code to 192.168.4.1 station
  delay(10);
  client.flush(); 
}
