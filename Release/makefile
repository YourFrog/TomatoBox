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
lcd_i2c.lss \

FLASH_IMAGE += \
lcd_i2c.hex \

EEPROM_IMAGE += \
lcd_i2c.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: lcd_i2c.elf secondary-outputs

# Tool invocations
lcd_i2c.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,lcd_i2c.map -mmcu=atmega328 -o "lcd_i2c.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

lcd_i2c.lss: lcd_i2c.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S lcd_i2c.elf  >"lcd_i2c.lss"
	@echo 'Finished building: $@'
	@echo ' '

lcd_i2c.hex: lcd_i2c.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex lcd_i2c.elf  "lcd_i2c.hex"
	@echo 'Finished building: $@'
	@echo ' '

lcd_i2c.eep: lcd_i2c.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex lcd_i2c.elf  "lcd_i2c.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: lcd_i2c.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328 lcd_i2c.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) lcd_i2c.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
