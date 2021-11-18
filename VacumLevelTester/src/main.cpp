#include <Arduino.h>
#include "Adafruit_MPRLS.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define LED 2

// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN -1 // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN -1   // set to any GPIO pin to read end-of-conversion by pin

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET 4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


int CURRENT_X = 0;


Adafruit_SSD1306 SSD1306_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  if (!SSD1306_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  SSD1306_display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  SSD1306_display.clearDisplay();
  SSD1306_display.display();

  Serial.println("MPRLS Simple Test");
  if (!mpr.begin())
  {
    Serial.println("Failed to communicate with MPRLS sensor, check wiring?");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("Found MPRLS sensor");
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float CalculateVacuum(float pressure_hPa)
{
  float KP = pressure_hPa / 10;
  // Serial.print("pressuress: ");
  //   Serial.print(KP);

  if (KP > 100)
    return 0;
  float result = mapfloat(KP, 100.00, 0.00, 0.00, 100.00);
  Serial.println(result);
  return result;
}

void WriteGraph(float vacuumPercentage)
{
  int TOP = 15;
  int LENGHT = 64;
// clear space first
  SSD1306_display.drawRect(CURRENT_X, TOP, 2, LENGHT, BLACK);
  int valueGraph = map(vacuumPercentage, 0, 100, 0, 39);

  int Calculated_Y = 64 - valueGraph;
  SSD1306_display.setCursor(CURRENT_X, 16);
  SSD1306_display.drawRect(CURRENT_X, Calculated_Y, 1, valueGraph, WHITE);

  CURRENT_X = CURRENT_X + 1;
  if (CURRENT_X == 128)
  {
    CURRENT_X = 0;
  }
}
void WriteLCD(float pressure_hPa)
{
  SSD1306_display.fillRect(0, 0, 128, 15, WHITE);

  float PSI = pressure_hPa / 68.947572932;
  float percentageVacuum = CalculateVacuum(pressure_hPa);
  SSD1306_display.setTextSize(1);
  SSD1306_display.setTextColor(BLACK);

  SSD1306_display.setCursor(0, 3);
  SSD1306_display.println(" mbar               %");

  SSD1306_display.setCursor(30, 3);
  SSD1306_display.println(pressure_hPa);

  SSD1306_display.setCursor(82, 3);
  SSD1306_display.println(percentageVacuum, 3);

  // Serial.print("Pressure (hPa): ");
  // Serial.println(pressure_hPa);
  // Serial.print("Pressure (PSI): ");
  // Serial.println(PSI);

  SSD1306_display.display();
}

void loop()
{
  float pressure_hPa = mpr.readPressure();
  WriteLCD(pressure_hPa);
  WriteGraph(CalculateVacuum(pressure_hPa));
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);
  digitalWrite(LED, HIGH);
}
