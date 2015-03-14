
#include "board.h"
#ifndef LCD_H
#define	LCD_H

#define LCD_MODE_4_BIT      1
#define LCD_MODE_8_BIT      2
#define LCD_MODE            LCD_MODE_8_BIT

#ifdef	__cplusplus
extern "C" {
#endif

void lcd_write_data(uint8_t data);
void lcd_write_cmd(uint8_t cmd);
void lcd_clear(void);
void lcd_puts(const char * s);
void lcd_gotoxy(uint8_t x, uint8_t y);
void lcd_init();
void lcd_putch(char c);
void lcd_strobe();

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */
