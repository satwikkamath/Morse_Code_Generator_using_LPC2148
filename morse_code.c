#include <lpc213x.h>
#include <string.h>

#define SLAVE_ADDR 0x70 // constants
#define MAX 12
#define AA 2
#define SI 3
#define STO 4
#define STA 5
#define I2EN 6

unsigned char write[] = {0xf7, 0xfd, 0xfb, 0xfe};                                                                              // array to write in row of keypad
unsigned char comb[] = {0x77, 0xb7, 0xd7, 0xe7, 0x7b, 0xbb, 0xdb, 0xeb, 0x7d, 0xbd, 0xdd, 0xed, 0x7e, 0xbe, 0xde, 0xee};       // combination of row and column
char num[] = "0000032106540987";                                                                                               // number corresponding to combination
unsigned char seg_values[] = {0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x4F, 0x5B, 0x06, 0x3F, 0x7D, 0x6D, 0x66, 0x3F, 0x6F, 0x7F, 0x07}; // 7 segment data corresponding to number

void wait(int count)
{
    while (count--)
        ;
}

const char *digitMorseCodeTable[10] = {
    "-----", ".----", "..---", "...--", "....-", ".....",
    "-....", "--...", "---..", "----."}; // morse code for 0 to 9

void I2C_Init(void) // initialization for i2c
{
    VPBDIV = 0x02;        // sets FOSC =  60MHHZ
    PINSEL0 = 0x30C00050; // set po.2,p0.3 to sda scl
    I2C1SCLH = 150;       // 50%duty,I2CFreq->100KHz,PCLK=30MHz
    I2C1SCLL = 150;
    I2C1CONSET = (1 << I2EN); // Enable I2C module
    I2C0SCLH = 150;           // 50%duty,I2CFreq->100KHz,PCLK=30MHz
    I2C0SCLL = 150;
    I2C0CONSET = (1 << I2EN);
}

int I2C1_Start() // start i2c communication
{
    I2C1CONCLR = 1 << STO;
    I2C1CONCLR = 1 << SI;
    I2C1CONSET = 1 << STA;
    I2C0CONCLR = 1 << STO;
    I2C0CONCLR = 1 << SI;
    I2C0CONSET = 1 << STA;
    return 0;
}

void delay_ms(int count)
{
    int j = 0, i = 0;
    for (j = 0; j < count; j++)
    {
        for (i = 0; i < 35; i++)
            ; // At 60Mhz, the below loop introduces delay of 10 us
    }
}

void senddata1(char data)
{
    while (!(I2C1CONSET & 0x08)) // wait till SI becomes 1
        ;
    I2C1DAT = data;
    I2C1CONCLR = 1 << SI; // clearing SI bit
    delay_ms(200);
}

void senddata0(char data)
{
    while (!(I2C0CONSET & 0x08))
        ;
    I2C0DAT = data;
    I2C0CONCLR = 1 << SI;
    delay_ms(200);
}

void sendchar1(char data)
{
    senddata1(0x50 | (data >> 4)); // sending 1st 4 bits with en =1
    delay_ms(50);
    senddata1(0x40 | (data >> 4)); // sending 1st 4 bits with en =0
    delay_ms(50);
    senddata1(0x50 | (data & 0x0f)); // sending next 4 bits with en =1
    delay_ms(50);
    senddata1(0x40 | (data & 0x0f)); // sending next 4 bits with en =0
    delay_ms(50);
    delay_ms(50);
}

