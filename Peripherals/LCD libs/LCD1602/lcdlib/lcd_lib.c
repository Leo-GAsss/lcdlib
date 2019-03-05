#include "lcd_lib.h"

// * static functions *
static int lcd_ddram_setaddr(LCD *thisLCD, short linePos, short columnPos)
{
    __u8 ddramAddr;
    if(linePos == 1)
        ddramAddr = LCD_DISPLAY_FIRSTROW + (__u8)(columnPos - 1);
    else if(linePos == 2)
        ddramAddr = LCD_DISPLAY_SECONDROW + (__u8)(columnPos - 1);
    
    lcd_write_cmd(thisLCD, LCD_SETDDRAMADDR | ddramAddr);
}

static int lcd_trigger(LCD *thisLCD, __u8 dat)
{
    i2c_smbus_write_byte(thisLCD->deviceFile, dat);
    usleep(200);
    i2c_smbus_write_byte(thisLCD->deviceFile, dat | En_BIT);
    usleep(600);
    i2c_smbus_write_byte(thisLCD->deviceFile, dat);
    usleep(200);
}
        
static int lcd_write_cmd(LCD *thisLCD, __u8 cmd)
{
    __u8 backLightCmd = thisLCD->config->backLight ? LCD_BACKLIGHT : LCD_NOBACKLIGHT;
    lcd_trigger(thisLCD, (cmd & 0xF0) | backLightCmd);
    lcd_trigger(thisLCD, ((cmd << 4) & 0xF0) | backLightCmd);
}

static int lcd_write_data(LCD *thisLCD, __u8 dat)
{
    __u8 backLightCmd = thisLCD->config->backLight ? LCD_BACKLIGHT : LCD_NOBACKLIGHT;
    lcd_trigger(thisLCD, (dat & 0xF0) | backLightCmd | Rs_BIT);
    lcd_trigger(thisLCD, ((dat << 4) & 0xF0) | backLightCmd | Rs_BIT);
}
// * static functions end *


// * public functions *
LCD *lcd_init(char i2cDevicePath[], int lcdAddr, lcd_config *config)
{
    LCD *thisLCD = malloc(sizeof(LCD));
    thisLCD->deviceFile = open(i2cDevicePath, O_RDWR);
    thisLCD->config = config;
    
    if(thisLCD->deviceFile < 0)
    {
        return NULL;
    }

    if(ioctl(thisLCD->deviceFile, I2C_SLAVE, lcdAddr) < 0)
    {
        return NULL;
    }
    
    // writing commands for initialization
    for(int i = 0; i < sizeof(INIT)/sizeof(INIT[0]); i++)
        lcd_write_cmd(thisLCD, INIT[i]);
    // updating configs
    lcd_update_config(thisLCD);

    return thisLCD;
}

int lcd_config_entrymode(LCD *thisLCD)
{
    __u8 entryModeCmd = LCD_ENTRYMODESET | thisLCD->config->entryMode;
    lcd_write_cmd(thisLCD, entryModeCmd);
}

int lcd_config_backlight(LCD *thisLCD)
{
    __u8 backLightCmd = thisLCD->config->backLight ? LCD_BACKLIGHT : LCD_NOBACKLIGHT;
    i2c_smbus_write_byte(thisLCD->deviceFile, backLightCmd);
}

int lcd_config_display_and_cursor(LCD *thisLCD)
{
    __u8 displayCursorCmd = thisLCD->config->onDisplay;
    displayCursorCmd *= 2;
    displayCursorCmd += thisLCD->config->cursorDisplay;
    displayCursorCmd *= 2;
    displayCursorCmd += thisLCD->config->cursorBlink;
    // the lowest three bits : D C B
    //  D: Display ON/OFF
    //  C: Cursor ON/OFF
    //  B: Cursor blink ON/OFF

    lcd_write_cmd(thisLCD, displayCursorCmd | LCD_DISPLAYCONTROL);

}

int lcd_update_config(LCD *thisLCD)
{
    lcd_config_backlight(thisLCD);
    lcd_config_entrymode(thisLCD);
    lcd_config_display_and_cursor(thisLCD);
}

lcd_config *lcd_config_init()
{
    lcd_config *thisConfig = malloc(sizeof(lcd_config));
    
    thisConfig->backLight = true;
    thisConfig->onDisplay = true;
    thisConfig->cursorDisplay = false;
    thisConfig->cursorBlink = false;
    thisConfig->entryMode = entryShiftRight;

    return thisConfig;
}

int lcd_display_string(LCD *thisLCD, char *string)
{
    lcd_ddram_setaddr(thisLCD, 1, 1);
    
    int i=0;
    while(string[i]!='\0')
    {
        lcd_write_data(thisLCD, (__u8)string[i]);
        i++;
    }
}

int lcd_display_string_pos(LCD *thisLCD, char *string, short linePos, short columnPos)
{
    lcd_ddram_setaddr(thisLCD, linePos, columnPos);
    
    int i=0;
    while(string[i]!='\0')
    {
        lcd_write_data(thisLCD, (__u8)string[i]);
        i++;
    }
}

int lcd_clear_display(LCD *thisLCD)
{
    lcd_write_cmd(thisLCD, LCD_CLEARDISPLAY);
}

int lcd_return_home(LCD *thisLCD)
{
    lcd_write_cmd(thisLCD, LCD_RETURNHOME);
}

int lcd_clear_and_return(LCD *thisLCD)
{
    lcd_write_cmd(thisLCD, LCD_CLEARDISPLAY);
    lcd_write_cmd(thisLCD, LCD_RETURNHOME);
}

int lcd_cgram_writechar(LCD *thisLCD, short charNO, char customChar[8])
{
    __u8 cgramAddr = (__u8)(charNO - 1);
    lcd_write_cmd(thisLCD, LCD_SETCGRAMADDR | cgramAddr);
    for(int i = 0; i < 8; i++)
        lcd_write_data(thisLCD, (__u8)customChar[i]);
}

int lcd_display_customchar(LCD *thisLCD, short charNO)
{
    lcd_ddram_setaddr(thisLCD, 1, 1);
    
    __u8 cgramAddr = (__u8)(charNO - 1);
    lcd_write_data(thisLCD, cgramAddr);
}

int lcd_display_customchar_pos(LCD *thisLCD, short charNO, short linePos, short columnPos)
{
    lcd_ddram_setaddr(thisLCD, linePos, columnPos);
    
    __u8 cgramAddr = (__u8)(charNO - 1);
    lcd_write_data(thisLCD, cgramAddr);
}

int lcd_close(LCD *thisLCD)
{
    close(thisLCD->deviceFile);
}
