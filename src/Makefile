# MPLAB IDE generated this makefile for use with GNU make.
# Project: proj1.mcp
# Date: Wed Nov 14 14:59:17 2012

AS = pic32-as.exe
CC = pic32-gcc.exe
LD = pic32-ld.exe
AR = pic32-ar.exe
HX = pic32-bin2hex.exe
RM = rm

proj1.hex : proj1.elf
	$(HX) "proj1.elf"

proj1.elf : proj1.o lcd.o funcs.o leds.o
	$(CC) -mdebugger -mprocessor=32MX460F512L "proj1.o" "lcd.o" "funcs.o" "leds.o" -o"proj1.elf" -Wl,-L"C:\Program Files (x86)\Microchip\MPLAB C32 Suite\lib",-L"C:\Program Files (x86)\Microchip\MPLAB C32 Suite\pic32mx\lib",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,--defsym=__ICD2RAM=1,-Map="proj1.map"

proj1.o : proj1.c funcs.h leds.h lcd.h
	$(CC) -mprocessor=32MX460F512L -x c -c "proj1.c" -o"proj1.o" -MMD -MF"proj1.d" -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -g

lcd.o : lcd.c lcd.h funcs.h
	$(CC) -mprocessor=32MX460F512L -x c -c "lcd.c" -o"lcd.o" -MMD -MF"lcd.d" -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -g

funcs.o : funcs.c funcs.h
	$(CC) -mprocessor=32MX460F512L -x c -c "funcs.c" -o"funcs.o" -MMD -MF"funcs.d" -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -g

leds.o : leds.c funcs.h
	$(CC) -mprocessor=32MX460F512L -x c -c "leds.c" -o"leds.o" -MMD -MF"leds.d" -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -g

clean : 
	$(RM) "proj1.o" "lcd.o" "funcs.o" "leds.o" "proj1.elf" "proj1.hex"

