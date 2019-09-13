#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <RemoteControl.h>                    // https://github.com/idotuchman/RemoteControl

#define IP_ADDRESS 10,10,10,1
#define HTTP_PORT 80
#define WEB_SOCKET_PORT 81
#define HOST_NAME "bert"      // http://bert.local/ or http://10.10.10.1/ 
#define SSID_NAME_BASE "bert-"

RemoteControl control;                        // create controller for remotely controlling pins, variables and function calls

const char WIFI_NETWORK[] = "XXXXX";
const char WIFI_PASSWORD[] = "XXXXX";



void startWebServer() {
  server.on("/upload", HTTP_GET, []() {                 // if the client requests the upload page
    if (!handleFileRead("/upload.html"))                // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.on("/upload", HTTP_POST,                       // if the client posts to the upload page
    [](){ server.send(200); },                          // Send status 200 (OK) to tell the client we are ready to receive
    handleFileUpload                                    // Receive and save the file
  );

  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void setupDNS() {
  // if DNSServer is started with "*" for domain name, 
  // it will reply with the provided IP to all DNS requests
  // https://github.com/idolpx/mobile-rr
  dnsServer.onQuery ( [] ( const IPAddress & remoteIP, const char *domain, const IPAddress & resolvedIP ) {
      if ( strstr(domain, "connectivitycheck.gstatic.com") )
        dnsServer.overrideIP =  IPAddress(74, 125, 21, 113);
      
      // connectivitycheck.android.com -> 74.125.21.113
      if ( strstr(domain, "connectivitycheck.android.com") )
        dnsServer.overrideIP =  IPAddress(74, 125, 21, 113);

      // dns.msftncsi.com -> 131.107.255.255
      if ( strstr(domain, "msftncsi.com") )
        dnsServer.overrideIP =  IPAddress(131, 107, 255, 255);

      // connectivitycheck.android.com -> 74.125.21.113
      if ( strstr( "clients1.google.com|clients2.google.com|clients3.google.com|clients4.google.com|connectivitycheck.android.com|connectivitycheck.gstatic.com", domain ) )
          dnsServer.overrideIP =  IPAddress(74, 125, 21, 113);
  });

  dnsServer.setTTL(0);
  dnsServer.start(53, "*", apIpAddress); 
}

void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incoming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

String macToString(int val) {
  String name="";
  char alphaNum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  while (val > 0) {
    name = name + alphaNum[val%36];
    val = val/36;
  }
  return name;
}

void setupWifi() {
  byte macAddr[6];
  int mac;
  String ssid, password;
  long connectTimeout;  

  // create host name in station mode or ssid name in access point mode
  WiFi.softAPmacAddress(macAddr);
  mac = (macAddr[3] && 0x0F) << 16 | macAddr[4] << 8 | macAddr[5];
  ssid = SSID_NAME_BASE + macToString(mac);
  WiFi.hostname(ssid.c_str());  // set client host name

  if (stationMode) {            // first try to connect to an wifi network
    WiFi.mode(WIFI_STA);  
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
    Serial.printf("\nAttempting to connect to wifi:\n");  
    connectTimeout = millis() + 10000;        // set timer for 10 seconds
    while (WiFi.status() != WL_CONNECTED) {   // wait for WiFi connection
      if (millis() > connectTimeout) {        // or timeout
        Serial.printf("\nAttempt timed out.\n");
        stationMode = false;
        break;
      }
      delay(250);
      Serial.print('.');
    }
  }

  if (stationMode) {    // check if station mode successful
    Serial.printf("\nConnection established!\n"); 
    Serial.printf("Connected to: ");
    Serial.println(WiFi.SSID());
    Serial.printf("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {              // if not successful, start an access point
    Serial.printf("Starting Access Point\n");
    Serial.printf("SSID: %s\n", ssid.c_str());
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIpAddress, apIpAddress, IPAddress(255, 255, 255, 0));  
    WiFi.softAP(ssid.c_str());
    Serial.printf("\nIP address: ");
    Serial.println(WiFi.softAPIP());
  }
}



String getContentType(String filename) { // convert the file extension to the MIME type
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".gz")) return "application/x-gzip";
    else if (filename.endsWith(".mp3")) return "audio/mpeg";
    else if (filename.endsWith(".wav")) return "audio/wave";
    else if (filename.endsWith(".ogg")) return "audio/ogg";
    else if (filename.endsWith(".png")) return "image/png";
    else if (filename.endsWith(".cur")) return "image/x-icon";
    return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
    String contentType = getContentType(path);             // Get the MIME type
    String pathWithGz = path + ".gz";
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
        if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
        path += ".gz";                                         // Use the compressed verion
        File file = SPIFFS.open(path, "r");                    // Open the file
        size_t sent = server.streamFile(file, contentType);    // Send it to the client
        file.close();                                          // Close the file again
        Serial.println(String("\tSent file: ") + path);
        return true;
    }
    Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
    return false;
}

void handleFileUpload(){ // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                    // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location","/success.html");      // Redirect the client to the success page
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}


// use let connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) { // When a WebSocket message is received
  String response;

  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      Serial.printf("[%u] get Text: %s\n", num, payload);      
      response = control.handle((char *) payload);
      if (response != "") {
        Serial.printf("Response: %s\n", response.c_str());
        webSocket.sendTXT(num, response.c_str());
      }
      break;
  }
}



void setup() {
    Serial.begin(115200);                       // start serial communication

    Serial.println("Starting wifi . . .");
    setupWifi();
    Serial.println();
    startWebSocket();             // Start a WebSocket server
    startWebServer();             // Start a HTTP server with a file read handler and an upload handler


  Serial.println('\n');
  SPIFFS.begin();                             // start SPI file system

  control.pin("LED", LED_BUILTIN, OUTPUT);    // remotely control on-board LED

  bertMotionStart();                          // initialize motors and PID control loops
  bertCommunicationStart();                   // initialize network communications
}

void loop() { 
  bertCommunicationLoop();                    // act on any network messages
  bertMotionLoop();                           // perform any PID calculations and movements
  delay(0);                                   // yield for built-in wifi functions
}