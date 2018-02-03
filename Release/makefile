################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include pcf8574/subdir.mk
-include lcdpcf8574/subdir.mk
-include i2c/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
TomatoBox2.lss \

FLASH_IMAGE += \
TomatoBox2.hex \

EEPROM_IMAGE += \
TomatoBox2.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: TomatoBox2.elf secondary-outputs

# Tool invocations
TomatoBox2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,TomatoBox2.map -mmcu=atmega328p -o "TomatoBox2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

TomatoBox2.lss: TomatoBox2.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S TomatoBox2.elf  >"TomatoBox2.lss"
	@echo 'Finished building: $@'
	@echo ' '

TomatoBox2.hex: TomatoBox2.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex TomatoBox2.elf  "TomatoBox2.hex"
	@echo 'Finished building: $@'
	@echo ' '

TomatoBox2.eep: TomatoBox2.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex TomatoBox2.elf  "TomatoBox2.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: TomatoBox2.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p TomatoBox2.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) TomatoBox2.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
