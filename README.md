# Solar Monitor System using INA219, ACS712, LCD Display & Relay

A real-time solar power monitoring system built with Arduino that measures photovoltaic (PV) panel performance, battery voltage, load current, and power consumption. The project combines the **INA219** and **ACS712** sensors with a **16x4 I2C LCD** for live monitoring and includes relay control functionality for future load management and battery protection applications.

---

## 📌 Features

* Real-time PV voltage measurement using INA219
* Real-time PV current measurement using INA219
* Real-time PV power calculation
* Battery voltage monitoring using analog voltage divider
* Load current monitoring using ACS712 (20A version)
* Live power calculations for connected load
* 16x4 I2C LCD display interface
* Serial monitor debugging output
* Relay control and relay toggle functions
* Modular code structure using reusable functions
* Designed for solar energy monitoring applications

---

## 🛠 Hardware Components

| Component                       | Purpose                         |
| ------------------------------- | ------------------------------- |
| Arduino Uno/Nano                | Main Controller                 |
| INA219 Current & Voltage Sensor | Solar panel monitoring          |
| ACS712 20A Current Sensor       | Battery/load current monitoring |
| 16x4 I2C LCD Display            | Data visualization              |
| Relay Module                    | Load control                    |
| Voltage Divider Circuit         | Battery voltage sensing         |
| Solar Panel                     | Power source                    |
| Battery                         | Energy storage                  |

---

## 📊 System Architecture

```text
           Solar Panel
                │
                ▼
          ┌──────────┐
          │  INA219  │
          └──────────┘
                │
                ▼
            Battery
                │
      ┌─────────┴─────────┐
      ▼                   ▼
Voltage Divider      ACS712 Sensor
      │                   │
      ▼                   ▼
   Arduino Uno/Nano
           │
    ┌──────┼──────┐
    ▼             ▼
 LCD Display    Relay
```

---

## 🔌 Pin Connections

### INA219

| INA219 Pin | Arduino |
| ---------- | ------- |
| VCC        | 5V      |
| GND        | GND     |
| SDA        | A4      |
| SCL        | A5      |

---

### LCD (I2C)

| LCD Pin | Arduino |
| ------- | ------- |
| VCC     | 5V      |
| GND     | GND     |
| SDA     | A4      |
| SCL     | A5      |

**Note:** Both the INA219 and I2C LCD share the same I2C bus.

---

### ACS712

| ACS712 Pin | Arduino |
| ---------- | ------- |
| OUT        | A0      |
| VCC        | 5V      |
| GND        | GND     |

---

### Battery Voltage Divider

| Divider Output | Arduino |
| -------------- | ------- |
| Output         | A2      |

---

### Relay Module

| Relay Pin | Arduino |
| --------- | ------- |
| IN        | D7      |
| VCC       | 5V      |
| GND       | GND     |

---

## 📈 Measured Parameters

### Solar Panel (INA219)

* Bus Voltage (V)
* Shunt Voltage (mV)
* Load Voltage (V)
* Current (A)
* Power (W)

### Battery / Load Side

* Battery Voltage (V)
* Load Current (A)
* Load Power (W)

---

## 🖥 LCD Display Layout

```text
PV-Power  XX.XXW
XX.XXV,XX.XXmA

BT-Load   XX.XXW
XX.XXV, XX.XXA
```

### Display Information

**Row 1**

* Solar panel power output

**Row 2**

* Solar panel voltage and current

**Row 3**

* Battery/load power

**Row 4**

* Battery voltage and load current

---

## ⚙ Sensor Calibration

### ACS712 (20A Version)

Current calculation:

```cpp
Current = (SensorVoltage - OffsetVoltage) / Sensitivity
```

Current settings used:

```cpp
float sensitivity = 0.100;
float offsetVoltage = 2.51;
```

### Battery Voltage Calibration

Calibration factor:

```cpp
const float CALIBRATION_FACTOR = 0.02047;
```

Derived from measured ADC values and actual battery voltages.

---

## 📂 Project Structure

```text
SolarMonitor/
│
├── SolarMoniter_INA219_ACS712_Display_Relay.ino
│
├── README.md
│
└── Libraries
    ├── Adafruit_INA219
    ├── Wire
    └── LiquidCrystal_I2C
```

---

## 📚 Required Libraries

Install the following libraries through the Arduino Library Manager:

### Adafruit INA219

```cpp
#include <Adafruit_INA219.h>
```

### LiquidCrystal I2C

```cpp
#include <LiquidCrystal_I2C.h>
```

### Wire

```cpp
#include <Wire.h>
```

---

## 🚀 Getting Started

1. Clone the repository:

```bash
git clone https://github.com/yourusername/SolarMonitor.git
```

2. Open the `.ino` file in Arduino IDE.

3. Install required libraries.

4. Connect the hardware according to the wiring diagram.

5. Upload the code to your Arduino.

6. Open the Serial Monitor at:

```text
9600 baud
```

7. Observe live measurements on the LCD display.

---

## 🔧 Future Improvements

* Battery State of Charge (SoC) estimation
* Data logging to SD card
* Wi-Fi monitoring using ESP32
* MQTT integration
* Mobile dashboard
* Automatic battery protection using relay
* MPPT performance monitoring
* Historical energy generation statistics
* Cloud monitoring dashboard

---

## 🎯 Applications

* Solar energy monitoring systems
* Battery charging stations
* Renewable energy projects
* Educational embedded systems projects
* Off-grid solar installations
* IoT-based power monitoring systems

---

## 📄 License

This project is open-source and available under the MIT License.

---

## 👨‍💻 Author

**Muhammad Furqan Omer**
Computer Engineering Graduate
Embedded Systems | Renewable Energy Monitoring | IoT Development

If you find this project useful, consider giving it a ⭐ on GitHub.
