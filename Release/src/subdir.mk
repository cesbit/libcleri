C_SRCS += \
../src/choice.c \
../src/cleri.c \
../src/dup.c \
../src/expecting.c \
../src/grammar.c \
../src/keyword.c \
../src/kwcache.c \
../src/kwmatch.c \
../src/list.c \
../src/node.c \
../src/olist.c \
../src/optional.c \
../src/parse.c \
../src/prio.c \
../src/ref.c \
../src/regex.c \
../src/repeat.c \
../src/rule.c \
../src/sequence.c \
../src/this.c \
../src/token.c \
../src/tokens.c \
../src/version.c


OBJS += \
./src/choice.o \
./src/cleri.o \
./src/dup.o \
./src/expecting.o \
./src/grammar.o \
./src/keyword.o \
./src/kwcache.o \
./src/kwmatch.o \
./src/list.o \
./src/node.o \
./src/olist.o \
./src/optional.o \
./src/parse.o \
./src/prio.o \
./src/ref.o \
./src/regex.o \
./src/repeat.o \
./src/rule.o \
./src/sequence.o \
./src/this.o \
./src/token.o \
./src/tokens.o \
./src/version.o

C_DEPS += \
./src/choice.d \
./src/cleri.d \
./src/dup.d \
./src/expecting.d \
./src/grammar.d \
./src/keyword.d \
./src/kwcache.d \
./src/kwmatch.d \
./src/list.d \
./src/node.d \
./src/olist.d \
./src/optional.d \
./src/parse.d \
./src/prio.d \
./src/ref.d \
./src/regex.d \
./src/repeat.d \
./src/rule.d \
./src/sequence.d \
./src/this.d \
./src/token.d \
./src/tokens.d \
./src/version.d

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -DNDEBUG -I../inc -O3 -Winline -Wall $(CPPFLAGS) $(CFLAGS) -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
