#include <Arduino.h>

// Pin definitions for ESP32-S3-DevKitC-1
#define BOOT_BUTTON_PIN 0     // Boot button is connected to GPIO0
#define RGB_LED_PIN     38    // Onboard addressable RGB LED data pin

// LED state variables
enum LEDState { RED, GREEN, BLUE };
LEDState currentLedState = RED;
bool lastButtonState = HIGH;  // Button is active LOW, so start with HIGH
bool buttonPressed = false;

// Function prototypes
void setLedState(LEDState state);
void cycleLedState();
void handleButtonPress();
void handleSerialData();

void setup() {
  Serial.begin(115200);
  
  // Initialize button
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
  
  // Turn off LED initially
  neopixelWrite(RGB_LED_PIN, 0, 0, 0);
  
  // Set initial LED state
  setLedState(RED);
  
  Serial.println("ESP32-S3-DevKitC-1 initialised with onboard RGB LED");
}

void setLedState(LEDState state) {
  switch (state) {
    case RED:
      neopixelWrite(RGB_LED_PIN, 255, 0, 0);
      break;
    case GREEN:
      neopixelWrite(RGB_LED_PIN, 0, 255, 0);
      break;
    case BLUE:
      neopixelWrite(RGB_LED_PIN, 0, 0, 255);
      break;
  }
  
  currentLedState = state;
  
  Serial.print("LED state changed to: ");
  switch (state) {
    case RED:   Serial.println("RED");   break;
    case GREEN: Serial.println("GREEN"); break;
    case BLUE:  Serial.println("BLUE");  break;
  }
}

void cycleLedState() {
  switch (currentLedState) {
    case RED:   setLedState(GREEN); break;
    case GREEN: setLedState(BLUE);  break;
    case BLUE:  setLedState(RED);   break;
  }
}

void handleButtonPress() {
  bool currentButtonState = digitalRead(BOOT_BUTTON_PIN);
  
  // Detect falling edge (press)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50); // debounce
    if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
      buttonPressed = true;
    }
  }
  
  // Detect rising edge (release)
  if (lastButtonState == LOW && currentButtonState == HIGH && buttonPressed) {
    Serial.println("Hello from io24m006");
    buttonPressed = false;
    //cycleLedState(); // also cycle LED on button release
  }
  
  lastButtonState = currentButtonState;
}

void handleSerialData() {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == 'n') {
      Serial.println("Received 'n' - cycling LED");
      cycleLedState();
    } else {
      //Serial.print("Echo: ");
      Serial.println(receivedChar);
    }
  }
}

void loop() {
  handleButtonPress();
  handleSerialData();
  delay(10); // small delay to reduce CPU usage
}