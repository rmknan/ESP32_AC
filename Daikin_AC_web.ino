#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Daikin.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "SSID";
const char* password = "pass";

WebServer server(80);
const uint16_t irledpin = 32;
IRDaikinESP ac(irledpin);

void sendAcon()
{
  ac.begin();
  ac.on();
  ac.setMode(kDaikinCool);
  ac.setTemp(24);
  ac.setFan(kDaikinFanMax);
  ac.send();
  server.send(200,"text/plain","AC turned ON");
}

void sendAcoff()
{
  ac.begin();
  ac.off();
  ac.send();
  server.send(200,"text/plain","AC turned OFF");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("📶 Connecting to Wi-Fi...");

  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries > 20) {
      Serial.println("\n❌ Failed to connect to Wi-Fi. Check SSID/password or range.");
      return;
    }
  }

  Serial.println("\n✅ WiFi connected!");
  Serial.print("📡 IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/ac_on", sendAcon);
  server.on("/ac_off", sendAcoff);
  server.begin();
  Serial.println("🌐 Web server started");
}

void loop() {
  server.handleClient();
}

