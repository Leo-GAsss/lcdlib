# LCD1602 library based on linux IIC libs

This is a C port of [LCD1602 Python Library](https://gist.github.com/DenisFromHR/cc863375a6e19dce359d), with some modifications.

### Usage

include the header  

```c
#include "lcd_lib.h"
```

and compile  

```bash
gcc yourSourceCode.c lcd_lib.c -o yourBinaryFileName
```

### Documentation

Sorry, not availiable yet...  

### TODO

- [x] Add `Cursor Control`

- [x] Entire Image Move

- [ ] Read Busy Signal and Strict Operation Period `Important !`

- [ ] Error Handling

- [ ] Documentation

- [ ] Write a document about `LCD1602`

### Change Log

- *2019-02-01*

  - *Add*

    - Display and Cursor Control
    - Config struct

- *2018-12-27*

  - *Add*

    - `lcd_close`

  - *First Version* Provide basic functions, including:  

    - `lcd_clear_display`  
    - `lcd_backlight_switch`  
    - `lcd_display_string` display a string (first line and first column)  
    - `lcd_display_string_pos` customize the string positon  
    - `lcd_cgram_writechar` for custom char  
    - `lcd_display_customchar`  
    - `lcd_display_customchar_pos`  
