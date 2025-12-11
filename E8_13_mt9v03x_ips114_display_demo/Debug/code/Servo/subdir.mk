################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Servo/Self_Servo.c 

COMPILED_SRCS += \
code/Servo/Self_Servo.src 

C_DEPS += \
code/Servo/Self_Servo.d 

OBJS += \
code/Servo/Self_Servo.o 


# Each subdirectory must supply rules for building sources it contributes
code/Servo/Self_Servo.src: ../code/Servo/Self_Servo.c code/Servo/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/TC264_p/TC264_CAR/E8_13_mt9v03x_ips114_display_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
code/Servo/Self_Servo.o: code/Servo/Self_Servo.src code/Servo/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Servo

clean-code-2f-Servo:
	-$(RM) code/Servo/Self_Servo.d code/Servo/Self_Servo.o code/Servo/Self_Servo.src

.PHONY: clean-code-2f-Servo

