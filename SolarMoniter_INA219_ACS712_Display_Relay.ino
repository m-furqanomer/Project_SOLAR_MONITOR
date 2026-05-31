#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>

#define ACS712_PIN A0
#define RELAY_PIN 7
#define BATTERY_VOLTAGE_PIN A2

// ======================================================
// LCD CONFIGURATION
// ======================================================

//uint8_t byte = 16;

// LCD address may be 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 4);


// ======================================================
// BATTERY VOLATGE CONFIGURATION
// ======================================================

// Calibration Multiplier based on your real-world readings:
// At 15V input, your divider outputs 3.58V.
// 15V 3.58V
// 14V 3.33V
// 13V 3.09V
// 12V 2.85V
// 11V 2.60V
// 10V 2.36V
//  9V 2.12V
//  8V 1.88V
// With a 5.0V Arduino reference and 1023 ADC steps:
// 15.0 Volts / ((3.58 Volts / 5.0 Volts) * 1023) = 0.02047
const float CALIBRATION_FACTOR = 0.02047;

// ======================================================
// INA219 OBJECT
// ======================================================
Adafruit_INA219 ina219;

// ======================================================
// ACS712 CONFIGURATION
// ======================================================
float stepVoltage = 0.0048875855;  // 5V / 1023
float sensitivity = 0.100;         // 100mV/A for ACS712-20A
float offsetVoltage = 2.51;        // Calibrate this value properly
bool chargeState = 0;

// ======================================================
// FUNCTION TO READ ACS712 SENSOR
// ======================================================
void readACS712(float &busVoltage, float &current) {
  float adcValue;
  float adcVoltage;

  // Optional averaging for stability
  adcValue = 0;

  // for (int i = 0; i < 10; i++) {
  //   adcValue += analogRead(ACS712_PIN);
  //   delayMicroseconds(50);
  // }

  // adcValue /= 10.0;

  adcValue = analogRead(ACS712_PIN);

  // Convert ADC to Voltage
  adcVoltage = adcValue * stepVoltage;

  // Remove Offset
  busVoltage = adcVoltage - offsetVoltage;

  // Current Calculation
  current = busVoltage / sensitivity;

  chargeState = 0;
  // Make current positive
  if (current < 0) {
    current *= -1;
    chargeState = 1;
  }
}

// ======================================================
// FUNCTION TO READ BATTERY VOLTAGE
// ======================================================

void readBatteryVoltage(float &voltage) {
  int rawADC = analogRead(BATTERY_VOLTAGE_PIN);  // A2
  Serial.println();
  Serial.print("RAW ADC VAlues: ");
  Serial.print(rawADC);
  Serial.println();
  voltage = rawADC * CALIBRATION_FACTOR;
  return;
}

// ======================================================
// FUNCTION TO READ INA219 SENSOR
// ======================================================
void readINA219(float &busVoltage_V,
                float &shuntVoltage_mV,
                float &loadVoltage_V,
                float &current_A,
                float &power_W) {
  // Read sensor data
  shuntVoltage_mV = ina219.getShuntVoltage_mV();
  busVoltage_V = ina219.getBusVoltage_V();

  // Convert values
  current_A = ina219.getCurrent_mA();
  power_W = ina219.getPower_mW() / 1000.0;

  // Calculate load voltage
  loadVoltage_V = busVoltage_V + (shuntVoltage_mV / 1000.0);
}

// ======================================================
// FUNCTION TO CONTROL RELAY
// ======================================================
void relayControl(bool state) {
  if (state) {
    digitalWrite(RELAY_PIN, HIGH);  // Relay ON
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Relay OFF
  }
}

// ======================================================
// FUNCTION TO TOGGLE RELAY
// ======================================================
void toggleRelay() {
  digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
}

// ======================================================
// OPTIONAL FUNCTION TO PRINT VALUES VIA SERIAL PORT
// ======================================================
void printACS712(float busVoltage, float current) {
  Serial.println("----------------- ACS-712 -----------------");

  Serial.print("Bus Voltage: ");
  Serial.print(busVoltage, 2);
  Serial.print(" V");

  Serial.print(" | Current: ");
  Serial.print(current, 2);
  Serial.println(" A");

  Serial.println("----------------- ACS-712 -----------------");
}

void printINA219(float busVoltage_V,
                 float shuntVoltage_mV,
                 float loadVoltage_V,
                 float current_A,
                 float power_W) {
  Serial.println("----------------- INA-219 -----------------");

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
  Serial.print(current_A, 2);
  Serial.println(" A");

  Serial.print("Power:         ");
  Serial.print(power_W, 2);
  Serial.println(" W");

  Serial.println("----------------- INA-219 -----------------");
}

// ======================================================
// FUNCTION TO DISPLAY PV AND BATTERY LOAD PARAMETERS
// ======================================================



// void clearSpecificRow(uint8_t rowNum) {
//   // Prevent system crashes by ignoring out-of-bounds rows (16x4 only has rows 0-3)
//   if (rowNum >= 4) return;

//   lcd.setCursor(0, rowNum);       // Move cursor to the start of the chosen row
//   lcd.print("                ");  // Print exactly 16 blank spaces to erase text
//   lcd.setCursor(0, rowNum);       // Reset cursor so new text prints from the start
// }

// void displayPowerData(float pvVoltage, float pvCurrent,
//                       float btVoltage, float btCurrent) {
//   // Calculate Power
//   float pvPower = pvVoltage * pvCurrent;
//   float btPower = btVoltage * btCurrent;

