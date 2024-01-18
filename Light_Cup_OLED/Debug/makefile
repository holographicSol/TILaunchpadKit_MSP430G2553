################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/tools/msp430-gcc/4.6.6

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Light_Cup_OLED.o" \
$(GEN_CMDS__FLAG) \
-Wl,--start-group \
-lmsp430_MSP-EXP430G2553LP_core \
-lm \
-Wl,--end-group \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Light_Cup_OLED.out \

EXE_OUTPUTS__QUOTED += \
"Light_Cup_OLED.out" \

BIN_OUTPUTS += \
Light_Cup_OLED.hex \

BIN_OUTPUTS__QUOTED += \
"Light_Cup_OLED.hex" \


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Light_Cup_OLED.out" secondary-outputs

dependents:
	-cd C:/Users/Benjamin/workspace_v9/msp430_MSP-EXP430G2553LP_core/Debug && $(MAKE) all

# Tool invocations
Light_Cup_OLED.out: $(OBJS) $(GEN_CMDS) C:/Users/Benjamin/workspace_v9/msp430_MSP-EXP430G2553LP_core/Debug/libmsp430_MSP-EXP430G2553LP_core.a
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/tools/msp430-gcc/4.6.6/bin/msp430-gcc-4.6.3.exe" -DF_CPU=16000000L -DENERGIA_MSP_EXP430G2553LP -DENERGIA_ARCH_MSP430 -DENERGIA=23 -DARDUINO=10610 -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -mmcu=msp430g2553 -Wl,-Map,"Light_Cup_OLED.map" -Wl,--gc-sections -L"C:/Users/Benjamin/workspace_v9/msp430_MSP-EXP430G2553LP_core/Debug" -L"C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/tools/msp430-gcc/4.6.6/include" -Wl,--undefined,main -w -Os -fno-rtti -fno-exceptions -o"Light_Cup_OLED.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Light_Cup_OLED.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: GNU Objcopy Utility'
	"C:/Users/Benjamin/AppData/Local/Energia15/packages/energia/tools/msp430-gcc/4.6.6/bin/msp430-objcopy.exe" -O ihex --remove-section .eeprom $(EXE_OUTPUTS__QUOTED) "Light_Cup_OLED.hex"
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Light_Cup_OLED.o" 
	-$(RM) "Light_Cup_OLED.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:
C:/Users/Benjamin/workspace_v9/msp430_MSP-EXP430G2553LP_core/Debug/libmsp430_MSP-EXP430G2553LP_core.a:

-include ../makefile.targets