void sendchar0(char data)
{
    senddata0(0x50 | (data >> 4));
    delay_ms(50);
    senddata0(0x40 | (data >> 4));
    delay_ms(50);
    senddata0(0x50 | (data & 0x0f));
    delay_ms(50);
    senddata0(0x40 | (data & 0x0f));
    delay_ms(50);
    delay_ms(50);
}
void LCD_init()
{
    int i = 0;
    char code = SLAVE_ADDR;
    I2C_Init();
    I2C1_Start();
    wait(4000);

    while (!(I2C1CONSET & 0x08)) // wait till SI becomes 1
    {
    };
    IO1SET = (1 << 21);
    I2C1CONCLR = 1 << STO;
    I2C1CONCLR = 1 << STA;
    I2C1CONSET = 1 << AA;
    I2C1DAT = code;
    I2C1CONCLR = 1 << SI;

    while (!(I2C0CONSET & 0x08))
    {
    };
    IO0SET = (1 << 21);
    I2C0CONCLR = 1 << STO;
    I2C0CONCLR = 1 << STA;
    I2C0CONSET = 1 << AA;
    I2C0DAT = code;
    I2C0CONCLR = 1 << SI;

    while (!(I2C1CONSET & 0x08))
    {
    };
    if (I2C1STAT == 0x18) // status code which indicates ready to receive
    {
        IO1SET = (1 << 23);
        I2C1CONSET = 1 << AA;
        I2C1DAT = 0x00;
        I2C1CONCLR = 1 << SI;
        while (!(I2C1CONSET & 0x08))
            ;
        for (i = 0; i < 2000; i++)
            wait(800);
        if (I2C1STAT == 0x28) // when ack received
        {
            senddata1(0x10); // function set
            senddata1(0x00);
            senddata1(0x12); // display off cursor on
            senddata1(0x02);
            senddata1(0x12); // display off cursor on
            senddata1(0x02);
            senddata1(0x18); // entry mode set
            senddata1(0x08);
            senddata1(0x10); // display on
            senddata1(0x00);
            senddata1(0x1e); // display shift left
            senddata1(0x0e);
            senddata1(0x10); // function set
            senddata1(0x00);
            senddata1(0x16); // entry mode
            senddata1(0x06);
            senddata1(0x10); // clear display
            senddata1(0x00);
            senddata1(0x11); // display on blink on
            senddata1(0x01);
            senddata1(0x18); // display shift right
            senddata1(0x08);
            senddata1(0x10); // function set
            senddata1(0x00);
        }
    }

    while (!(I2C0CONSET & 0x08))
        ;
    if (I2C0STAT == 0x18)
    {
        IO0SET = (1 << 23);
        I2C0CONSET = 1 << AA;
        I2C0DAT = 0x00;
        I2C0CONCLR = 1 << SI;
        while (!(I2C0CONSET & 0x08))
            ;
        for (i = 0; i < 2000; i++)
            wait(800);
        if (I2C0STAT == 0x28)
        {
            senddata0(0x10);
            senddata0(0x00);
            senddata0(0x12);
            senddata0(0x02);
            senddata0(0x12);
            senddata0(0x02);
            senddata0(0x18);
            senddata0(0x08);
            senddata0(0x10);
            senddata0(0x00);
            senddata0(0x1e);
            senddata0(0x0e);
            senddata0(0x10);
            senddata0(0x00);
            senddata0(0x16);
            senddata0(0x06);
            senddata0(0x10);
            senddata0(0x00);
            senddata0(0x11);
            senddata0(0x01);
            senddata0(0x18);
            senddata0(0x08);
            senddata0(0x10);
            senddata0(0x00);
        }
    }
}

void init()
{
    VPBDIV = 0x02;
    PINSEL1 = 0x0;
    PINSEL2 = 0x0;
    IODIR0 = 0xFF;
    IODIR1 = 0x000F0000; // 16,17,18,19 as output (row)
}

char GetKey()
{
    int row = 0;
    unsigned char w, w_final;
    int ind;
    int i;
    int temp;
    while (1)
    {
        IO1CLR = 0xffffffff;
        w = write[row];
        IO1SET |= (w << 16); // writing to the row (16,17,18,19). so shifting 16 bits
        delay_ms(1000);
        temp = IO1PIN;                   // reading the bits
        w_final = ((temp >> 16) & 0xFF); // right shift 16 times and ANDing with FF to remove unnecessary data
        if (w_final != w)
            break;
        row++;
        if (row >= 4)
            row = 0;
    }

    for (i = 0; i < 16; i++)
    {
        if (comb[i] == w_final)
            ind = i;
    }
    return num[ind];
}

const char *getMorseCode(char digit)
{
    switch (digit)
    {
    case '0':
        return "-----";
    case '1':
        return ".----";
    case '2':
        return "..---";
    case '3':
        return "...--";
    case '4':
        return "....-";
    case '5':
        return ".....";
    case '6':
        return "-....";
    case '7':
        return "--...";
    case '8':
        return "---..";
    case '9':
        return "----.";
    default:
        return "Invalid input"; // In case the input is not a digit
    }
}

void delay_ms_1(int j)
{
    int x, i;
    for (i = 0; i < j; i++)
    {
        for (x = 0; x < 6000; x++)
            ;
    }
}
void led(char input)
{
    int led_pin = 0; // connection of LED to p0.0
    int dot = 100;   // ON time for dot
    int dash = 250;  // ON time for dash
    switch (input)
    {

    case '0': // -----
        IOSET0 |= 1 << led_pin;
        delay_ms_1(dash);
        IOCLR0 |= 1 << led_pin;
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '1': // .----
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '2': // ..---
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '3': // ...--
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dash);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dash);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '4': // ....-
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '5': // .....
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '6': // -....
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '7': // --...
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '8': // ---..
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '9':
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;
    }
}

int main()
{
    char x;
    int i = 0;
    int count = 100;
    const char *morseCode;
    LCD_init();
    init();

	
    while (count > 0)
    {
        x = GetKey();                // keyboard input
        sendchar0(x);                // sending to LCD 1 for display
        morseCode = getMorseCode(x); // morsecode of character input
        for (i = 0; morseCode[i] != '\0'; i++)
        {
            sendchar1(morseCode[i]); // sending to second LCD for display
        }
        sendchar1(' ');
        led(x); // switching led according to Morse code
        delay_ms(35000);
        count--;
    }
    senddata1(0x10);
    senddata1(0x0C);
    senddata0(0x10); // display off cursor off
    senddata0(0x0C);
    return 0;
}