#include "web.h"
#include <ESPAsyncWebServer.h>


extern AsyncWebServer web;


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


}
