# 🕹️ LPC2148 Embedded RTC + LDR + LCD System

This embedded systems project for the LPC21xx microcontroller demonstrates the integration of a Real-Time Clock (RTC), Light Dependent Resistor (LDR) sensor, LCD display, keypad-based RTC editing via external interrupt, and LED brightness control based on ambient light and time.

---

## 📦 Features

- ✅ Display **current time and date** on 16x2 LCD
- ✅ Show **day of the week**
- ✅ Use **external interrupt (EINT0)** to enter RTC edit mode
- ✅ Edit RTC (Hour, Minute, Second, Date, Month, Year, Day) via **4x4 matrix keypad**
- ✅ Read LDR sensor using **ADC**
- ✅ Automatically control **LED brightness** based on time of day and light intensity

---

## 🔧 Hardware Requirements

- ✅ LPC2148 Development Board (based on LPC21xx)
- ✅ 16x2 LCD Display
- ✅ 4x4 Matrix Keypad
- ✅ LDR Sensor connected to ADC Channel 0
- ✅ 8 LEDs connected to P1.24 - P1.31
- ✅ External Interrupt button (for EINT0)
- ✅ Power Supply (5V)

---

## 📁 Project Structure

```
├── main.c              // Main control loop
├── defines.h           // Global definitions
├── delay.h             // Delay functions
├── lcd_defines.h       // LCD control macros
├── Interruptenable.h   // External interrupt setup
├── kpm.h               // Keypad scanning functions
├── LCD_INIT.h          // LCD initialization
├── RTC.h               // RTC variable definitions and setup
├── adc.h               // ADC interface
├── types.h             // Custom data types
├── README.md           // This file
```

---

## 🚀 How It Works

1. **LCD Display**: Shows the current time on Line 1 and date + day on Line 2.
2. **RTC**: Maintains real-time data using internal RTC peripheral.
3. **Interrupt**: Pressing the external interrupt button (EINT0) enters RTC edit mode.
4. **Keypad**: User can edit time, date, or exit from the menu.
5. **LDR Sensor**: Voltage from LDR is read via ADC0 and displayed on LCD.
6. **LED Control**:
    - Between 6 PM and 6 AM, LEDs light up based on darkness (LDR value).
    - More darkness = more LEDs ON.
    - During the day, LEDs remain OFF.

---

## 🔢 RTC Edit Menu (via Keypad)

```
1. Hour     (0 - 23)
2. Minute   (0 - 59)
3. Second   (0 - 59)
4. Day      (0 - 6)  [Sun = 0, Mon = 1, ..., Sat = 6]
5. Date     (1 - 31)
6. Month    (1 - 12)
7. Year     (2000 - 2099)
8. Exit
```

---

## 📸 Example LCD Output

```
12:45:30 3.20
07/10/25 Tue
```

---

## 🔌 Pin Configuration (Important)

| Peripheral | LPC2148 Pin  | Notes                 |
|------------|--------------|-----------------------|
| LCD        | GPIO         | 8-bit data + control  |
| Keypad     | GPIO         | 4x4 matrix            |
| LDR        | P0.28 (AD0.1)| ADC input             |
| LEDs       | P1.24 - P1.31| 8 LEDs                |
| Interrupt  | EINT0        | Triggered on key press|

---

## Hardware Setup (Proteus)
<img width="1180" height="834" alt="image" src="https://github.com/user-attachments/assets/5b9068df-3f85-487c-8ccd-d1d4f8b16f9e" />

____


## 🔨 To Build and Flash

> You can use **Keil uVision**, **Flash Magic**, or **LPCXpresso**.

1. Import the project into your IDE.
2. Set the target device to **LPC2148**.
3. Compile and generate HEX file.
4. Flash using **Flash Magic** via UART or any supported programmer.

___

## Output (Proteus)
<img width="1185" height="833" alt="image" src="https://github.com/user-attachments/assets/87b92457-e2a1-47a6-b20c-b21411a98fe5" />
<img width="1179" height="831" alt="image" src="https://github.com/user-attachments/assets/225d7db1-9818-4581-b3ac-1d2f1f8ad4e8" />
<img width="1183" height="834" alt="image" src="https://github.com/user-attachments/assets/cbb7f341-f21f-4acb-aa36-c5c3d19faff9" />

## 📚 References

- LPC2148 Datasheet
- Embedded C Programming
- ARM7 Based Microcontroller Tutorials
