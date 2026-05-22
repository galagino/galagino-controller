#include "web.h"
#include "globals.h"
#include <ESPAsyncWebServer.h>

AsyncWebServer web(80);
AsyncWebSocket ws("/ws");

const char *ok = "OK\n";

void setupWebServer() {

  web.on("/", 
         HTTP_GET, 
         [](AsyncWebServerRequest *req) { 
    AsyncWebServerResponse *rsp = req->beginResponse(200, "text/html", index_html_gz, sizeof(index_html_gz));
    rsp->addHeader("Content-Encoding", "gzip");
    req->send(rsp);
  });
  web.on("/index.html", 
         HTTP_GET, 
         [](AsyncWebServerRequest *req) { 
    AsyncWebServerResponse *rsp = req->beginResponse(200, "text/html", index_html_gz, sizeof(index_html_gz));
    rsp->addHeader("Content-Encoding", "gzip");
    req->send(rsp);
  });

  web.on("/index.js", 
         HTTP_GET, 
         [](AsyncWebServerRequest *req) { 
    AsyncWebServerResponse *rsp = req->beginResponse(200, "text/javascript", index_js_gz, sizeof(index_js_gz));
    rsp->addHeader("Content-Encoding", "gzip");
    req->send(rsp);
  });

  web.on("/index.css",
         HTTP_GET,
         [](AsyncWebServerRequest *req) {
    AsyncWebServerResponse *rsp = req->beginResponse(200, "text/css", style_css_gz, sizeof(style_css_gz));
    rsp->addHeader("Content-Encoding", "gzip");
    req->send(rsp);
  });

  web.on("/pair",
         HTTP_GET,
         [](AsyncWebServerRequest *req) {
    pairingStart=millis();
    pairingEnabled=true;

    AsyncWebServerResponse *rsp = req->beginResponse(200, "text/css", (const uint8_t *)ok, strlen(ok));
    req->send(rsp);
  });

  web.on("/dump",
         HTTP_GET,
         [](AsyncWebServerRequest *req) {
    
    serialDump=true;
    AsyncWebServerResponse *rsp = req->beginResponse(200, "text/css", (const uint8_t *)ok, strlen(ok));
    req->send(rsp);
  });
  
  web.on("/nodump",
         HTTP_GET,
         [](AsyncWebServerRequest *req) {
    
    serialDump=false;
    AsyncWebServerResponse *rsp = req->beginResponse(200, "text/css", (const uint8_t *)ok, strlen(ok));
    req->send(rsp);
  });

  web.on("/forgetkeys",
         HTTP_GET,
         [](AsyncWebServerRequest *req) {
    
    forgetKeys=true;
    AsyncWebServerResponse *rsp = req->beginResponse(200, "text/css", (const uint8_t *)ok, strlen(ok));
    req->send(rsp);
  });
}
