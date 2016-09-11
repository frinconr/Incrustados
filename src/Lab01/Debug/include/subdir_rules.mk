################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
include/HAL_I2C.obj: ../include/HAL_I2C.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/fabian/local/CCS/ccsv6/ccs_base/arm/include" --include_path="/home/fabian/local/CCS/ccsv6/ccs_base/arm/include/" --include_path="/home/fabian/local/CCS/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/fabian/dev/incrustados/src/Lab01" --include_path="/home/fabian/local/MSPWare/MSP432Ware_3_50_00_02/driverlib/driverlib/MSP432P4xx/" --include_path="/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include/" --include_path="/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="include/HAL_I2C.d" --obj_directory="include" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

include/HAL_OPT3001.obj: ../include/HAL_OPT3001.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/fabian/local/CCS/ccsv6/ccs_base/arm/include" --include_path="/home/fabian/local/CCS/ccsv6/ccs_base/arm/include/" --include_path="/home/fabian/local/CCS/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/fabian/dev/incrustados/src/Lab01" --include_path="/home/fabian/local/MSPWare/MSP432Ware_3_50_00_02/driverlib/driverlib/MSP432P4xx/" --include_path="/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include/" --include_path="/home/fabian/local/CCS/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="include/HAL_OPT3001.d" --obj_directory="include" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


