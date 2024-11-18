FILE := Makefile
OS_EXT =
ifeq ($(OS),Windows_NT)
	OS_EXT := .win32
	MYFIND :=C:/msys64/usr/bin/find.exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		OS_EXT := .linux
    endif
    ifeq ($(UNAME_S),Darwin)
		OS_EXT := .mac
    endif
	MYFIND :=find
endif

COMMAND_CDEFINES		=-DCHICHI
COMMAND_CFLAGS			=-Wvarargs
DEBUG_LEVEL				=-g3 -gcodeview

ifeq ($(OPTI),none)
	COMMAND_CFLAGS		+= -O0
else
	COMMAND_CFLAGS		+= -O3
endif

ifeq ($(ASAN_USE),ON)
	COMMAND_CFLAGS		+= -fsanitize=address
endif

NAME			=fusee
BUILD_DIR		=build
OBJ_DIR			=$(BUILD_DIR)/obj
OBJ				=obj
ECHO_E			=echo

DEPENDS_FLAGS	=-MMD -MP
CC				=clang
CLINKER			=clang
CFLAGS			=$(COMMAND_CFLAGS)
CFLAGS			+= -fdiagnostics-absolute-paths
CFLAGS			+= -std=c23
CFLAGS			+= -fno-inline -fno-omit-frame-pointer
CFLAGS			+= -Wno-missing-field-initializers -Wno-unused-but-set-variable
CDEFINES		=$(COMMAND_CDEFINES)
CPP				=clang++
CPPFLAGS		=-Wno-format
CPPDEFINES		=

INCLUDE_DIRS	=-Isrc -Ithirdparty
LIB_PATH		=
LIBS			=

SRC_DIR			=src
CORE_DIR		=$(SRC_DIR)/core
ROOT_FOLDER		=$(shell $(MYFIND) $(SRC_DIR) -maxdepth 1 -type f -name '*.c')
ROOT_OBJS		=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(ROOT_FOLDER))
CORE_FILES		=$(shell $(MYFIND) $(CORE_DIR) -type f -name '*.c')
CORE_OBJS		=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CORE_FILES))

C_OBJS			=$(ROOT_OBJS) $(CORE_OBJS)

ALL_C_FILES		=$(ROOT_FOLDER) $(CORE_FILES)

ifdef CPP_USE
	CPP_FILES	+= $(wildcard $(SRC_DIR)/*.cpp)
	CPP_OBJS	+= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))
endif

color_link		=$(ECHO_E) "$(PURPLE)$(CC)$(NC) $(CFLAGS) -o $(YELLOW)$@$(NC) $(BLUE)$^$(NC) $(LIB_PATH) $(LIBS)"
cpp_compile		=$(ECHO_E) "$(PURPLE)$(CPP)$(NC) -c $(YELLOW)$<$(NC) -o $(BLUE)$@$(NC) $(CPPFLAGS) $(INCLUDE_DIRS)"
c_compile		=$(ECHO_E) "$(PURPLE)$(CC)$(NC) -c $(YELLOW)$<$(NC) -o $(BLUE)$@$(NC) $(YELLOW)$(DEBUG_LEVEL)$(NC) $(CFLAGS) $(INCLUDE_DIRS)"

include $(FILE)$(OS_EXT)

DEPENDS			=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d,$(ALL_C_FILES))

CFLAGS			+= $(CDEFINES)
CPPFLAGS		+= $(CPPDEFINES)

ifeq ($(CC),clang)
MJJSON			=-MJ$@.json
endif

ifeq ($(CC),clang-19)
MJJSON			=-MJ$@.json
endif

#*************************** ALL ***************************************#

all: $(OBJ_DIR) $(BUILD_DIR)/$(OUTPUT) $(CCJSON) $(SHADER_OBJS)

#*************************** BUILDER ***********************************#

$(BUILD_DIR)/$(OBJ):
	@mkdir -p build/obj

#*************************** LINK_OBJS *********************************#

$(BUILD_DIR)/$(OUTPUT): $(C_OBJS) $(CPP_OBJS)
	@$(color_link)
	@$(CC) $(DEBUG_LEVEL) $(CFLAGS) -o $@ $^ $(INCLUDE_DIRS) $(LIB_PATH) $(LIBS)

#*************************** COMPILE_FILES *****************************#

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(cpp_compile)
	@$(CPP) $(DEBUG_LEVEL) $(CPPFLAGS) $(DEPENDS_FLAGS) $(MJJSON) -c $< -o $@ $(INCLUDE_DIRS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(c_compile)
	@$(CC) $(DEBUG_LEVEL) $(CFLAGS) $(DEPENDS_FLAGS) $(MJJSON) -c $< -o $@ $(INCLUDE_DIRS)

#*************************** CLEAN *************************************#

-include $(DEPENDS)

clean:
	@$(ECHO_E) "$(RED)Deleting files..$(NC)"
	rm -f $(BUILD_DIR)/$(OUTPUT)
	rm -rf $(OBJ_DIR)
	$(RM_EXTRA)
	$(RM_EXTRA2)

fclean: clean
	rm -f compile_commands.json

fc: clean

re: clean all

re_fast: clean
	@make --no-print-directory -f $(FILE) -j24 all

.PHONY: all re clean fclean fc re_fast
