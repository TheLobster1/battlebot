#include <HCSR04.h>
#include <WiFi.h>
#include <Adafruit_VL53L0X.h>
#include <analogWrite.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <HTTPClient.h>
#define SCREEN_WIDTH 128 // OLED width, pixel size
#define SCREEN_HEIGHT 32 // OLED height, pixel size
#define OLED_RESET -1 // Resetpin#(or -1 When connecting the aduino to the resetpin.)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
// wifi setup
const char* ssid     = "BattleBot";
const char* password = "43638253";

//String website = "battlebot3.server.nl";

WiFiServer server(80);
UltraSonicDistanceSensor distanceSensorLeft(14, 12);
UltraSonicDistanceSensor distanceSensorRight(33, 25);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
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

//int state = 0;
//void lineFollowState() {
//  switch (state) {
//    case 1:
//      rev();
//      break;
//    case 2:
//      left();
////      delay(200);
//      break;
//    case 3:
//      right();
////      delay(200);
//      break;
//    default:
//      Stop();
//  }
//}


void followLine() {
//  lineFollowState();

  if (sensor1val <= 400 && sensor2val <= 400) {
    speed1 = 125;
    speed2 = 125;
    rev();    //forward
  } 
  else if (sensor1val >= 400 && sensor2val <= 400) {
    speed1 = 180;
    speed2 = 180;
    Stop();
//delay(30);
    left();  
//    delay(125);
  }
  else if (sensor1val <=400 && sensor2val >= 400) {
    speed1 = 180;
    speed2 = 180;
    Stop();
    //delay(30);
    right();
//    delay(125);
  }
  else {
    fwd();  //reverse
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  // put your main code here, to run repeatedly:
  display.setCursor(0,0);
  display.print("IR left: "); display.print(sensor1val);
  display.print(" IR right: "); display.print(sensor2val);
  display.display();
  display.clearDisplay();
  display.display();
}

void race(int distance) {
  
//  distance = 
   
//speedboost setup
  sensor1val = analogRead(sensor1);
  sensor2val = analogRead(sensor2);
  
  if (distance >= 300) {
//  if (sensor1val >= 400 && sensor2val >= 400) {
//    speed1 = 250;
//    speed2 = 250;
//  }
//  else if (sensor1val <= 400 && sensor2val <= 400) {
//    speed1 = 200;
//    speed2 = 200;
//  }
  
  speed1 = 400;
  speed2 = 400;
  fwd();
  } else {
  Stop();
  }
}

void maze(int distance) {
    speed1 = 180;
  speed2 = 180;
  int ultraDistanceLeft = distanceSensorLeft.measureDistanceCm();
  int ultraDistanceRight = distanceSensorRight.measureDistanceCm();

  
  if (distance >= 200) {
    fwd();
  } else {
        speed1 = 200;
       speed2 = 200;
       
       if (ultraDistanceRight >= 5 && ultraDistanceLeft >= 5) {
      rev();
      delay(200);
    }else if (ultraDistanceRight >= 5) {
        Stop();
        delay(50);
        left();
        delay(800);
    } else if(ultraDistanceLeft >= 5) {
        Stop();
        delay(50);
        right();
        delay(800);
    } 
  }
  
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  // put your main code here, to run repeatedly:
  

  display.setCursor(0,0);
  display.println(ultraDistanceRight);
  display.println(ultraDistanceLeft);
  display.display();
  display.clearDisplay();
  display.display();
}

void setup() {
  pinMode(fwd1, OUTPUT);
  pinMode(rev1, OUTPUT);
  pinMode(fwd2, OUTPUT);
  pinMode(rev2, OUTPUT);

  // Infrared
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  //front distance sensor
  if (!lox.begin()) {
    Serial.println(F("Failed to boot"));
    while(1);
  }

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
  sensor1val = analogRead(sensor1);
  sensor2val = analogRead(sensor2);
   VL53L0X_RangingMeasurementData_t measure;
   lox.rangingTest(&measure, false);
   int distance = measure.RangeMilliMeter;

   
    if (currentState == "fwd") {
    speed1 = 180;
    speed2 = 180;
    fwd();
  }
  if (currentState == "rev") {
    speed1 = 180;
    speed2 = 180;
    rev();
  }
  if (currentState == "left") {
    speed1 = 180;
    speed2 = 180;
    left();
  }
  if (currentState == "right") {
    speed1 = 180;
    speed2 = 180;
    right();
  }
  if (currentState == "followLine") {
    followLine();
  }
  if (currentState == "Stop") {
    Stop();
  }
  if (currentState == "race") {
    race(distance);
  }
  if (currentState == "maze") {
    maze(distance);
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
        if (currentLine.endsWith("GET /Forward")) {
          currentState = "fwd";
        }
        if (currentLine.endsWith("GET /Back")) {
          currentState = "rev";
        }
        if (currentLine.endsWith("GET /TurnLeft")) {
          currentState = "left";
        }
        if (currentLine.endsWith("GET /TurnRight")) {
          currentState = "right";
        }
        if (currentLine.endsWith("GET /LineTrack")) {
          currentState = "followLine";
        }
        if (currentLine.endsWith("GET /Stop")) {
          currentState = "Stop";
        }
        if (currentLine.endsWith("GET /Race")) {
          currentState = "race";
        }
        if (currentLine.endsWith("GET /Maze")) {
          currentState = "maze";
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }





}
