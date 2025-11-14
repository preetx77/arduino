#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD setup: I2C address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo setup
Servo myservo;

// IR sensor pins
const int IR1 = 2;
const int IR2 = 3;

// Parking slot count
int Slot = 4;   // Total number of parking slots

// Flags for sensor states
int flag1 = 0;
int flag2 = 0;

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Configure IR sensors
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Attach servo to pin 4
  myservo.attach(4);
  myservo.write(100); // Initial position (closed)

  // Display welcome message
  lcd.setCursor(0, 0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Car entering (IR1 triggered)
  if (digitalRead(IR1) == LOW && flag1 == 0) {
    if (Slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(0);   // Open gate
        Slot = Slot - 1;    // Decrease slot count
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print("    SORRY :(    ");
      lcd.setCursor(0, 1);
      lcd.print("  Parking Full  ");
      delay(3000);
      lcd.clear();
    }
  }

  // Car exiting (IR2 triggered)
  if (digitalRead(IR2) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(0);   // Open gate
      Slot = Slot + 1;    // Increase slot count
    }
  }

  // Reset flags and close gate
  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    myservo.write(100);   // Close gate
    flag1 = 0;
    flag2 = 0;
  }

  // Display current status
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor(0, 1);
  lcd.print("Slot Left: ");
  lcd.print(Slot);
}
