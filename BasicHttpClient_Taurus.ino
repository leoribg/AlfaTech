/**
 * BasicHTTPClient.ino
 *
 *  By: Leonardo Gonçalves - Inatel - Brazil
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
String KEY = "Teste1";
String ID = "1001";
int VAR = 0;
String GET, GETCREATE;
String V;
void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();
  digitalWrite(BUILTIN_LED, HIGH);
  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFiMulti.addAP("Goncalves", "34710775");

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    URL();
    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("192.168.1.12", 443, "/test.html", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    http.begin("177.153.20.101", 80, GET); //http://taurus.jelasticlw.com.br
    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      //if(httpCode == 200) {
      String payload = http.getString();
      int posicao = payload.indexOf(ID);
      if (posicao == -1 && httpCode == 200)
      {
        CREATE();
      }
      String dado = payload.substring(posicao + 5, posicao + 6);
      USE_SERIAL.print("Dado: ");
      USE_SERIAL.println(dado);
      if (dado == "0")
      {
        digitalWrite(BUILTIN_LED, HIGH);//ANODO COMUM, ACENDE COM LOW
      }
      if (dado == "1")
      {
        digitalWrite(BUILTIN_LED, LOW);
      }
      USE_SERIAL.println(payload);
      //}
    } else {
      USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
    }
  }
  delay(1000);
}

void URL(void)
{
  GET = "http://taurus.jelasticlw.com.br/ws/webresources/GetDigitais?key=Teste1";
  String estado = ",0)";
  GETCREATE = "http://taurus.jelasticlw.com.br/ws/webresources/Create?key=Teste1&&load=(";
  GETCREATE = GETCREATE + ID + estado;
  /*
  VAR++;
  V = String(VAR);
  USE_SERIAL.println(V);
  GET=GET+V;*/
}
void CREATE(void)
{
  URL();
  HTTPClient http;
  USE_SERIAL.println("Componente não existente, criando ...");
  http.begin("177.153.20.101", 80, GETCREATE); //HTTP
  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  if (httpCode) {
    // HTTP header has been send and Server response header has been handled
    USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    //if(httpCode == 200) {
    String payload = http.getString();
    int posicao = payload.indexOf(ID);
  }
}

