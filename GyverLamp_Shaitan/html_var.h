#ifdef HTML_REMOTE
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

WiFiServer wifiServerLamp(80);
WiFiClient client;

#define HTML_pages (5)
int BRI=150;
int SPD=100; 
int SCA=50; 
int VOL=20;
int HTML_page=0;
String req;
uint32_t HTMLTimer;
#endif
