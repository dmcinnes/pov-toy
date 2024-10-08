# Name: Makefile
#
# A Makefile for the ATtiny84.
#
# https://gist.github.com/electronut/5763929

MCU        = attiny84
DEVICE     = attiny84
CLOCK      = 1000000
PROGRAMMER = usbtiny
OBJECTS    = main.o

# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega168p&LOW=E2&HIGH=DF&EXTENDED=F9&LOCKBIT=FF
FUSES      = -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xF9:m

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE) -B3
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(MCU)

# symbolic targets:
all:  main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

reset:
# verify the fuse, this will trigger a reset on the AVR
	$(AVRDUDE) -U lfuse:v:0xE2:m

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS) -Wl,-u,vfprintf -lprintf_flt -lm

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(MCU) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
