

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Pin definitions for ESP32-S3-DevKitC-1
#define BOOT_BUTTON_PIN 0     // Boot button is connected to GPIO0
#define RGB_LED_PIN     38    // Onboard addressable RGB LED data pin
#define LED_COUNT       1     // Only one LED onboard

// NeoPixel object
Adafruit_NeoPixel led(LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

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

  // Initialise button
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

  // Initialise NeoPixel
  led.begin();
  led.show(); // Start with all LEDs off

  // Set initial LED state
  setLedState(RED);

  Serial.println("ESP32-S3-DevKitC-1 initialised with onboard RGB LED");
}

void setLedState(LEDState state) {
  uint32_t color = 0;

  switch (state) {
    case RED:   color = led.Color(255,   0,   0); break;
    case GREEN: color = led.Color(  0, 255,   0); break;
    case BLUE:  color = led.Color(  0,   0, 255); break;
  }

  led.setPixelColor(0, color);
  led.show();

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


