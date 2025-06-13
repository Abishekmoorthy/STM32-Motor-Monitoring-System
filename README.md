# 🔧 Motor Failure Prediction using STM32F401

This project is a smart embedded system designed to predict motor failure by monitoring **temperature** and **shaft position** (using ultrasonic sensors), and then providing **visual alerts** and **Bluetooth-based updates**.

When the ultrasonic sensor detects a **close-range object**, it is treated as a **shaft misalignment or wear** — triggering a **motor maintenance alert**.

---

## 📌 Project Overview

Our system is built using the **STM32F401** microcontroller and integrates the following components:

- ✅ Two **Ultrasonic Sensors** for shaft (proximity) monitoring  
- 🌡️ One **Temperature Sensor**  
- 🚦 **LED Indicators** (Red, Yellow, Green) to signal motor health status  
- 🌬️ A **Fan** that turns on at high temperature  
- 📲 **Bluetooth Communication** to send temperature data over UART  
- ⚡ **Interrupts** for real-time event handling  

---

## 🛠️ Key Functionalities

### 🔍 Sensor Monitoring

- **Distance Measurement** via ultrasonic sensors simulating shaft position
- **Temperature Reading** through ADC
- **Interrupt-driven sensor event handling**

### 💡 Alerts & Responses

- **LED Indicators** for shaft/motor condition:
  - 🔴 **Red** – Close shaft reading → *Maintenance Needed*  
  - 🟡 **Yellow** – Moderate shaft clearance  
  - 🟢 **Green** – Shaft running normally  

- **Fan Activation** at high temperature
- **Bluetooth Data Transmission** of live temperature values

---

## ⚙️ System Components

| Component                | Function                           |
|--------------------------|------------------------------------|
| **STM32F401**            | Main embedded controller           |
| **Ultrasonic Sensors**   | Shaft position / proximity monitor |
| **Temperature Sensor**   | Detects overheating conditions     |
| **LEDs (R, Y, G)**       | Indicate motor shaft status        |
| **Fan**                  | Activates during overheating       |
| **UART + Bluetooth**     | Wireless temperature reporting     |

---

## 🧠 How It Works

1. The system initializes all hardware configurations
2. In a continuous loop:
   - Measures distance (shaft position) via ultrasonic sensors  
   - Reads motor temperature  
   - Updates LEDs based on shaft condition  
   - Turns on fan if temperature exceeds threshold  
   - Sends temperature data over Bluetooth  

---

## 📡 Pin Configuration Highlights

| Pin(s)        | Purpose                       |
|---------------|-------------------------------|
| `PA0`, `PA2`  | Ultrasonic Trigger Pins       |
| `PA1`, `PA3`  | Ultrasonic Echo Pins          |
| `PB12–PB14`   | Sensor 1 LED Indicators       |
| `PA8`, `PA11–PA12` | Sensor 2 LED Indicators  |
| `PB9`         | Temperature Sensor (ADC Input)|
| `PB8`         | Fan Output                    |
| `PA9`, `PA10` | UART1 TX/RX (Bluetooth)       |

---

## 🧾 Files Included

- `motor-failure.c` – Embedded system source code  
- `MOTOR FAILURE PREDICTION.docx` – Project documentation and pin details  

---

## 📦 Getting Started

To run this project on your STM32 board:

1. Flash the `motor-failure.c` using STM32CubeIDE (or compatible toolchain)  
2. Connect ultrasonic sensors, temperature sensor, fan, and Bluetooth module properly  
3. Open a serial/Bluetooth terminal to receive live temperature updates  

---

## 📈 Sample Bluetooth Output
  temperature 28
  temperature 30
  temperature 35

---

## 🧾 Conclusion

This project demonstrates how **embedded systems** and **sensor integration** can be applied to **predictive motor maintenance**. By monitoring **shaft behavior** and **thermal health**, timely maintenance actions can be taken to prevent failure, particularly useful in **industrial environments** or **mechanical systems**.

