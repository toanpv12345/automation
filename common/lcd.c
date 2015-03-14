
#include "lcd.h"

#define	LCD_RS      RC4
#define	LCD_RW      RC5
#define LCD_EN      RC6

#define TRIS_RS     TRISC4
#define TRIS_RW     TRISC5
#define TRIS_EN     TRISC6

#define LCD_DATA    PORTB
#define TRIS_DATA   TRISB

#define IR  0
#define DR  1
#define READ    1
#define WRITE   0

uint8_t reserve_bits(uint8_t byte)
{
    uint8_t c = 0;
    uint8_t i = 0;

    for(i = 0; i < 8; i++)
    {
        if(i < 4)
            c |= ((byte & (0x01 << i)) << (7 - (i << 1)));
        else
            c |= ((byte & (0x01 << i)) >> ((i << 1) - 7));
    }
    return c;
}
/* write a byte to the LCD in 4 bit mode */
void lcd_strobe()
{
    LCD_EN = 1;
    delay_us(200);
    LCD_EN = 0;
    delay_us(200);
}

void lcd_write_cmd(unsigned char cmd)
{
    uint8_t c = reserve_bits(cmd);

    LCD_RS = IR;
#if (LCD_MODE == LCD_MODE_8_BIT)
    LCD_DATA = c;
    lcd_strobe();
#else
    LCD_DATA = ((c >> 4) & 0x0F);
    lcd_strobe();
    LCD_DATA = (c & 0x0F);
    lcd_strobe();

#endif
}

void lcd_write_data(unsigned char data)
{
    uint8_t c = reserve_bits(data);
    LCD_RS = DR;
#if (LCD_MODE == LCD_MODE_8_BIT)
    LCD_DATA = c;
    lcd_strobe();
#else
    LCD_DATA = ((c >> 4) & 0x0F);
    lcd_strobe();
    LCD_DATA = (c & 0x0F);
    lcd_strobe();
#endif
}

/*
 * 	Clear and home the LCD
 */

void lcd_clear(void)
{
    lcd_write_cmd(0x01);
    delay_ms(5);
}

/* write a string of chars to the LCD */

void lcd_puts(const char * s)
{
    while (*s)
        lcd_write_data(*s++);
}

/* write one character to the LCD */

void lcd_putch(char c)
{
    lcd_write_data(c);
}

/*
 * Go to the specified position
 */

void lcd_gotoxy(uint8_t x, uint8_t y)
{
    if (y == 0) // Line 1
    {
        lcd_write_cmd(0x80 + x);
    }
    else // Line 2
    {
        lcd_write_cmd(0xC0 + x);
    }
}

/* initialise the LCD - put into 4 bit mode */
void lcd_init()
{
    TRIS_RS = 0;
    TRIS_RW = 0;
    TRIS_EN = 0;
    TRIS_DATA = 0;

    LCD_RS = 0;
    LCD_EN = 0;
    LCD_RW = 0;
    LCD_DATA = 0;

    //    lcd_write(0x28); // Set interface length
    //    lcd_write(0xF); // Display On, Cursor On, Cursor Blink
    //    lcd_clear(); // Clear screen
    //    lcd_write(0x6); // Set entry Mode

    delay_ms(20);
    lcd_write_cmd(0x30);

    delay_ms(6);
    lcd_write_cmd(0x30);

    delay_us(300);
    lcd_write_cmd(0x30);

    delay_ms(2);
    /////////////////////////////////////////////////////

    lcd_write_cmd(0x38); //function set
    lcd_write_cmd(0x0c); //display on,cursor off,blink off
    lcd_clear(); //clear display
    lcd_write_cmd(0x06); //entry mode, set increment
}
