/*Connect to TAURUS IOT PLATFORM using Arduino Mega + ESP8266
Code by : Leonardo Gonçalves - Inatel - Brazil
*/
#define SSID "SSID" //Insert your Wireless SSID Here
#define PASS "PASSWWORD" //Insert your Wireless Password Here
#define IP "177.153.20.101" // taurussystem.com
String GET = "GET /ws/webresources/SetSensor?key=SERIAL&&load=(2001,13)"; //Insert your Taurus Serial, will be put on sensor id2001 the value 13
void setup()
{
  Serial1.begin(115200);
  Serial.begin(9600);
  sendDebug("AT");
  delay(5000);
  if (Serial1.find("OK")) {
    Serial.println("RECEIVED: OK");
    //connectWiFi();
  }
}
void loop() {
  //String tenmpF = String(76, DEC);
  updateTaurus();
  delay(4000);
}
void updateTaurus() {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if (Serial1.find("Error")) {
    Serial.print("RECEIVED: Still Conected");
    return;
  }
  cmd = GET;
  cmd += "\r\n";
  Serial1.print("AT+CIPSEND=");
  Serial.print("AT+CIPSEND=");
  Serial1.println(cmd.length());
  Serial.println(cmd.length());
  if (Serial1.find(">")) {
    Serial.print(">");
    Serial.print(cmd);
    Serial1.print(cmd);
  } else {
    sendDebug("AT+CIPCLOSE");
  }
  if (Serial1.find("OK")) {
    Serial.println("RECEIVED: OK");
  } else {
    Serial.println("RECEIVED: Error");
  }
}
void sendDebug(String cmd) {
  Serial.print("SEND: ");
  Serial.println(cmd);
  Serial1.println(cmd);
}
boolean connectWiFi() {
  Serial1.println("AT+CIPMUX=0");
  Serial1.println("AT+CWMODE=1");
  delay(2000);
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  sendDebug(cmd);
  delay(5000);
  if (Serial1.find("OK")) {
    Serial.println("RECEIVED: OK");
    return true;
  } else {
    Serial.println("RECEIVED: Error");
    return false;
  }
}
