// CODE by Leonardo Gonçalves - Das Coisas- Santa Rita do Sapucaí - MG - Brazil
#define SSID "SSID" //Insert your Wireless SSID Here
#define PASS "PASSWORD" //Insert your Wireless Password Here
#define IP "104.131.189.228" // Taurus Address
String GET = "GET /ws/webresources/Sensor/set?key=Teste1&&sensor=SENSORID&&value=VALUE HTTP/1.0\r\nHost: www.taurussystem.com\r\nConnection: close\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n";
void setup()
{
Serial1.begin(115200);
Serial.begin(115200);
sendDebug("AT");
delay(5000);
if (Serial1.find("OK")) {
Serial.println("RECEIVED: OK");
connectWiFi();
}
}
void loop() {
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
