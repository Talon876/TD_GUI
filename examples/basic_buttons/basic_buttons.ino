#include <SPI.h>
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FT6206.h"
#include "TD_GUI.h"

#define TFT_DC 9
#define TFT_CS 10
#define LED_PIN 3

Adafruit_ILI9341 gfx = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_FT6206 touch = Adafruit_FT6206();

TD_Button button = TD_Button(32, 30, 240, 80, 4);
TD_Button button2 = TD_Button(32, 130, 240, 80, 4);

void setup() {
  Serial.begin(115200);
  gfx.begin();
  touch.begin(40);
  gfx.setRotation(ROTATE_90);
  gfx.fillScreen(BLACK);
  
  button.onButtonPressed = &buttonOnePressed;
  button.onButtonReleased = &buttonOneReleased;
  button.init(gfx);
  
  button2.primaryColor = RED;
  button2.secondaryColor = MAROON;
  button2.init(gfx);
  
  pinMode(LED_PIN, OUTPUT);
}

void loop(void) {
  button.update(touch, gfx);
  button2.update(touch, gfx);
}

void buttonOnePressed() {
  Serial.println("Button one was pressed");
  digitalWrite(LED_PIN, HIGH);
}

void buttonOneReleased() {
  Serial.println("Button one was released");
  digitalWrite(LED_PIN, LOW);
}

