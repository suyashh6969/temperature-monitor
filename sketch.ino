// Temperature monitor: Arduino Uno + NTC temperature sensor + 16x2 LCD + LED
// Shows the temperature on the LCD and lights the LED when it exceeds 25 C.

#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
const int LCD_RS = 12;
const int LCD_E  = 11;
const int LCD_D4 = 5;
const int LCD_D5 = 4;
const int LCD_D6 = 3;
const int LCD_D7 = 2;
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

const int SENSOR_PIN = A0;        // NTC sensor output
const int LED_PIN = 8;            // alert LED (through a 220 ohm resistor)
const float THRESHOLD_C = 25.0;   // alert temperature in degrees Celsius
const float BETA = 3950.0;        // beta coefficient of the NTC thermistor

float readTemperatureC() {
  int raw = analogRead(SENSOR_PIN);
  if (raw <= 0 || raw >= 1023) {
    return NAN;                   // avoid dividing by zero at the extremes
  }
  // Beta equation with a 10k NTC at 25 C (298.15 K)
  return 1.0 / (log(1.0 / (1023.0 / raw - 1.0)) / BETA + 1.0 / 298.15) - 273.15;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Temp Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1200);
  lcd.clear();
}

void loop() {
  float tempC = readTemperatureC();

  lcd.setCursor(0, 0);
  if (isnan(tempC)) {
    lcd.print("Sensor error    ");
    digitalWrite(LED_PIN, LOW);
  } else {
    lcd.print("Temp: ");
    lcd.print(tempC, 1);
    lcd.print((char)223);         // degree symbol
    lcd.print("C   ");

    lcd.setCursor(0, 1);
    if (tempC > THRESHOLD_C) {
      digitalWrite(LED_PIN, HIGH);
      lcd.print("ALERT: Too hot! ");
    } else {
      digitalWrite(LED_PIN, LOW);
      lcd.print("Status: Normal  ");
    }
  }
  delay(500);
}
