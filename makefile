COMMON_SOURCES = \
		  simple_parser.cpp\
		  tools.cpp\

COMMON_DIR = common
COMMON_SOURCES := $(addprefix $(COMMON_DIR)/, $(COMMON_SOURCES))

ASM_SOURCES = \
		  assembler.cpp\
		  labels.cpp\
		  op_handler.cpp\
		  
ASM_DIR = assembler
ASM_SOURCES := $(addprefix $(ASM_DIR)/, $(ASM_SOURCES))

# GEN_SOURCES_ALT = \
# 	  	  front_end/state_machine_generator/state_machine_functions.cpp\
# 		  front_end/state_machine_generator/state_machine_main.cpp


# COMP_SOURCES_ALT = \
# 		  back_end/compiler_ctor.cpp\
# 		  back_end/compiler.cpp\
# 		  back_end/compiler_main.cpp



INCLUDES = \
		  assembler\
		  common\

OBJ_DIR = obj
SOURCE_DIR = src

ASM_SOURCES := $(COMMON_SOURCES) $(ASM_SOURCES)
# GEN_SOURCES = $(COMMON_SOURCES) $(GEN_SOURCES_ALT)
# COMP_SOURCES = $(COMMON_SOURCES) $(COMP_SOURCES_ALT)

INCLUDES := $(addprefix -I$(SOURCE_DIR)/, $(INCLUDES))

#front_end part 
ASM_OBJECTS := $(addprefix $(OBJ_DIR)/, $(ASM_SOURCES:.cpp=.o))
ASM_SOURCES := $(addprefix $(SOURCE_DIR)/, $(ASM_SOURCES))
TARGET_ASM = asm.out

#key words generator
# GEN_OBJECTS := $(addprefix $(OBJ_DIR)/, $(GEN_SOURCES:.cpp=.o))
# GEN_SOURCES := $(addprefix $(SOURCE_DIR)/, $(GEN_SOURCES))
# TARGET_GEN = gen.out

#compiler part 
# COMP_OBJECTS := $(addprefix $(OBJ_DIR)/, $(COMP_SOURCES:.cpp=.o))
# COMP_SOURCES := $(addprefix $(SOURCE_DIR)/, $(COMP_SOURCES))
# TARGET_COMP = compiler.out

# c++/c compiler options
CC = g++ 
CFLAGS =  -D _DEBUG -ggdb3 -std=c++17 -O2 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations\
		 -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts\
		 -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal\
		 -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op\
		 -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self\
		 -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel\
		 -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods\
		 -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand\
		 -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix\
		 -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector\
		 -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -pie\
		 -fPIE -Werror=vla \
		 -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
CFLAGS += -lm
CFLAGS += $(INCLUDES)

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@mkdir -p logs
	@echo "Compiling" $<
	@$(CC) $(CFLAGS) -c $< -o $@ 	
	@echo "Compiled Successfully" $<

$(TARGET_ASM): $(ASM_OBJECTS)
	@echo "Linking..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Linked Successfully"

# $(TARGET_GEN): $(GEN_OBJECTS)
# 	@echo "Linking..."
# 	@$(CC) $(CFLAGS) $^ -o $@
# 	@echo "Linked Successfully"
	
# $(TARGET_COMP): $(COMP_OBJECTS)
# 	@echo "Linking..."
# 	@$(CC) $(CFLAGS) $^ -o $@
# 	@echo "Linked Successfully"

asm: $(TARGET_ASM)
	@./asm.out
# gen: $(TARGET_GEN)
# 	@./gen.out
# comp: $(TARGET_COMP)
# 	@./compiler.out
	
all : $(TARGET_ASM) 
	
clean:
	@rm -rf logs/*
	@rm -rf $(OBJ_DIR)
	@rm -f $(TARGET_ASM)
	@echo "Cleaned Successfully"