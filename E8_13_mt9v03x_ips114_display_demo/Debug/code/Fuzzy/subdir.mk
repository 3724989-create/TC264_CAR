################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Fuzzy/VC_Fuzzy.c 

COMPILED_SRCS += \
code/Fuzzy/VC_Fuzzy.src 

C_DEPS += \
code/Fuzzy/VC_Fuzzy.d 

OBJS += \
code/Fuzzy/VC_Fuzzy.o 


# Each subdirectory must supply rules for building sources it contributes
code/Fuzzy/VC_Fuzzy.src: ../code/Fuzzy/VC_Fuzzy.c code/Fuzzy/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/TC264_p/TC264_CAR/E8_13_mt9v03x_ips114_display_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/Fuzzy/VC_Fuzzy.o: code/Fuzzy/VC_Fuzzy.src code/Fuzzy/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Fuzzy

clean-code-2f-Fuzzy:
	-$(RM) code/Fuzzy/VC_Fuzzy.d code/Fuzzy/VC_Fuzzy.o code/Fuzzy/VC_Fuzzy.src

.PHONY: clean-code-2f-Fuzzy

