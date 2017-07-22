################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Users\ivanm\Documents\Arduino\libraries\Adafruit_SSD1306\Adafruit_SSD1306.cpp 

LINK_OBJ += \
.\libraries\Adafruit_SSD1306\Adafruit_SSD1306.cpp.o 

CPP_DEPS += \
.\libraries\Adafruit_SSD1306\Adafruit_SSD1306.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\Adafruit_SSD1306\Adafruit_SSD1306.cpp.o: C:\Users\ivanm\Documents\Arduino\libraries\Adafruit_SSD1306\Adafruit_SSD1306.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\ivanm\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\Users\ivanm\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.19\cores\arduino" -I"C:\Users\ivanm\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.19\variants\standard" -I"C:\Users\ivanm\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.19\libraries\SPI" -I"C:\Users\ivanm\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.19\libraries\SPI\src" -I"C:\Users\ivanm\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.19\libraries\Wire" -I"C:\Users\ivanm\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.19\libraries\Wire\src" -I"C:\Users\ivanm\Documents\Arduino\libraries\Adafruit_GFX" -I"C:\Users\ivanm\Documents\Arduino\libraries\Adafruit_SSD1306" -I"C:\Users\ivanm\Documents\Arduino\libraries\GameMachine" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


