

const char* host = "esp32";
const char* ssid = "hackspace-wifi";
const char* password = "funkytown1234";

int servoMin;
int servoMax;
int servoPos;

const char* controlPage =
"<html><head>"
"<meta name='viewport' content='width=device-width, initial-scale=1'>"
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>"
"<style>body{font-family:Arial;text-align:center;} .slider{width:300px;}</style>"
"</head><body>"
"<h2>mini R2D2 Control Panel</h2>"
"<button onclick=\"location.href='/ota'\">OTA Update</button>"
"<p>Min: <span id='minVal'>500</span></p>"
"<input type='range' min='500' max='2500' value='500' class='slider' id='minSlider' oninput='minVal.innerHTML=this.value'>"

"<p>Max: <span id='maxVal'>2500</span></p>"
"<input type='range' min='500' max='2500' value='2500' class='slider' id='maxSlider' oninput='maxVal.innerHTML=this.value'>"

"<p>Position: <span id='posVal'>0</span></p>"
"<input type='range' min='0' max='180' value='90' class='slider' id='posSlider' oninput='posVal.innerHTML=this.value'>"

"<br><br>"
"<button onclick='applyLimits()'>Apply limits</button>"
"<button onclick='moveServo()'>Move servo</button>"

"<script>"
"function applyLimits(){"
"  let mn = document.getElementById('minSlider').value;"
"  let mx = document.getElementById('maxSlider').value;"
"  $.get('/setlimits?min=' + mn + '&max=' + mx);"
"}"
"function moveServo(){"
"  let p = document.getElementById('posSlider').value;"
"  $.get('/setpos?pos=' + p);"
"}"
"</script>"
"</body></html>";


/*
 * setup function
 */
void setupOTA(void) {
  //Serial.begin(115200); baud rate setup in main setup

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }

  server.on("/setlimits", HTTP_GET, []() {
  if (server.hasArg("min")) servoMin = server.arg("min").toInt();
  if (server.hasArg("max")) servoMax = server.arg("max").toInt();
  if (servoMin > servoMax) {
    int t = servoMin;
    servoMin = servoMax;
    servoMax = t;
  }
  Serial.print(" min");
  Serial.print(servoMin);
  Serial.print(" max");
  Serial.println(servoMax);
  server.send(200, "text/plain", "OK");
});

server.on("/setpos", HTTP_GET, []() {
  if (server.hasArg("pos")) {
    int servoDem ;
    servoPos = server.arg("pos").toInt();
    /*
    this test code is for a towerpro mg90s servo connected to channel 
    left position is 1 msec , centre is 1.5 msec and right is 2 msec
    i.e. 0 to 180 degrees
    */
    int pulse7 = map(servoPos, 0, 180, 200, 400);
    pwm1.setPWM(7, 0, pulse7) ;
    int pulse3 = map(servoPos, 0, 180, 150, 600);
    pwm1.setPWM(3, 0, pulse3) ;
    pwm1.setPWM(4, 0, pulse3) ;
    //myservo.writeMicroseconds(pulse);
    Serial.println(servoPos);
  }
  server.send(200, "text/plain", "OK");
});

  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", controlPage);
  });

  server.on("/ota", HTTP_GET, []() {
  server.sendHeader("Connection", "close");
  server.send(200, "text/html", otaIndex);
});

  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}


