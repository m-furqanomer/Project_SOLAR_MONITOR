// /*
//   ACS712 20A Current Sensor with Arduino UNO
//   Reads DC current and prints value on Serial Monitor
// */

// const int sensorPin = A0;

// // Arduino ADC parameters
// const float adcResolution = 1023.0;
// const float referenceVoltage = 5.12;

// // ACS712 10A sensitivity
// const float sensitivity = 0.100;   // 100mV per Amp

// // Zero current voltage
// float zeroCurrentVoltage = 2.5;

// // Optional auto calibration
// float caliberateACS712(const float referenceVoltage, const float adcResolution, const int sensorPin){

//   float zeroCurrent = 2.5;
//   long sum = 0;

//   for (int i = 0; i < 50; i++) {
//     sum += analogRead(sensorPin);
//     delayMicroseconds(20);
//   }

//   float avgADC = (float)sum / 50.0;

//   zeroCurrent = (avgADC * referenceVoltage) / adcResolution;

//   Serial.print("Calibrated Zero Voltage: ");
//   Serial.println(zeroCurrent, 3);
//   return zeroCurrent;
// }

// void setup() {
//   Serial.begin(9600);

//   Serial.println("ACS712 20A Current Sensor");
//   delay(2000);
//   zeroCurrentVoltage = caliberateACS712(referenceVoltage, adcResolution, sensorPin);
// }

// void loop() {

//   // Read ADC value
//   int total = 0;

//   for(int i = 0; i < 50; i++)
//   {
//       total += analogRead(sensorPin);
//       delayMicroseconds(20);
//   }

//   float adcValue = (float)total / 50.0;;

//   // Convert ADC to voltage
//   float voltage = (adcValue * referenceVoltage) / adcResolution;

//   // Calculate current
//   float current = (voltage - zeroCurrentVoltage) / sensitivity;

//   // Remove tiny noise values
//   if (current > -0.05 && current < 0.05) {
//     current = 0.0;
//   }

//   Serial.print("ADC Value: ");
//   Serial.print(adcValue);

//   Serial.print(" | Voltage: ");
//   Serial.print(voltage, 3);

//   Serial.print(" V | Current: ");
//   Serial.print(current, 3);

//   Serial.println(" A");

//   delay(1000);
// }

#define acs712 A0  // input PIN Define

double adcValue = 0;
float adcVoltage = 0;                    // Bus Voltage
float stepVoltage = 0.0048875855327468;  // 5V/1023=Steps
float sensitivity = 0.100;               //mV for 20A
float current = 0;

void setup() {
  Serial.begin(9600);  // Communiation rate
  Serial.println();
  Serial.println("| ++ # ++ # ++ |");
  Serial.println(" | Setting Up An ACS712 SENSOR | ");
  pinMode(acs712, INPUT);
}

void loop() {

  // for (int i = 0; i < 5; i++) {
  //   adcValue += analogRead(acs712);
  //   delayMicroseconds(10);
  // }
  // adcValue = adcValue / 5.0;

  adcValue = analogRead(acs712);
  // Serial.print("RAW adcValue: ");
  // Serial.println(adcValue);
  adcVoltage = (float)adcValue * stepVoltage;
  // Serial.print("RAW adcVoltage: ");
  // Serial.println(adcVoltage);
  adcVoltage -= 2.51;
  if (adcVoltage > 0.00) {
    Serial.print("Bus Voltage: ");
    Serial.print(adcVoltage, 2);
    Serial.print(" V ");
    //Serial.println(" V ");

    current = adcVoltage / sensitivity;

    Serial.print("| Current: ");
    Serial.print(current, 2);
    Serial.println(" A");

  } else {
    Serial.print("Bus Voltage: ");
    Serial.print(adcVoltage, 2);
    Serial.print(" V ");
    //Serial.println(" V ");

    current = adcVoltage / (sensitivity * -1);

    Serial.print("| Current: ");
    Serial.print(current, 2);
    Serial.println(" A");
  }


  delay(500);
}
