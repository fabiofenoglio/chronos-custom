################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
driver/as.obj: ../driver/as.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx -g -O4 --opt_for_speed=0 --define=__CCE__ --define=ISM_LF --include_path="C:/Program Files (x86)/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --include_path="C:/Program Files (x86)/ti/ccsv5/msp430/include" --include_path="D:/Git/MyChronos/include" --include_path="D:/Git/MyChronos/driver" --include_path="D:/Git/MyChronos/logic" --include_path="D:/Git/MyChronos/bluerobin" --include_path="D:/Git/MyChronos/simpliciti" --diag_warning=225 --call_assumptions=0 --auto_inline=0 --gen_opt_info=2 --printf_support=minimal --preproc_with_compile --preproc_dependency="driver/as.pp" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

driver/bmp_as.obj: ../driver/bmp_as.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx -g -O4 --opt_for_speed=0 --define=__CCE__ --define=ISM_LF --include_path="C:/Program Files (x86)/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --include_path="C:/Program Files (x86)/ti/ccsv5/msp430/include" --include_path="D:/Git/MyChronos/include" --include_path="D:/Git/MyChronos/driver" --include_path="D:/Git/MyChronos/logic" --include_path="D:/Git/MyChronos/bluerobin" --include_path="D:/Git/MyChronos/simpliciti" --diag_warning=225 --call_assumptions=0 --auto_inline=0 --gen_opt_info=2 --printf_support=minimal --preproc_with_compile --preproc_dependency="driver/bmp_as.pp" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

driver/bmp_ps.obj: ../driver/bmp_ps.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx -g -O4 --opt_for_speed=0 --define=__CCE__ --define=ISM_LF --include_path="C:/Program Files (x86)/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --include_path="C:/Program Files (x86)/ti/ccsv5/msp430/include" --include_path="D:/Git/MyChronos/include" --include_path="D:/Git/MyChronos/driver" --include_path="D:/Git/MyChronos/logic" --include_path="D:/Git/MyChronos/bluerobin" --include_path="D:/Git/MyChronos/simpliciti" --diag_warning=225 --call_assumptions=0 --auto_inline=0 --gen_opt_info=2 --printf_support=minimal --preproc_with_compile --preproc_dependency="driver/bmp_ps.pp" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

driver/cma_as.obj: ../driver/cma_as.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx -g -O4 --opt_for_speed=0 --define=__CCE__ --define=ISM_LF --include_path="C:/Program Files (x86)/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --include_path="C:/Program Files (x86)/ti/ccsv5/msp430/include" --include_path="D:/Git/MyChronos/include" --include_path="D:/Git/MyChronos/driver" --include_path="D:/Git/MyChronos/logic" --include_path="D:/Git/MyChronos/bluerobin" --include_path="D:/Git/MyChronos/simpliciti" --diag_warning=225 --call_assumptions=0 --auto_inline=0 --gen_opt_info=2 --printf_support=minimal --preproc_with_compile --preproc_dependency="driver/cma_as.pp" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

driver/cma_ps.obj: ../driver/cma_ps.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx -g -O4 --opt_for_speed=0 --define=__CCE__ --define=ISM_LF --include_path="C:/Program Files (x86)/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --include_path="C:/Program Files (x86)/ti/ccsv5/msp430/include" --include_path="D:/Git/MyChronos/include" --include_path="D:/Git/MyChronos/driver" --include_path="D:/Git/MyChronos/logic" --include_path="D:/Git/MyChronos/bluerobin" --include_path="D:/Git/MyChronos/simpliciti" --diag_warning=225 --call_assumptions=0 --auto_inline=0 --gen_opt_info=2 --printf_support=minimal --preproc_with_compile --preproc_dependency="driver/cma_ps.pp" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

driver/ps.obj: ../driver/ps.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx -g -O4 --opt_for_speed=0 --define=__CCE__ --define=ISM_LF --include_path="C:/Program Files (x86)/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Program Files (x86)/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --include_path="C:/Program Files (x86)/ti/ccsv5/msp430/include" --include_path="D:/Git/MyChronos/include" --include_path="D:/Git/MyChronos/driver" --include_path="D:/Git/MyChronos/logic" --include_path="D:/Git/MyChronos/bluerobin" --include_path="D:/Git/MyChronos/simpliciti" --diag_warning=225 --call_assumptions=0 --auto_inline=0 --gen_opt_info=2 --printf_support=minimal --preproc_with_compile --preproc_dependency="driver/ps.pp" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


