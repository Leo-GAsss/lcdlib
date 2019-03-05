#include <stdio.h>
#include <stdlib.h>

#include "lcd_lib.h"

// Fill Array anime with all zero
void initData(char anime[8])
{
    for (int i=0; i<8; i++)
        anime[i] = 0b00000000;
    return ;
}

int main()
{
    LCD * aLCD=lcd_init("/dev/i2c-1", 0x27, LCD_LIGHT_ON); // Replace "0x27" with your IIC device address here
    
    unsigned char anime[8]={0};
    
    printf("Started!\n");
    for (int row=1; row<=2; row++)
    {
        initData(anime);
        printf("Row %d\n", row);
        
        for (int i=1; i<=8; i++)
        {
            initData(anime);
            
            for (int column=1; column<=16; column++)
            {
                if (i%2 == 0)
                    column = 17-column;
                for (int k=1; k<=5; k++)
                {
                    if (i%2 == 1)
                        anime[i-1] = 0x01 << (5-k);
                    else
                        anime[i-1] = 0x01 << (k-1);

                    lcd_cgram_writechar(aLCD, 1, anime);
                    lcd_display_customchar_pos(aLCD, 1, row, column);
                    usleep(100000);
                    lcd_clear_display(aLCD);
                }
                if (i%2 == 0)
                    column = 17-column;
            }
        }
    }

    lcd_close(aLCD);
    printf("Finished!\n");
    return 0;
}
