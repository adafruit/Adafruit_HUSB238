#include <Wire.h>
#include "Adafruit_HUSB238.h"

Adafruit_HUSB238 husb238;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Adafruit HUSB238 Test Sketch");

  // Initialize the HUSB238
  if (husb238.begin(HUSB238_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("HUSB238 initialized successfully.");
  } else {
    Serial.println("Couldn't find HUSB238, check your wiring?");
    while (1);
  }
}

void loop() {
  delay(1000);  // Add a delay to prevent flooding the serial output
  Serial.println(F("----------------------------------------------"));

  if (! husb238.isAttached()) 
    return;

  if (husb238.getPDResponse() != SUCCESS)
    return;

  // What voltages and currents are available from this adapter?
  Serial.println("Available PD Voltages and Current Detection Test:");
  for (int i = PD_SRC_5V; i <= PD_SRC_20V; i++) {
    bool voltageDetected = husb238.isVoltageDetected((HUSB238_PDSelection)i);

    switch ((HUSB238_PDSelection)i) {
      case PD_SRC_5V:
        Serial.print("5V");
        break;
      case PD_SRC_9V:
        Serial.print("9V");
        break;
      case PD_SRC_12V:
        Serial.print("12V");
        break;
      case PD_SRC_15V:
        Serial.print("15V");
        break;
      case PD_SRC_18V:
        Serial.print("18V");
        break;
      case PD_SRC_20V:
        Serial.print("20V");
        break;
      default:
        continue;
    }
    Serial.println(voltageDetected ? " Available" : " Unavailable");

    Serial.println("\tSetting new PD voltage");
    // Change to that voltage
    husb238.selectPD((HUSB238_PDSelection)i);
    // Perform the actual PD voltage request!
    husb238.requestPD();

    delay(2000);
  }
}
