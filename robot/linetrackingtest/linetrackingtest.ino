#include <WiFi.h>
#include <Adafruit_VL53L0X.h>
#include <analogWrite.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128 // OLED width, pixel size
#define SCREEN_HEIGHT 32 // OLED height, pixel size
#define OLED_RESET -1 // Resetpin#(or -1 When connecting the aduino to the resetpin.)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
// wifi setup
const char* ssid     = "yesman";
const char* password = "abcdefghijk";
String website = "battlebot3.server.nl";
WiFiServer server(80);

/*
   Backwards
  LOW, HIGH

  Forwards
  HIGH,LOW

  RIGHT, LEFT ?

  STOP LOW,LOW
*/


//Motor 1 RIGHT SIDE
int fwd1 = 17;  
int rev1 = 16;  


//Motor 2 LEFT SIDE
int fwd2 = 5;   
int rev2 = 18;  

// IR Sensor
int sensor1 = 39;
int sensor2 = 34;
int sensor1val = 0;
int sensor2val = 0;

// speed per game
// line tracking
int speed1 = 0;
int speed2 = 0;

String currentState = "Stop";

int state = 0;
void lineFollowState() {
  switch (state) {
    case 1:
      rev();
      break;
    case 2:
      left();
//      delay(200);
      break;
    case 3:
      right();
//      delay(200);
      break;
    default:
      Stop();
  }
}

void followLine() {
  lineFollowState();
  if (sensor1val <= 400 && sensor2val <= 400) {
    speed1 = 140;
    speed2 = 140;
//    robot.rotate(1, 50, 2);
//    robot.rotate(2, 50, 2);
    state = 1; 
  } 
  else if (sensor1val >= 300 && sensor2val <= 300) {
    speed1 = 160;
    speed2 = 160;
    state = 2;
  }
  else if (sensor1val <=300 && sensor2val >=300) {
    speed1 = 160;
    speed2 = 160;
    state = 3;
  }
  else {
    state = 0;
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  // put your main code here, to run repeatedly:
  sensor1val = analogRead(sensor1);
  sensor2val = analogRead(sensor2);
  display.setCursor(0,0);
  display.print("IR left: "); display.print(sensor1val);
  display.print(" IR right: "); display.print(sensor2val);
  display.display();
  display.clearDisplay();
  display.display();
}

void race() {
  
//  distance = 
  
//  if (distance >= 300) {
  speed1 = 200;
  speed2 = 200;
  fwd();
//  } else {
//  analogWrite(fwd1, 0);
//  analogWrite(rev1, 0);
//  analogWrite(fwd2, 0);
//  analogWrite(rev2, 0);
//  }
}

void setup() {
  pinMode(fwd1, OUTPUT);
  pinMode(rev1, OUTPUT);
  pinMode(fwd2, OUTPUT);
  pinMode(rev2, OUTPUT);

  // Infrared
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  Serial.begin(115200); // Enable serial monitor for debugging message output
  // SSD1306_SWITCHCAPVCC = Leave the inner 3.3v charge pump circuit on.
  // OLED Specify address 0x3c
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Not Connected"));
    for (;;);   // If SSD1306 is not addressed, infinite loop
  }
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        display.display();
    }
    display.clearDisplay();
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    display.display();
    server.begin();

}

void Stop() {
  analogWrite(fwd1, 0);
  analogWrite(rev1, 0);
  analogWrite(fwd2, 0);
  analogWrite(rev2, 0);
}

void fwd() {
  analogWrite(fwd1, speed1);
  analogWrite(rev1, 0);
  analogWrite(fwd2, speed2);
  analogWrite(rev2, 0);
}

void rev() {
  analogWrite(fwd1, 0);
  analogWrite(rev1, speed1);
  analogWrite(fwd2, 0);
  analogWrite(rev2, speed2);
}

void left() {
  analogWrite(fwd1, speed1);
  analogWrite(rev1, 0);
  analogWrite(fwd2, 0);
  analogWrite(rev2, speed2);
}

void right() {
  analogWrite(fwd1, 0);
  analogWrite(rev1, speed1);
  analogWrite(fwd2, speed2);
  analogWrite(rev2, 0);
}

void loop() {
    if (currentState == "fwd") {
    fwd();
  }
  if (currentState == "rev") {
    rev();
  }
  if (currentState == "left") {
    left();
  }
  if (currentState == "right") {
    right();
  }
  if (currentState == "followLine") {
    followLine();
  }
  if (currentState == "Stop") {
    Stop();
  }
  if (currentState == "race") {
    race();
  }
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /1")) {
          currentState = "fwd";
        }
        if (currentLine.endsWith("GET /2")) {
          currentState = "rev";
        }
        if (currentLine.endsWith("GET /3")) {
          currentState = "left";
        }
        if (currentLine.endsWith("GET /4")) {
          currentState = "right";
        }
        if (currentLine.endsWith("GET /5")) {
          currentState = "followLine";
        }
        if (currentLine.endsWith("GET /6")) {
          currentState = "Stop";
        }
        if (currentLine.endsWith("GET /7")) {
          currentState = "race";
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }





}