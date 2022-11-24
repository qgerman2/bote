#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

const char* ssid = "autoh2";
const int maxclients = 4;
ESP8266WebServer HTTPserver(80);
using namespace websockets;
WebsocketsServer WSserver;
WebsocketsClient clients[maxclients];
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int servocenter = 305;
const int servodelta = 35;
int servotarget = 305;
int servocurrent = 305;

void onMessage(WebsocketsClient& client, WebsocketsMessage message) {
  WSInterfaceString msg = message.data();
  if (msg == "upp") {
    pwm.setPWM(9, 4096, 0);
  } else if (msg == "upr") {
    pwm.setPWM(9, 0, 4096);
  } else if (msg == "leftp") {
    servotarget = servocenter - servodelta;
  } else if (msg == "leftr") {
    servotarget = servocenter;
  } else if (msg == "rightp") {
    servotarget = servocenter + servodelta;
  } else if (msg == "rightr") {
    servotarget = servocenter;
  }
}

void setup() {
  LittleFS.begin();
  WiFi.softAP(ssid);
 
  WSserver.listen(8080);

  HTTPserver.serveStatic("/", LittleFS, "/");
  HTTPserver.begin();

  Wire.begin(2,0);
  pwm.begin();
  pwm.setPWMFreq(50);

  pwm.setPWM(9, 0, 4096);
  pwm.setPWM(8, 0, 305);
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
  // Giro de servo suave
  if (servocurrent != servotarget) {
    if (servocurrent > servotarget) {
      servocurrent--;
    } else if (servocurrent < servotarget) {
      servocurrent++;
    }
  }
  pwm.setPWM(8, 0, servocurrent);
  delay(10);
}