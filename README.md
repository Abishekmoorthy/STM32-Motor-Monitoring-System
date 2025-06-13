# 🔧 Motor Failure Prediction using STM32F401

This project is a smart embedded system designed to predict motor failure by monitoring **temperature** and **proximity** using sensors, and then providing **visual alerts** and **Bluetooth-based updates**.

---

## 👨‍💻 Team Members

- CB.EN.U4CSE22001  
- CB.EN.U4CSE22011  
- CB.EN.U4CSE22015  
- CB.EN.U4CSE22036  

---

## 📌 Project Overview

Our system is built using the **STM32F401** microcontroller and integrates the following components:

- ✅ Two **Ultrasonic Sensors** for proximity detection  
- 🌡️ One **Temperature Sensor**  
- 🚦 **LED Indicators** (Red, Yellow, Green) to signal warning levels  
- 🌬️ A **Fan** that turns on at high temperature  
- 📲 **Bluetooth Communication** to send temperature data over UART  
- ⚡ **Interrupts** for real-time sensor responses  

---

## 🛠️ Key Functionalities

### 🔍 Sensor Monitoring

- **Distance Measurement** using ultrasonic sensors  
- **Temperature Reading** via analog-to-digital conversion (ADC)  
- **Real-time Interrupts** for accurate sensor response  

### 💡 Alerts & Responses

- **LED Alerts:**
  - 🔴 Red – Close range (Warning)  
  - 🟡 Yellow – Moderate range  
  - 🟢 Green – Safe distance  
- **Fan Activation** when temperature crosses the threshold  
- **Bluetooth Transmission** of temperature to paired devices  

---

## ⚙️ System Components

| Component            | Function                      |
|----------------------|-------------------------------|
| STM32F401            | Main controller               |
| Ultrasonic Sensors   | Distance monitoring           |
| Temperature Sensor   | Detects overheating           |
| LEDs (Red, Yellow, Green) | Visual status indicator |
| Fan                  | Cooling system                |
| UART + Bluetooth     | Wireless data communication   |

---

## 🧠 How It Works

1. System starts and initializes all configurations  
2. Then continuously:
   - Measures distance from two sensors  
   - Checks temperature using ADC  
   - Updates LED indicators based on distance  
   - Turns on fan if temperature is high  
   - Sends temperature via Bluetooth  

---

## 📡 Pin Configuration Highlights

- `PA0`, `PA2` – Ultrasonic Triggers  
- `PA1`, `PA3` – Ultrasonic Echoes  
- `PB12–14` – LEDs for Sensor 1  
- `PA8`, `PA11–12` – LEDs for Sensor 2  
- `PB9` – Temperature Sensor (ADC Input)  
- `PB8` – Fan Output  
- `PA9`, `PA10` – USART1 TX/RX for Bluetooth  

---

## 🧾 Files Included

- `motor-failure.c`: Source code for the STM32 embedded system  
- `MOTOR FAILURE PREDICTION.docx`: Detailed documentation and pin reference  

---

## 📦 Getting Started

To run this on your STM32 board:

1. Flash the `motor-failure.c` code using STM32CubeIDE or any compatible tool  
2. Power up the circuit with proper sensor and Bluetooth module connections  
3. Use a serial terminal or Bluetooth app to view temperature updates  

---

## 📈 Output Example (via Bluetooth)

