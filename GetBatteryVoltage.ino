// // Pin definition
// const int ANALOG_INPUT_PIN = A0;

// // Calibration Multiplier based on your real-world data
// const float CALIBRATION_FACTOR = 0.02047;

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   // Call the function to get the current voltage
//   float currentVoltage = readInputVoltage(ANALOG_INPUT_PIN);
  
//   // Print the result to the Serial Monitor
//   Serial.print("Measured Input Voltage: ");
//   Serial.print(currentVoltage, 2); 
//   Serial.println(" V");
  
//   delay(1000); // Wait 1 second between readings
// }

// /**
//  * Reads an analog pin and calculates the real-world input voltage 
//  * based on the 14.8K / 4.7K resistor divider calibration data.
//  * 
//  * @param pin The analog input pin to read (e.g., A0)
//  * @return The calculated true input voltage as a float
//  */
// float readInputVoltage(int pin) {
//   int rawADC = analogRead(pin);
//   float voltage = rawADC * CALIBRATION_FACTOR;
//   return voltage;
// }


const int ANALOG_INPUT_PIN = A0;

// Hardcoded physical hardware ratio based on your true 14V input test:
// At 14V true input, you measured exactly 3.32V at the pin.
// Divider Ratio = Vout / Vin = 3.32 / 14.0 = 0.23714
// Multiplier to flip it back = 1 / 0.23714 = 4.2169
const float HARDWARE_DIVIDER_MULTIPLIER = 4.2169;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // 1. Get the current, exact supply voltage powering the Arduino chip
  float trueArduinoVCC = getArduinoVCC();
  
  // 2. Read the raw analog pin value (0 to 1023)
  int rawADC = analogRead(ANALOG_INPUT_PIN);
  
  // 3. Convert raw value to the true pin voltage using the live VCC reference
  float physicalPinVoltage = (rawADC * trueArduinoVCC) / 1023.0;
  
  // 4. Calculate the original supply voltage using your hardware multiplier
  float realWorldInputVoltage = physicalPinVoltage * HARDWARE_DIVIDER_MULTIPLIER;
  
  // Print everything out clearly so you can diagnose the fix
  Serial.print("Live Arduino VCC: ");
  Serial.print(trueArduinoVCC, 2);
  Serial.print("V | Pin Voltage: ");
  Serial.print(physicalPinVoltage, 2);
  Serial.print("V | Calculated Supply: ");
  Serial.print(realWorldInputVoltage, 2);
  Serial.println(" V");
  
  delay(1000); 
}

/**
 * Secretly measures the Arduino's own 5V/VCC rail against its internal, 
 * stable 1.1V reference bandgap. This prevents USB voltage sag errors.
 */
float getArduinoVCC() {
  #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    // Set the reference to Vcc and the conversion source to the internal 1.1V bandgap
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U4RUM__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    ADCSRB = 0; // Clear MUX5
  #else
    return 5.00; // Fallback default for non-AVR boards
  #endif
  
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // Measuring...

  uint8_t low  = ADCL; // Changed from uint256_t to uint8_t
  uint8_t high = ADCH; // Changed from uint256_t to uint8_t
  long result = (high << 8) | low;

  // Calculate Vcc in millivolts: 1.1V * 1023 * 1000 = 1125300
  long vcc_mv = 1125300L / result; 
  return vcc_mv / 1000.0; // Return as a standard float voltage (e.g., 4.68)
}
