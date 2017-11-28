################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OS := $(shell uname)
ifeq ($(OS),Darwin)
FN := libcleri.dylib
INSTALL_PATH := /usr/local
SO_NAME := install_name
else
FN := libcleri.so
INSTALL_PATH := /usr
SO_NAME := soname
endif

# Add inputs and outputs from these tool invocations to the build variables

# All Target
all: libcleri

# Tool invocations
libcleri: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc -shared -Wl,-$(SO_NAME),$(FN).$(MAJOR) -o $(FN) $(OBJS) $(USER_OBJS) $(LIBS) $(LDFLAGS)
	@chmod -x $(FN)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(OBJS)$(C_DEPS) $(FN)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
