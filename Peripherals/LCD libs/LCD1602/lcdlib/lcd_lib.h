#ifndef LCD_LIB
#define LCD_LIB

#include <time.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdbool.h>

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// Flags for display position(DDRAM address) control
#define LCD_DISPLAY_FIRSTROW 0x00
#define LCD_DISPLAY_SECONDROW 0x40

// Flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// Flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// Flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En_BIT 0b00000100 // Enable bit
#define Rw_BIT 0b00000010 // Read/Write bit
#define Rs_BIT 0b00000001 // Register select bit

// Flags exposed to end users
#define LCD_LIGHT_ON 1
#define LCD_LIGHT_OFF 0


static __u8 INIT[] = {
    0x03, 0x02,
    LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE,
    LCD_DISPLAYCONTROL | LCD_DISPLAYON,
    LCD_CLEARDISPLAY,
    LCD_ENTRYMODESET | LCD_ENTRYLEFT,
};

typedef struct{
    bool backLight; // the flag LCD_BACKLIGHT(0x08) for on
    bool onDisplay; // control the liquid crystal display
    bool cursorDisplay; // control the display of the cursor
    bool cursorBlink; // control the blink of the cursor
    enum {
        entryShiftLeft = 0,
        entireShiftRight = 1,
        entryShiftRight = 2,
        entireShiftLeft = 3,
    }entryMode;    // control the shift of the image after reading/writing data
                    //      entryShiftLeft:     0b00
                    //      entireShiftRight:   0b01
                    //      entryShiftRight:    0b10
                    //      entireShiftLeft:    0b11
}lcd_config;


typedef struct{
    int deviceFile; // linux device file
    lcd_config *config; // config struct
}LCD;

// * static functions *
static int lcd_trigger(LCD *, __u8 dat);	
static int lcd_write_cmd(LCD *, __u8 cmd);
static int lcd_write_data(LCD *, __u8 dat);


// * public functions *
LCD *lcd_init(char i2cDevicePath[], int lcdAddr, lcd_config *config);
int lcd_read_busy_signal(LCD *);

// config
int lcd_config_backlight(LCD *);
int lcd_config_entrymode(LCD *);
int lcd_config_display_and_cursor(LCD *);
int lcd_update_config(LCD *);
lcd_config *lcd_config_init();

// display control
int lcd_clear_and_return(LCD *);
int lcd_clear_display(LCD *);
int lcd_return_home(LCD *);
int lcd_display_string(LCD *, char *string);
int lcd_display_customchar(LCD *, short charNO);
int lcd_display_string_pos(LCD *, char *string, short linePos, short columnPos);
int lcd_display_customchar_pos(LCD *, short charNO, short linePos, short columnPos);

// CGRAM & DDRAM
int lcd_cgram_writechar(LCD *, short charNO, char customChar[8]);

// clean up
int lcd_close(LCD *);

// Deprecated
// int lcd_backlight_switch(LCD *, short backLight); 

#endif
