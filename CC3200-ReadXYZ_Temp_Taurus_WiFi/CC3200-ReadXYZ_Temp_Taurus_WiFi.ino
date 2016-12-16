
/*
Taurus IOT Plataform with CC3200 LaunchPad
 
 By: Leonardo Gonçalves - Inatel - Brazil
 */


#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>

#include <Wire.h>
#include <BMA222.h>
BMA222 mySensor;

#include <Wire.h>
#include "Adafruit_TMP006.h"
Adafruit_TMP006 tmp006(0x41);

// SSID da sua rede
char ssid[] = "SSID";
// Senha
char password[] = "PASSWORD";
int objt;
int diet;
int8_t x;
int8_t y;
int8_t z;
char server[] = "104.131.189.228";    // name address for Google (using DNS)
String GETtoTaurus;

WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  mySensor.begin();
  uint8_t chipID = mySensor.chipID();
  Serial.print("chipID: ");
  Serial.println(chipID);
  if (! tmp006.begin()) 
  {
    Serial.println("No sensor found");
  }
  // Tentativa de conexão a Wi-Fi
  Serial.print("Tentando se conectar a: ");
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nVoce esta conectado a rede");
  Serial.println("Aguardando por um endereco IP");

  while (WiFi.localIP() == INADDR_NONE) {
    // enquanto não se conecta a rede
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nEndereco IP obtido");
  printWifiStatus();
}

void loop() 
{
  GET();
  ReadData();
  TaurusUpdate();
  delay(10000);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void TaurusUpdate()
{
  Serial.println("\nConectando a Taurus......");
  // Se conectado, reporte na serial:
  if (client.connect(server, 80)) {
    Serial.println("Conectado");
    // Make a HTTP request:
    client.println(GETtoTaurus);
    client.println("Host: taurussystem.com");
    client.println("Connection: close");
    client.println();
    client.println();
  }
  while (client.available()) {
    char c = client.read(); //Resposta do servidor
    Serial.write(c);
  }
  // if the server's disconnected, stop the client:
  if (!client.connected()) 
  {
    Serial.println();
    Serial.println("Disconectando.");
    client.stop();
  }
}
void GET()
{
  String IDX = "2001"; //2001 em meu exemplo
  String IDY = "2002"; //2002 em meu exemplo
  String IDZ = "2003"; //2003 em meu exemplo
  String TempObj = "2004"; //2004 em meu exemplo  
  String TempReal = "2005"; //2005 em meu exemplo
  GETtoTaurus = "GET /ws/webresources/SetSensor?key=SERIAL&&load=("+IDX+","+x+"*"+IDY+","+y+"*"+IDZ+","+z+"*"+TempObj+","+objt+"*"+TempReal+","+diet+") HTTP/1.1"; 
  Serial.println(GETtoTaurus);
}

void ReadData()
{
  x = mySensor.readXData();
  Serial.print("X: ");
  Serial.print(x);
  y = mySensor.readYData();
  Serial.print(" Y: ");
  Serial.print(y);
  z = mySensor.readZData();
  Serial.print(" Z: ");
  Serial.println(z);
  tmp006.wake();
  objt = tmp006.readObjTempC();
  Serial.print("Object Temperature: "); 
  Serial.println(objt); 
  //Serial.println("*C");
  diet = tmp006.readDieTempC();
  Serial.print("Die Temperature: "); 
  Serial.println(diet); 
  //Serial.println("*C");
  delay(2000);
}
