void handleRoot() {
  if (!server.authenticate(www_username, www_password)) {
    return server.requestAuthentication();
  }
  server.send(200, "text/plain", "Hello from esp8266 over HTTPS!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void webServerSetup() {

//  // Read certificates from SPIFF and pass them to the server...
//  char * srvCert = getCLOB("/cert.txt");
//  char * srvKey = getCLOB("/priv_key.txt");
//  if( !srvCert || !srvKey ) {
//    Serial.println((long)srvCert);    Serial.println((long)srvKey);    Serial.println("Eh....");    while(1) yield();
//  }
//


  server.setRSACert(new BearSSLX509List(serverCert), new BearSSLPrivateKey(serverKey));
//  free(srvCert);
//  free(srvKey);

  server.on("/", handleRoot);

  server.on("/favicon.ico", []() {
    File favIconFile = SPIFFS.open("/favicon.ico", "r");
    if ( !favIconFile ) {
      Serial.println(F("Error: /favicon.ico not found."));
      server.send(404, "text/plain", "Not found");
      return;
    }
    server.streamFile(favIconFile, "image/vnd.microsoft.icon");
    favIconFile.close();
  });

//  server.on("/time", []() {
//    String beforeTime = timeClient.getFormattedTime();
//    timeClient.update();
//    String timeResponse = "Time was " + beforeTime + " and is now " + timeClient.getFormattedTime();
//    server.send(200, "text/plain", timeResponse);
//  });

  server.on("/drop", []() {
    String response = "Raindrop sensor gives ";
    response += String(analogRead(A0));
    response += " V.";
    server.send(200, "text/plain", response);
  });

  server.onNotFound(handleNotFound);

  server.begin();
}
