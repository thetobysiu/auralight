#include "WiFi.h"
#include "ESPmDNS.h"
#include "AsyncUDP.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "FastLED.h"

#define NUM_LEDS 174
#define DATA_PIN 15
#define UDP_PORT 3000

uint8_t colors[3] = {0, 0, 0};
uint8_t brightness = 60;
uint16_t channel = 0;
bool mood = false;
uint32_t breathColor = 0xFFFFFF;
uint8_t breathDelay = 40;
uint8_t startBrightness = 20;
uint8_t endBrightness = 60;

AsyncWebServer server(80);
CRGB leds[NUM_LEDS];
AsyncUDP udp;

void changeColor(uint32_t colorValue) {
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = colorValue;
  }
}

void setup() {
  Serial.begin(115200);
  
  const char * ssid = "";
  const char * password = "";
  
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Init FastLED lights
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalPixelString);
  FastLED.setMaxPowerInMilliWatts(7900);
  FastLED.setBrightness(brightness);
  FastLED.show();

  // Connect to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(1000);
  digitalWrite(2, HIGH);
  delay(1000);

  if (!MDNS.begin("auralight")) {
    Serial.println("Error setting up mDNS responder");
  } else {
    Serial.println("mDNS responder started: auralight.local");
  }

  // The web server for handling post request and provide web interface
  server.on("/", HTTP_GET, handleRoot);
  server.on("/aura.js", HTTP_GET, handleAura);
  server.on("/styles.css", HTTP_GET, handleCSS);
  server.on("/", HTTP_POST, handlePost);
  server.onNotFound(handleNotFound);
  server.begin();
  
  MDNS.addService("http", "tcp", 80);

  // The udp server handling the data from computer
  if (udp.listen(UDP_PORT)) {
    udp.onPacket([](AsyncUDPPacket packet) {
      if (!mood) {
        channel = 0;
        while (packet.available()) {
          colors[channel % 3] = packet.read();
          if (channel % 3 == 2) {
            leds[channel / 3] = CRGB(colors[0], colors[1], colors[2]);
          }
          channel++;
        }
        FastLED.setBrightness(brightness);
        FastLED.show();
      }
    });
  }
}

void loop() {
  // The breathing lights, if mood mode is enabled(default off = screen extension mode)
  if (mood) {
    for(int i=startBrightness; i<endBrightness; i++){
      FastLED.setBrightness(i);
      FastLED.show();
      delay(breathDelay);
    }
    for(int i=endBrightness; i>startBrightness; i--){
      FastLED.setBrightness(i);
      FastLED.show();
      delay(breathDelay);
    }
  }
}
