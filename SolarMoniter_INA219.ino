#include <Wire.h>
#include <Adafruit_INA219.h>

// Create INA219 object
Adafruit_INA219 ina219;

void setup() {
  Serial.begin(9600);

  // Initialize INA219
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) {
      delay(10);
    }
  }

  // Optional calibration modes:
  // ina219.setCalibration_32V_1A();
  //ina219.setCalibration_16V_400mA();

  Serial.println("INA219 Sensor Initialized");
  delay(1000);
}

void loop() {
  float shuntVoltage_mV = 0;
  float busVoltage_V = 0;
  float current_mA = 0;
  float power_mW = 0;
  float loadVoltage_V = 0;

  // Read sensor values
  shuntVoltage_mV = ina219.getShuntVoltage_mV();
  busVoltage_V = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();

  // Load voltage calculation
  loadVoltage_V = busVoltage_V + (shuntVoltage_mV / 1000);

  // Print results
  Serial.print("Bus Voltage:   ");
  Serial.print(busVoltage_V);
  Serial.println(" V");

  Serial.print("Shunt Voltage: ");
  Serial.print(shuntVoltage_mV);
  Serial.println(" mV");

  Serial.print("Load Voltage:  ");
  Serial.print(loadVoltage_V);
  Serial.println(" V");

  Serial.print("Current:       ");
  Serial.print(current_mA / 1000, 2);
  Serial.println(" A");

  Serial.print("Power:         ");
  Serial.print(power_mW / 1000, 2);
  Serial.println(" W");

  Serial.println("----------------------------------");

  delay(2000);
}