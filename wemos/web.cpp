#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "options.h"
#include "web_page.h"
#include "ir.h"

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

String logBuffer;

void sendText(String text) {
  logBuffer += text + "\n";
  server.send(200, "text/plain", text);

  if (logBuffer.length() > 2000) {
    logBuffer.remove(0, 500);
  }
}

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleStatus() {
  String json = "{";
  json += "\"IRReceiveEnabled\":" + String(IRReceiveEnabled ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleLog() {
  server.send(200, "text/plain", logBuffer);
}

void handleSendIR() {
  String protocol = server.arg("protocol");
  uint16_t address = strtoul(server.arg("address").c_str(), NULL, 16);
  uint16_t commandIR = strtoul(server.arg("command").c_str(), NULL, 16);
  int_fast8_t repeats = server.arg("repeats").toInt();
  sendIR(protocol, address, commandIR, repeats);
}

void wifiSetup() {
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handleRoot);
  server.on("/toggleIR", HTTP_GET, handleToggleIR);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/sendIR", HTTP_POST, handleSendIR);
  server.on("/log", handleLog);

  server.begin();
  Serial.println("HTTP server started");
}

void wifiLoop() {
  server.handleClient();
  yield(); 
}