################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./hardware.obj" \
"./interruptions.obj" \
"./lab01.obj" \
"./main.obj" \
"./startup_msp432p401r_ccs.obj" \
"./system_msp432p401r.obj" \
"./include/HAL_I2C.obj" \
"./include/HAL_OPT3001.obj" \
"../driverlib_empty_project_ccs.cmd" \
"/home/fabian/local/MSPWare/MSP432Ware_3_50_00_02/driverlib/driverlib/MSP432P4xx/ccs/msp432p4xx_driverlib.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include include/subdir_vars.mk
-include subdir_rules.mk
-include include/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Lab01.out \

EXE_OUTPUTS__QUOTED += \
"Lab01.out" \

BIN_OUTPUTS += \
Lab01.bin \

BIN_OUTPUTS__QUOTED += \
"Lab01.bin" \


# All Target
all: Lab01.out

# Tool invocations
Lab01.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP432 Linker'
	"/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --display_error_number --diag_warning=225 -z -m"Lab01.map" --stack_size=512 -i"/home/fabian/local/CCS/ccsv6/ccs_base/arm/include" -i"/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/lib" -i"/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --reread_libs --display_error_number --warn_sections --diag_wrap=off --xml_link_info="Lab01_linkInfo.xml" --rom_model -o "Lab01.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

Lab01.bin: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP432 Hex Utility'
	"/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armhex"  -o "Lab01.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "hardware.d" "interruptions.d" "lab01.d" "main.d" "startup_msp432p401r_ccs.d" "system_msp432p401r.d" "include/HAL_I2C.d" "include/HAL_OPT3001.d" 
	-$(RM) "hardware.obj" "interruptions.obj" "lab01.obj" "main.obj" "startup_msp432p401r_ccs.obj" "system_msp432p401r.obj" "include/HAL_I2C.obj" "include/HAL_OPT3001.obj" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-bash "/home/fabian/local/CCS/ccsv6/utils/tiobj2bin/tiobj2bin" "Lab01.out" "Lab01.bin" "/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armofd" "/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armhex" "/home/fabian/local/CCS/ccsv6/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

