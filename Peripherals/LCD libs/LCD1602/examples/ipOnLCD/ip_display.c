#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcd_lib.h"

int main(int argc, char *argv[])
{
    FILE *fp;
    char path[1035];
    LCD * aLCD=lcd_init("/dev/i2c-1", 0x27, 1);
    printf("LCD_init Fin.\n");
    lcd_display_string(aLCD, "LCD_init Fin.");
    sleep(3);
    
    // Open the command for reading
    // Please use absolute value
    fp = popen("/bin/bash /home/pi/startUP/ipOnLCD/ip_output.sh", "r");
    if (fp == NULL) {
        lcd_display_string(aLCD, "Failed to run command.\n");
        exit(1);
    }

    // Read the output of ip.sh
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        path[strlen(path)-2]='\0';
        printf("IP:%s\nLength:%d\n", path, strlen(path));
        lcd_clear_display(aLCD);
        lcd_display_string(aLCD, path);
    }

    /* close */
    pclose(fp);
    lcd_close(aLCD);
    return 0;
}