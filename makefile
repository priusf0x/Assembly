SOURCES_COMPILER =  compiler/read_commands.cpp compiler/compiler.cpp common/tools.cpp
SOURCES_DISASSEMBLER = disasm/disassembler.cpp disasm/disassembler_commands.cpp
SOURCES_PROCESSOR = stack/stack.cpp processor/processor.cpp processor/processor_functions.cpp common/logger.cpp common/tools.cpp

OBJ_DIR = obj
SOURCE_DIR = src

OBJECTS_COMPILER := $(addprefix $(OBJ_DIR)/, $(SOURCES_COMPILER:.cpp=.o))
SOURCES_COMPILER  := $(addprefix $(SOURCE_DIR)/, $(SOURCES_COMPILER))

OBJECTS_DISASSEMBLER := $(addprefix $(OBJ_DIR)/, $(SOURCES_DISASSEMBLER:.cpp=.o))
SOURCES_DISASSEMBLER  := $(addprefix $(SOURCE_DIR)/, $(SOURCES_DISASSEMBLER))

OBJECTS_PROCESSOR := $(addprefix $(OBJ_DIR)/, $(SOURCES_PROCESSOR:.cpp=.o))
SOURCES_PROCESSOR := $(addprefix $(SOURCE_DIR)/, $(SOURCES_PROCESSOR))

HEADERS = $(wildcard $(SOURCE_DIR)/*.h)

TARGET_COMPILER = compiler.out
TARGET_DISASSEMBLER = disassembler.out
TARGET_PROCESSOR= processor.out

CC = g++

CFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O2 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CFLAGS += -I$(SOURCE_DIR)/compiler -I$(SOURCE_DIR)/common -I$(SOURCE_DIR)/processor -I$(SOURCE_DIR)/disasm -I$(SOURCE_DIR)/stack

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $@)/compiler
	@mkdir -p $(dir $@)/disassembler
	@mkdir -p logs
	@echo "Compiling" $<
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled Successfully" $<

$(TARGET_COMPILER): $(OBJECTS_COMPILER)
	@echo "Linking..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Linked Successfully"

$(TARGET_DISASSEMBLER): $(OBJECTS_DISASSEMBLER)
	@echo "Linking..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Linked Successfully"

$(TARGET_PROCESSOR): $(OBJECTS_PROCESSOR)
	@echo "Linking..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Linked Successfully"

comp: $(TARGET_COMPILER)
	@./$(TARGET_COMPILER)

disasm: $(TARGET_DISASSEMBLER)
	@./$(TARGET_DISASSEMBLER)

proc : $(TARGET_PROCESSOR)
	@./$(TARGET_PROCESSOR)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(TARGET_COMPILER)
	@rm -f $(TARGET_PROCESSOR)
	@rm -f $(TARGET_DISASSEMBLER)
	@rm -rf logs
	@echo "Cleaned Successfully"
