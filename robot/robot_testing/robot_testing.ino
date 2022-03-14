#include <Adafruit_VL53L0X.h>


#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED width, pixel size
#define SCREEN_HEIGHT 32 // OLED height, pixel size
#define OLED_RESET -1 // Resetpin#(or -1 When connecting the aduino to the resetpin.)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

/*
   Backwards
  LOW, HIGH

  Forwards
  HIGH,LOW

  RIGHT, LEFT ?

  STOP LOW,LOW
*/


//Motor 1 RIGHT SIDE
int fwd1 = 17;  //right
int rev1 = 16;  //left


//Motor 2 LEFT SIDE
int fwd2 = 5;   //right
int rev2 = 18;  //left

// IR Sensor
int sensor1 = 39;
int sensor2 = 34;
int sensor1val = 0;
int sensor2val = 0;

void setup() {
  pinMode(fwd1, OUTPUT);
  pinMode(rev1, OUTPUT);
  pinMode(fwd2, OUTPUT);
  pinMode(rev2, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  Serial.begin(115200); // Enable serial monitor for debugging message output
  // SSD1306_SWITCHCAPVCC = Leave the inner 3.3v charge pump circuit on.
  // OLED Specify address 0x3c

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Not Connected"));
    for (;;);   // If SSD1306 is not addressed, infinite loop
  }
}

void fwd() {
  analogWrite(fwd1, 200);
  analogWrite(rev1, 0);
  analogWrite(fwd2, 220);
  analogWrite(rev2, 0);
}

void rev() {
  analogWrite(fwd1, 0);
  analogWrite(rev1, 200);
  analogWrite(fwd2, 0);
  analogWrite(rev2, 220);
}

void left() {
  analogWrite(fwd1, 200);
  analogWrite(rev1, 0);
  analogWrite(fwd2, 0);
  analogWrite(rev2, 0);
}

void right() {
  analogWrite(fwd1, 200);
  analogWrite(rev1, 0);
  analogWrite(fwd2, 220);
  analogWrite(rev2, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //sensor1val = analogRead(sensor1);
  //sensor2val = analogRead(sensor2);
  //Serial.println(sensor1val);

}
