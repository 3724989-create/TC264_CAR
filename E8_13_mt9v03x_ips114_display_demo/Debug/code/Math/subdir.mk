################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Math/self_math.c 

COMPILED_SRCS += \
code/Math/self_math.src 

C_DEPS += \
code/Math/self_math.d 

OBJS += \
code/Math/self_math.o 


# Each subdirectory must supply rules for building sources it contributes
code/Math/self_math.src: ../code/Math/self_math.c code/Math/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/TC264_p/TC264_CAR/E8_13_mt9v03x_ips114_display_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/Math/self_math.o: code/Math/self_math.src code/Math/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Math

clean-code-2f-Math:
	-$(RM) code/Math/self_math.d code/Math/self_math.o code/Math/self_math.src

.PHONY: clean-code-2f-Math

