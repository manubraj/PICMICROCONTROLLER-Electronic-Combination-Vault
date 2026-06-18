# PICMICROCONTROLLER-Electronic-Combination-Vault
A secure digital vault door simulation utilizing password sequence verification on a PIC18F4480 microcontroller built via MPLAB X and Proteus."
# PIC18F4580-Electronic-Combination-Vault
A secure digital vault door simulation utilizing password sequence verification on a PIC18F4480 microcontroller built via MPLAB X and Proteus."
# Electronic Combination Vault

## 💡 Overview

This project demonstrates a secure electronic vault locking system using the **PIC18F4580 microcontroller**. The user enters a passcode using push buttons, and the system verifies the entered sequence against a stored password.

For security, the LCD displays asterisks (`*`) instead of the actual digits. If the password is correct, the vault unlocks by activating a DC motor through an L293D motor driver and turns on the green access LED. If the password is incorrect, red warning LEDs flash while access remains denied.

---

## 🛠️ Hardware Requirements

* PIC18F4580 Microcontroller
* 16×2 LCD Display
* L293D Motor Driver IC
* DC Motor
* Push Buttons (4)
* LEDs (6)
* 10 kΩ Pull-up Resistors
* Crystal Oscillator
* Capacitors
* 5V Regulated Power Supply
* Connecting Wires
* Breadboard / PCB

---

## 🔌 Connections

### Push Buttons

| Pin | Component | Function        |
| --- | --------- | --------------- |
| RB0 | Button 1  | Enter Digit '1' |
| RB1 | Button 2  | Enter Digit '2' |
| RB2 | Button 3  | Clear / Cancel  |
| RB3 | Button 4  | Submit Password |

### LCD Connections

| LCD Pin | PIC18F4580 Pin |
| ------- | -------------- |
| RS      | RA0            |
| EN      | RA1            |
| D0      | RC0            |
| D1      | RC1            |
| D2      | RC2            |
| D3      | RC3            |
| D4      | RC4            |
| D5      | RC5            |
| D6      | RC6            |
| D7      | RC7            |

### Motor Driver Connections

| PIC Pin | L293D   |
| ------- | ------- |
| RD0     | Input 1 |
| RD1     | Input 2 |

### LED Connections

| PIC Pin | Function         |
| ------- | ---------------- |
| RD2     | Green Access LED |
| RD3-RD7 | Red Alert LEDs   |

---

## 🗺️ Circuit Diagram

### Schematic

