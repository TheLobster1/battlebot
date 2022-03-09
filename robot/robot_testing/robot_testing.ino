#include <Adafruit_VL53L0X.h>
#include <vl53l0x_api.h>
#include <vl53l0x_api_calibration.h>
#include <vl53l0x_api_core.h>
#include <vl53l0x_api_ranging.h>
#include <vl53l0x_api_strings.h>
#include <vl53l0x_def.h>
#include <vl53l0x_device.h>
#include <vl53l0x_i2c_platform.h>
#include <vl53l0x_interrupt_threshold_settings.h>
#include <vl53l0x_platform.h>
#include <vl53l0x_platform_log.h>
#include <vl53l0x_tuning.h>
#include <vl53l0x_types.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#define SCREEN_WIDTH 128 // OLED width, pixel size
#define SCREEN_HEIGHT 32 // OLED height, pixel size
#define OLED_RESET -1 // Resetpin#(or -1 When connecting the aduino to the resetpin.)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
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


void setup() {
  pinMode(fwd1, OUTPUT);
  pinMode(rev1, OUTPUT);
  pinMode(fwd2, OUTPUT);
  pinMode(rev2, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  Serial.begin(9600); // Enable serial monitor for debugging message output
  // SSD1306_SWITCHCAPVCC = Leave the inner 3.3v charge pump circuit on.
  // OLED Specify address 0x3c

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Not Connected"));
    for (;;);   // If SSD1306 is not addressed, infinite loop
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(fwd1, HIGH);
  digitalWrite(rev1, LOW);
  digitalWrite(fwd2, HIGH);
  digitalWrite(rev2, LOW);
  display.clearDisplay();

}
