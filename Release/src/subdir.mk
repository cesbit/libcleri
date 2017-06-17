################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/errmap.c \
../src/req.c \
../src/queue.c \
../src/pkg.c \
../src/resp.c \
../src/series.c \
../src/version.c \
../src/siridb.c \
../src/packer.c

OBJS += \
./src/errmap.o \
./src/req.o \
./src/queue.o \
./src/pkg.o \
./src/resp.o \
./src/series.o \
./src/version.o \
./src/siridb.o \
./src/packer.o

C_DEPS += \
./src/errmap.d \
./src/req.d \
./src/queue.d \
./src/pkg.d \
./src/resp.d \
./src/series.d \
./src/version.d \
./src/siridb.d \
./src/packer.d

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I.././include -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


