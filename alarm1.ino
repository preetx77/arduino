#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDR 0x27
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

const int smokePin = A0;      // change only if your sensor uses another pin
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 500; // ms

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Fire Alarm Boot");
  lcd.setCursor(0,1);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
}

void loop() {
  int raw = analogRead(smokePin);          // 0 - 1023
  int smokePercent = map(raw, 0, 1023, 0, 100);
  if (smokePercent < 0) smokePercent = 0;
  if (smokePercent > 999) smokePercent = 999;

  // Serial output (your existing behaviour)
  Serial.print("Smoke: ");
  Serial.println(smokePercent);

  // Update LCD only at set intervals
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();

    lcd.setCursor(0,0);
    lcd.print("Smoke:");
    char buf[6];
    snprintf(buf, sizeof(buf), "%3d%%", smokePercent);
    lcd.setCursor(7,0);
    lcd.print(buf);         // writes e.g. " 98%"

    lcd.setCursor(0,1);
    if (smokePercent >= 70) {
      lcd.print("ALARM! Evacuate ");
    } else if (smokePercent >= 40) {
      lcd.print("Warning         ");
    } else {
      lcd.print("Normal          ");
    }
  }

  delay(100);
}
