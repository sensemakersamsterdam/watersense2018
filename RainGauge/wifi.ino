void wifiSetup(void) {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  delay(1000);

  Serial.print("Waiting to establish connection to: ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //    Serial.println("Connection Failed! Rebooting...");
  //    delay(5000);
  //    ESP.restart();
  //  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}
