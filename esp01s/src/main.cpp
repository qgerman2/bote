#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <Wire.h>

const char* ssid = "BOTEEEEE";
const int maxclients = 4;
ESP8266WebServer HTTPserver(80);
using namespace websockets;
WebsocketsServer WSserver;
WebsocketsClient clients[maxclients];

const int servocenter = 305;
const int servodelta = 35;
int servotarget = 305;
int servocurrent = 305;

void onMessage(WebsocketsClient& client, WebsocketsMessage message) {
  WSInterfaceString msg = message.data();
  Wire.beginTransmission(6);
  Wire.write(msg[0]);
  Wire.write(msg[1]);
  Wire.write(msg[2]);
  Wire.endTransmission();
}

void setup() {
  LittleFS.begin();
  WiFi.softAP(ssid);
 
  WSserver.listen(8080);

  HTTPserver.serveStatic("/", LittleFS, "/");
  HTTPserver.begin();

  Wire.begin(2, 0);
}

void loop() {
  HTTPserver.handleClient();
  if (WSserver.available()) {
    // Aceptar nuevos clientes
    if (WSserver.poll()) {
      int slotAvailable = -1;
      // Revisar que quede un espacio disponible
      for (int i = 0; i < maxclients; i++) {
        if (!clients[i].available()) {
          slotAvailable = i;
          break;
        }
      }
      // Registrar cliente nuevo
      if (slotAvailable > -1) {
        WebsocketsClient client = WSserver.accept();
        client.onMessage(onMessage);
        clients[slotAvailable] = client;
      }
    }
    // Consultar actualizaciones de clientes
    for (int i = 0; i < maxclients; i++) {
      if (!clients[i].available()) {continue;}
      clients[i].poll();
    }
  }
  delay(10);
}