Proteus schematic image here.
![image alt](https://github.com/Abiramisankar0425/PIC18F4580-Electronic-Combination-Vault/blob/f7d52b74125499ba089b4a99528d3b122945c707/Screenshot%202026-06-18%20103250.png)

---

## ⚙️ How it Works

### Password Entry

* Press **RB0** to enter digit **1**
* Press **RB1** to enter digit **2**
* Press **RB2** to clear the current entry
* Press **RB3** to submit the entered password

Each entered digit is displayed as an asterisk (`*`) on the LCD.

### Correct Password

Stored Password:

1212

If the entered password matches:

* LCD displays **ACCESS GRANTED**
* DC motor rotates to unlock the vault
* Green LED turns ON

### Incorrect Password

If the password does not match:

* LCD displays **ACCESS DENIED**
* Red warning LEDs flash repeatedly
* Vault remains locked
* Password entry is reset

---

## 🔐 Password Example

| Button Press | Digit |
| ------------ | ----- |
| RB0          | 1     |
| RB1          | 2     |
| RB0          | 1     |
| RB1          | 2     |

Entered Password:

1212

Result:

ACCESS GRANTED

---

## 🧑‍💻 Code

```c
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF

#include <xc.h>

void delay() {
    for (int i = 0; i < 500; i++)
        for (int j = 0; j < 350; j++);
}

void command(int cmd) {
    LATC = cmd;
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 1;
    delay();
    LATAbits.LATA1 = 0;
}

void data(int val) {
    LATC = val;
    LATAbits.LATA0 = 1;
    LATAbits.LATA1 = 1;
    delay();
    LATAbits.LATA1 = 0;
}

void lcd_print(char* s) {
    while(*s)
        data(*s++);
}

void main() {

    ADCON1 = 0x0F;

    TRISB = 0xFF;
    TRISD = 0x00;
    TRISC = 0x00;

    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;

    LATD = 0;

    command(0x38);
    command(0x0C);
    command(0x01);

    lcd_print("ENTER PASSCODE:");

    int inputCode = 0;
    int enteredDigits = 0;

    while(1) {

        if(PORTBbits.RB0 == 0) {

            inputCode = (inputCode * 10) + 1;
            enteredDigits++;

            command(0xC0);

            for(int i = 0; i < enteredDigits; i++)
                data('*');

            delay();
            delay();
        }

        else if(PORTBbits.RB1 == 0) {

            inputCode = (inputCode * 10) + 2;
            enteredDigits++;

            command(0xC0);

            for(int i = 0; i < enteredDigits; i++)
                data('*');

            delay();
            delay();
        }

        else if(PORTBbits.RB2 == 0) {

            inputCode = 0;
            enteredDigits = 0;

            command(0x01);

            lcd_print("ENTER PASSCODE:");

            LATD = 0;

            delay();
        }

        else if(PORTBbits.RB3 == 0) {

            command(0x01);

            if(inputCode == 1212) {

                lcd_print("ACCESS GRANTED");

                LATD = 0x05;

                delay();
                delay();
            }

            else {

                lcd_print("ACCESS DENIED");

                for(int i = 0; i < 5; i++) {

                    LATD = 0xFC;

                    delay();

                    LATD = 0;

                    delay();
                }

                inputCode = 0;
                enteredDigits = 0;

                command(0x01);

                lcd_print("ENTER PASSCODE:");
            }
        }
    }
}
```

---

## 📊 System Operation

### Access Granted

* LCD shows **ACCESS GRANTED**
* Motor rotates in unlock direction
* Green LED turns ON

### Access Denied

* LCD shows **ACCESS DENIED**
* Red LEDs flash as an alarm indication
* Motor remains OFF

---

## 🪜 Steps to Build & Test

### 1️⃣ Create Project

Create a new MPLAB X project for PIC18F4580.

### 2️⃣ Add Source Code

Copy the provided Embedded C code into the project.

### 3️⃣ Compile

Build the project using the XC8 compiler.

### 4️⃣ Generate HEX File

Compile successfully to obtain the HEX file.

### 5️⃣ Proteus Simulation

* Connect LCD, buttons, LEDs, and L293D according to the schematic.
* Load the generated HEX file into PIC18F4580.

### 6️⃣ Test Operation

* Enter the password using buttons.
* Verify unlocking for correct code.
* Verify alarm indication for incorrect code.

---

## 🚀 Applications

* Electronic Safe Systems
* Digital Lockers
* Smart Security Systems
* Access Control Systems
* Bank Vault Simulators
* Embedded Systems Education

---

## ✅ Advantages

* Simple password authentication
* Secure masked password entry
* Visual access indication
* Motorized locking mechanism
* Low-cost implementation
* Easy to expand for larger passwords

---

## ⚠️ Limitations

* Supports only digits 1 and 2 in current implementation
* Uses software delay instead of hardware timers
* Password stored directly in code
* No EEPROM-based password storage

---

## 🔮 Future Enhancements

* Numeric Keypad Interface
* EEPROM Password Storage
* RFID Authentication
* Fingerprint Authentication
* GSM Alert System
* Buzzer Alarm
* Multiple User Accounts
* Password Change Feature


---

## 🙏 Acknowledgments

Designed and simulated using Proteus and MPLAB XC8 with the PIC18F4580 microcontroller.

---

## 📚 Resources

* PIC18F4580 Datasheet
* MPLAB X IDE
* XC8 Compiler
* Proteus Design Suite
