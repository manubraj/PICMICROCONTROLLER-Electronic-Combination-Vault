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
