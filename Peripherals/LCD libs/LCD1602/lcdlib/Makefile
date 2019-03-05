.PHONY: lcd_test
lcd_test: lcd_test.o lcd_lib.o
	cc lcd_test.o lcd_lib.o -o a
	sudo ./a

lcd_lib.o: lcd_lib.h

.PHONY: clean
clean:
	rm *.o a
