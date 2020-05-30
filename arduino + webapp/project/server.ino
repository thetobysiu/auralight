String processor(const String& var)
{
  if(var == "BRIGHTNESS"){
    return String(brightness);
  }
  if(var == "COLOR"){
    char colorCode[7];
    sprintf(colorCode, "#%06lX", breathColor);
    return colorCode;
  }
  if(var == "DELAY"){
    return String(breathDelay);
  }
  if(var == "START"){
    return String(startBrightness);
  }
  if(var == "END"){
    return String(endBrightness);
  }
  if(var == "STATE"){
    if (mood) {
      return String("Mood");
    } else {
      return String("Extension");
    }
  }
  return String();
}

void handleRoot(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/index.html", "text/html", false, processor);
}

void handleAura(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/aura.js", "application/javascript");
}

void handleCSS(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/styles.css", "text/css");
}

void handlePost(AsyncWebServerRequest *request) {
  if (request->hasArg("brightness")) { // updateBrightness
    brightness = (request->arg("brightness")).toInt();
    request->send(200);
  }
  else if (request->hasArg("temp")) { // updateTemp
    FastLED.setCorrection((request->arg("temp")).toInt());
    request->send(200);
  }
  else if ((request->hasArg("breathColor")) && (request->hasArg("breathDelay")) && (request->hasArg("startBrightness")) && (request->hasArg("endBrightness"))) { // updateMood
    breathColor = (request->arg("breathColor")).toInt();
    changeColor(breathColor);
    breathDelay = (request->arg("breathDelay")).toInt();
    startBrightness = (request->arg("startBrightness")).toInt();
    endBrightness = (request->arg("endBrightness")).toInt();
    request->send(200);
  }
  else if (request->hasArg("mood")) { // updateUDP
    mood = !mood;
    if (mood) {
      changeColor(breathColor);
    } else {
      changeColor(0);
    }
    request->send(200);
  }
  else {
    request->send(500);
  }
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404);
}
