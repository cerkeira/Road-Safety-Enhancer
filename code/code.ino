// Include the NeoPixel library to control RGB LED strips
#include <Adafruit_NeoPixel.h>

// Include BluetoothSerial library for Bluetooth communication (ESP32 only)
#include "BluetoothSerial.h"

// Create a BluetoothSerial object named SerialBT
BluetoothSerial SerialBT;

// Define LED strip pin and number of LEDs
#define PIN_LED 14
#define NUM_LED 9

// Define ultrasonic sensor pins for LEFT
#define TRIG_LEFT 12
#define ECHO_LEFT 13

// Define ultrasonic sensor pins for CENTER
#define TRIG_CENTER 5
#define ECHO_CENTER 18

// Define ultrasonic sensor pins for RIGHT
#define TRIG_RIGHT 21
#define ECHO_RIGHT 19

// Create NeoPixel strip object: number of LEDs, pin, color order and frequency
Adafruit_NeoPixel strip(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

// Set default and alert brightness values for the LED strip
const int fixedBrightness = 180;
const int alertBrightness = 220;

// Threshold for detecting rapid distance drop (used to detect fast-approaching object)
const float DISTANCE_DROP_THRESHOLD = 150.0; // in cm
const unsigned long TIME_WINDOW = 300;       // in milliseconds

// Variables to store last measured distances
float lastDistanceLeft = 1000;
float lastDistanceCenter = 1000;
float lastDistanceRight = 1000;

// Timing variables to manage how often readings and messages are processed
unsigned long lastCheckLeft = 0;
unsigned long lastCheckCenter = 0;
unsigned long lastCheckRight = 0;
unsigned long lastSentL = 0, lastSentC = 0, lastSentR = 0;

// Minimum interval between sending Bluetooth messages, this logic is needed to prevent the android app from crashing
const unsigned long sendInterval = 1000;

void setup() {
  Serial.begin(115200);            // Start serial monitor for debugging
  strip.begin();                   // Initialize the NeoPixel strip
  strip.setBrightness(fixedBrightness); // Set default brightness
  strip.show();                    // Turn off all LEDs initially
  SerialBT.begin("Road Safety Enhancer"); // Start Bluetooth with device name

  // Set ultrasonic sensor pins as INPUT/OUTPUT
  pinMode(TRIG_LEFT, OUTPUT);   pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_CENTER, OUTPUT); pinMode(ECHO_CENTER, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);  pinMode(ECHO_RIGHT, INPUT);
}

void loop() {
  unsigned long now = millis(); // Current time in ms

  // Measure distances using each ultrasonic sensor
  float distanceLeft = getDistance(TRIG_LEFT, ECHO_LEFT);
  float distanceCenter = getDistance(TRIG_CENTER, ECHO_CENTER);
  float distanceRight = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  // Print distances to Serial Monitor for debugging
  Serial.print("Left: "); Serial.print(distanceLeft);
  Serial.print(" cm | Center: "); Serial.print(distanceCenter);
  Serial.print(" cm | Right: "); Serial.println(distanceRight);

  // ===== LEFT SENSOR LOGIC =====
  if (isFastClosing(distanceLeft, lastDistanceLeft, now - lastCheckLeft) && now - lastSentL > sendInterval) {
    flashWarning(0, 2);                  // Flash LEDs 0 to 2
    SerialBT.println("LF");              // Send "Left Fast" alert over Bluetooth
    lastSentL = now;
  } else {
    setStripColor(0, 2, getColorFromDistance(distanceLeft)); // Set color based on distance
    if (distanceLeft < 20 && now - lastSentL > sendInterval) {
      SerialBT.println("L");             // Send "Left Close" alert
      lastSentL = now;
    }
  }
  lastDistanceLeft = distanceLeft;
  lastCheckLeft = now;

  // ===== CENTER SENSOR LOGIC =====
  if (isFastClosing(distanceCenter, lastDistanceCenter, now - lastCheckCenter) && now - lastSentL > sendInterval) {
    flashWarning(3, 5);                  // Flash LEDs 3 to 5
    SerialBT.println("CF");              // Send "Center Fast" alert
    lastSentL = now;
  } else {
    setStripColor(3, 5, getColorFromDistance(distanceCenter)); // Set color based on distance
    if (distanceCenter < 20 && now - lastSentC > sendInterval) {
      SerialBT.println("C");             // Send "Center Close" alert
      lastSentC = now;
    }
  }
  lastDistanceCenter = distanceCenter;
  lastCheckCenter = now;

  // ===== RIGHT SENSOR LOGIC =====
  if (isFastClosing(distanceRight, lastDistanceRight, now - lastCheckRight) && now - lastSentL > sendInterval) {
    flashWarning(6, 9);                  // Flash LEDs 6 to 8
    SerialBT.println("RF");              // Send "Right Fast" alert
    lastSentL = now;
  } else {
    setStripColor(6, 9, getColorFromDistance(distanceRight)); // Set color based on distance
    if (distanceRight < 20 && now - lastSentR > sendInterval) {
      SerialBT.println("R");             // Send "Right Close" alert
      lastSentR = now;
    }
  }
  lastDistanceRight = distanceRight;
  lastCheckRight = now;

  strip.show();      // Update LED strip with new colors
  delay(400);        // Wait a short time before the next loop
}

// Measure distance using ultrasonic sensor (returns distance in cm)
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(50);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 150000); // Timeout after 150ms
  if (duration == 0) return 4444;  // If no pulse received, return max error value
  return duration * 0.034 / 2.0;   // Convert to distance in cm
}

// Set a range of LEDs to a specific color
void setStripColor(int startLED, int endLED, uint32_t color) {
  for (int i = startLED; i <= endLED; i++) {
    strip.setPixelColor(i, color);
  }
}

// Return LED color depending on distance
uint32_t getColorFromDistance(float distance) {
  if (distance < 80) return strip.Color(255, 0, 0);       // Red: very close
  else if (distance < 150) return strip.Color(255, 100, 0); // Orange
  else if (distance < 280) return strip.Color(255, 255, 0); // Yellow
  else if (distance < 450) return strip.Color(0, 0, 255);   // Blue: far object
  else return strip.Color(0, 0, 0);                         // Off: very far
}

// Determine if an object is approaching fast
bool isFastClosing(float current, float previous, unsigned long elapsed) {
  return (previous - current >= DISTANCE_DROP_THRESHOLD && elapsed <= TIME_WINDOW);
}

// Flash LEDs red quickly 3 times to indicate fast approaching object warning
void flashWarning(int startLED, int endLED) {
  for (int j = 0; j < 3; j++) {
    strip.setBrightness(alertBrightness);
    setStripColor(startLED, endLED, strip.Color(255, 0, 0)); // Turn on red
    strip.show();
    delay(100);
    setStripColor(startLED, endLED, strip.Color(0, 0, 0));   // Turn off
    strip.show();
    delay(100);
  }
  strip.setBrightness(fixedBrightness); // Restore original brightness
}