//   // Clear LCD
//   lcd.clear();

//   // ---------------- ROW 1 ----------------
//   // PV-Power 00.00 W
//   lcd.setCursor(0, 0);
//   lcd.print("PV-Power ");

//   if (pvPower < 10) lcd.print("0");
//   lcd.print(pvPower, 2);
//   lcd.print("W");

//   // ---------------- ROW 2 ----------------
//   // 00.0 V || 0.00 A
//   lcd.setCursor(0, 1);

//   if (pvVoltage < 10) lcd.print("0");
//   lcd.print(pvVoltage, 2);
//   lcd.print("V, ");

//   if (pvCurrent < 10) lcd.print("0");
//   lcd.print(pvCurrent, 2);
//   lcd.print("mA");

//   // ---------------- ROW 3 ----------------
//   // BT-Load 00.00 W
//   clearSpecificRow(2);
//   lcd.setCursor(-4, 2);
//   lcd.print("BT-Load  ");

//   // if (chargeState) {
//   //   lcd.print("CHRGNG");
//   // } else {
//   //   if (btPower < 10) lcd.print("0");
//   //   lcd.print(btPower, 2);
//   //   lcd.print("W");
//   // }

//   if (btPower < 10) lcd.print("0");
//   lcd.print(btPower, 2);
//   lcd.print("W");

//   // ---------------- ROW 4 ----------------
//   // 00.0 V || 0.00 A
//   clearSpecificRow(3);
//   lcd.setCursor(-4, 3);

//   if (btVoltage < 10) lcd.print("0");
//   lcd.print(btVoltage, 2);
//   lcd.print("V,  ");

//   if (btCurrent < 10) lcd.print("0");
//   lcd.print(btCurrent, 2);
//   lcd.print("A");
// }

void displayPowerData(float pvVoltage, float pvCurrent,
                      float btVoltage, float btCurrent) {
  // Calculate Power
  float pvPower = pvVoltage * pvCurrent;
  float btPower = btVoltage * btCurrent;

  // REMOVED lcd.clear() to stop screen flickering.
  // Instead, we overwrite or clear specific rows.
  char buffer[17];  // Buffer to hold exactly 16 characters + null terminator

  // ---------------- ROW 1 ----------------
  // Target format: "PV-Power 000.00W" (Total 16 chars)
  // Use dtostrf to force the float into exactly 6 characters total (e.g., " 25.40" or "105.20")
  char pvPowerStr[7];
  dtostrf(pvPower, 6, 2, pvPowerStr);
  sprintf(buffer, "PV-Power %sW", pvPowerStr);
  lcd.setCursor(0, 0);
  lcd.print(buffer);

  // ---------------- ROW 2 ----------------
  // Target format: "000.00V,000.00mA" (Total 16 chars)
  char pvVoltStr[7];
  char pvCurrStr[7];
  dtostrf(pvVoltage, 6, 2, pvVoltStr);  // e.g. "12.40"
  dtostrf(pvCurrent, 6, 2, pvCurrStr);  // e.g. " 1.50"
  sprintf(buffer, "%sV,%smA", pvVoltStr, pvCurrStr);
  lcd.setCursor(0, 1);
  lcd.print(buffer);

  // ---------------- ROW 3 ----------------
  // Target format: "BT-Load  000.00W" (Total 16 chars)
  char btPowerStr[7];
  dtostrf(btPower, 6, 2, btPowerStr);
  sprintf(buffer, "BT-Load  %sW", btPowerStr);
  lcd.setCursor(-4, 2);
  lcd.print(buffer);

  // ---------------- ROW 4 ----------------
  // Target format: "000.00V, 000.00A" (Total 16 chars)
  char btVoltStr[7];
  char btCurrStr[7];
  dtostrf(btVoltage, 6, 2, btVoltStr);
  dtostrf(btCurrent, 6, 2, btCurrStr);
  sprintf(buffer, "%sV, %sA", btVoltStr, btCurrStr);
  lcd.setCursor(-4, 3);
  lcd.print(buffer);
}


// Variables INA219
float busVoltage;
float shuntVoltage;
float loadVoltage;
float current_INA;
float power;

// variables ACS712
float voltage;
float current_ACS;

// variable Battery Voltage
float batteryVoltage = 0;

// ======================================================
// SETUP
// ======================================================
void setup() {
  Serial.begin(9600);

  // Relay Setup
  pinMode(RELAY_PIN, OUTPUT);

  // Initial State OFF
  digitalWrite(RELAY_PIN, LOW);

  lcd.init();
  lcd.backlight();

  pinMode(ACS712_PIN, INPUT);           // A0
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);  // A2

  Serial.println();
  Serial.println("ACS712 SENSOR INITIALIZED");

  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");

    while (1) {
      delay(10);
    }
  }

  Serial.println("INA219 Sensor Initialized");
  Serial.println("SYSTEM INITIALIZED");

  delay(1000);
}

// ======================================================
// LOOP
// ======================================================
void loop() {

  // Function Call
  readINA219(busVoltage,
             shuntVoltage,
             loadVoltage,
             current_INA,
             power);

  readACS712(voltage, current_ACS);
  readBatteryVoltage(batteryVoltage);


  // Print Function
  printINA219(busVoltage,
              shuntVoltage,
              loadVoltage,
              current_INA,
              power);

  printACS712(batteryVoltage, current_ACS);

  displayPowerData(loadVoltage, current_INA, batteryVoltage, current_ACS);

  delay(500);
}
