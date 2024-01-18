################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Light_Cup_OLED.cpp: ../Light_Cup_OLED.ino
	@echo 'Building file: "$<"'
	@echo 'Invoking: Resource Custom Build Step'
	
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ./%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/tools/msp430-gcc/4.6.6/bin/msp430-gcc-4.6.3.exe" -c -mmcu=msp430g2553 -DF_CPU=16000000L -DENERGIA_MSP_EXP430G2553LP -DENERGIA_ARCH_MSP430 -DENERGIA=23 -DARDUINO=10610 -I"C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/tools/msp430-gcc/4.6.6/include" -I"C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/hardware/msp430/1.0.7/cores/msp430" -I"C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/hardware/msp430/1.0.7/variants/MSP-EXP430G2553LP" -I"C:/Users/Benjamin/workspace_v9/Light_Cup_OLED" -I"C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/tools/msp430-gcc/4.6.6/msp430/include" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"   -fno-threadsafe-statics -fno-exceptions $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


