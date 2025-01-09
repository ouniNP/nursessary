#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// RFID RC522 pins
#define RST_PIN 35   // RST connected to GPIO 35
#define SS_PIN  5    // SDA (SS) connected to GPIO 5

// Custom SPI pins
#define SCK_PIN 18   // SCK connected to GPIO 18
#define MISO_PIN 39  // MISO connected to GPIO 39
#define MOSI_PIN 19  // MOSI connected to GPIO 19

// Initialize RFID reader
MFRC522 rfid(SS_PIN, RST_PIN);

// Initialize I2C LCD (change address if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buzzer and button pins
#define BUZZER_PIN 47
#define BUTTON_PIN 36

// Variables for buzzer control
bool buzzerState = false; // Whether the buzzer is active
bool buttonPressedToStop = false; // Track if the button has stopped the buzzer
unsigned long lastBuzzTime = 0;   // Time tracking for buzzer intervals
unsigned long buzzInterval = 1500; // Interval for buzzer beeping
bool isBuzzerOn = false;           // Current state of the buzzer

// Initialize a string to store the UID
String uidString = "";

// Define the User structure
struct User {
    String name;
    String uid;
    String medicine;
    String time;
};

// Define the users
User users[] = {
    {"Alice", "949cc801", "Panadol", "10:00AM"},
    {"Tom", "cead0304", "Antibiotic", "8:00AM"}
};

void setup() {
  Wire.begin(21, 20);
  Serial.begin(115200);

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

  rfid.PCD_Init();
  Serial.println("Place your RFID card near the reader...");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("RFID Reader");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure the buzzer is off
}

void loop() {
  // Check if a new card is present
  if (!rfid.PICC_IsNewCardPresent()) {
    if (buzzerState && !buttonPressedToStop) {
      controlBuzzer(); // Keep the buzzer beeping
    }
    checkButton(); // Continuously check the button state
    return;
  }

  // Check if the card can be read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Process the RFID card UID
  uidString = ""; // Reset the UID string
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      uidString += "0"; // Add leading zero if necessary
    }
    uidString += String(rfid.uid.uidByte[i], HEX);
  }

  Serial.println("Card Scanned: " + uidString);

  // Check if the UID matches any stored user
  bool isAuthorized = false;
  for (int i = 0; i < sizeof(users) / sizeof(users[0]); i++) {
    if (uidString == users[i].uid) {
      isAuthorized = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hello, " + users[i].name);
      lcd.setCursor(0, 1);
      lcd.print("Dispensing...");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(users[i].medicine);
      lcd.setCursor(0, 1);
      lcd.print("Take at: " + users[i].time);
      
      // Start buzzer loop
      buzzerState = true;
      buttonPressedToStop = false; // Reset button control
      break;
    }
  }

  if (!isAuthorized) {
    Serial.println("Access Denied");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
  }

  // Halt the card to avoid re-reading
  rfid.PICC_HaltA();
}

void checkButton() {
  static bool lastButtonState = HIGH; // Store the previous button state
  bool buttonState = digitalRead(BUTTON_PIN);

  // Check for button press
  if (buttonState == LOW && lastButtonState == HIGH) {
    if (buzzerState) {
      buttonPressedToStop = true; // Stop the buzzer forever
      buzzerState = false;
      digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
      Serial.println("Buzzer stopped permanently.");
    }
  }

  lastButtonState = buttonState;
}

void controlBuzzer() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastBuzzTime >= buzzInterval) {
    lastBuzzTime = currentMillis;
    isBuzzerOn = !isBuzzerOn; // Toggle buzzer state
    digitalWrite(BUZZER_PIN, isBuzzerOn ? HIGH : LOW);
  }
}

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// RFID RC522 pins
#define RST_PIN 35   // RST connected to GPIO 35
#define SS_PIN  5    // SDA (SS) connected to GPIO 5

// Custom SPI pins
#define SCK_PIN 18   // SCK connected to GPIO 18
#define MISO_PIN 39  // MISO connected to GPIO 39
#define MOSI_PIN 19  // MOSI connected to GPIO 19

// Initialize RFID reader
MFRC522 rfid(SS_PIN, RST_PIN);

// Initialize I2C LCD (change address if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buzzer and button pins
#define BUZZER_PIN 47
#define BUTTON_PIN 36

// Variables for buzzer control
bool buzzerState = false; // Whether the buzzer is active
bool buttonPressedToStop = false; // Track if the button has stopped the buzzer
unsigned long lastBuzzTime = 0;   // Time tracking for buzzer intervals
unsigned long buzzInterval = 1500; // Interval for buzzer beeping
bool isBuzzerOn = false;           // Current state of the buzzer
String user = "";
String medicine = "";

// Initialize a string to store the UID
String uidString = "";

// Define the User structure
struct User {
    String name;
    String uid;
    String medicine;
    String time;
};

// Define the users
User users[] = {
    {"Alice", "949cc801", "Panadol", "10:00AM"},
    {"Tom", "cead0304", "Antibiotic", "8:00AM"}
};

void setup() {
  Wire.begin(21, 20);
  Serial.begin(115200);

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

  rfid.PCD_Init();
  Serial.println("Place your RFID card near the reader...");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("RFID Reader");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure the buzzer is off
}

void loop() {
  // Check if a new card is present
  if (!rfid.PICC_IsNewCardPresent()) {
    if (buzzerState && !buttonPressedToStop) {
      controlBuzzer(); // Keep the buzzer beeping
    }
    checkButton(); // Continuously check the button state
    return;
  }

  // Check if the card can be read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Process the RFID card UID
  uidString = ""; // Reset the UID string
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      uidString += "0"; // Add leading zero if necessary
    }
    uidString += String(rfid.uid.uidByte[i], HEX);
  }

  Serial.println("Card Scanned: " + uidString);

  // Check if the UID matches any stored user
  bool isAuthorized = false;
  for (int i = 0; i < sizeof(users) / sizeof(users[0]); i++) {
    if (uidString == users[i].uid) {
      isAuthorized = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hello, " + users[i].name);
      lcd.setCursor(0, 1);
      lcd.print("Dispensing...");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(users[i].medicine);
      lcd.setCursor(0, 1);
      lcd.print("Take at: " + users[i].time);
      
      // Start buzzer loop
      buzzerState = true;
      buttonPressedToStop = false; // Reset button control
      user = users[i].name;
      medicine = users[i].medicine;
      break;
    }
  }

  if (!isAuthorized) {
    Serial.println("Access Denied");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
  }

  // Halt the card to avoid re-reading
  rfid.PICC_HaltA();
}

void checkButton() {
  static bool lastButtonState = HIGH; // Store the previous button state
  bool buttonState = digitalRead(BUTTON_PIN);

  // Check for button press
  if (buttonState == LOW && lastButtonState == HIGH) {
    if (buzzerState) {
      buttonPressedToStop = true; // Stop the buzzer forever
      buzzerState = false;
      digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
      Serial.println(user + " has consumed " + medicine);
    }
  }

  lastButtonState = buttonState;
}

void controlBuzzer() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastBuzzTime >= buzzInterval) {
    lastBuzzTime = currentMillis;
    isBuzzerOn = !isBuzzerOn; // Toggle buzzer state
    digitalWrite(BUZZER_PIN, isBuzzerOn ? HIGH : LOW);
  }
}
