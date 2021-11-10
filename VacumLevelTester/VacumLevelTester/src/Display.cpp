#include "Display.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 SSD1306_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Display::Display()
{
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!SSD1306_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
}

void Display::Setup()
{
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  SSD1306_display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  SSD1306_display.clearDisplay();

  // Draw a single pixel in white
  SSD1306_display.drawPixel(10, 10, SSD1306_WHITE);
}