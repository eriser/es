#################################################################
## This Makefile Exported by MinGW Developer Studio
## Copyright (c) 2002-2004 by Parinya Thipchart
#################################################################

ifneq (,$(findstring Release, $(CFG)))
  override CFG = Release
else
  override CFG = Debug
endif

# MOD
PROJECT = eswav
CC = g++

# MOD
ifeq ($(CFG),Debug)
  OBJ_DIR = Debug
  OUTPUT_DIR = Debug
  TARGET = prj
  C_INCLUDE_DIRS = 
  C_PREPROC = -D_DEBUG -D__LINUX__
  CFLAGS = -pipe -Wall -W -g2 -O0 -frtti -Wno-variadic-macros -pg # -pedantic 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = -L"../bin/lib" 
  LIBS =
  LDFLAGS = -pipe -pg
endif

# MOD
ifeq ($(CFG),Release)
  OBJ_DIR = Release
  OUTPUT_DIR = Release
  TARGET = prj
  C_INCLUDE_DIRS = 
  C_PREPROC = -D_RELEASE -D__LINUX__
  CFLAGS = -pipe -ffast-math -fno-builtin -W -g0 -O0 -fno-rtti -Wno-variadic-macros
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = -L"../bin/lib" 
  LIBS =
  LDFLAGS = -pipe
endif

ifeq ($(OS),Windows_NT)
  NULL =
else
  NULL = nul
endif

SRC_OBJS = \
  $(OBJ_DIR)/Str.o	\
  $(OBJ_DIR)/List.o	\
  $(OBJ_DIR)/ptch.o

# MOD
define build_target
@echo Linking...
@$(CC) -o "$(OUTPUT_DIR)/$(TARGET)" $(SRC_OBJS) $(LIB_DIRS) $(LIBS) $(LDFLAGS)
endef

define compile_source
@echo Compiling $<
@$(CC) $(CFLAGS) $(C_PREPROC) $(C_INCLUDE_DIRS) -c "$<" -o "$@"
endef

.PHONY: print_header directories

$(TARGET): print_header directories $(SRC_OBJS)
	$(build_target)

.PHONY: clean cleanall

# MOD
cleanall:
	@echo Deleting intermediate files for 'prj - $(CFG)'
	-@rm $(OBJ_DIR)/*.o
	-@rm "$(OUTPUT_DIR)/$(TARGET)"
	-@rm -r "$(OUTPUT_DIR)"

# MOD
clean:
	@echo Deleting intermediate files for 'prj - $(CFG)'
	-@rm $(OBJ_DIR)/*.o

print_header:
	@echo ----------Configuration: ptch - $(CFG)----------

# MOD
directories:
	-@if [ ! -d "$(OUTPUT_DIR)" ]; then mkdir "$(OUTPUT_DIR)"; fi
	-@if [ ! -d "$(OBJ_DIR)" ]; then mkdir "$(OBJ_DIR)"; fi


# MOD
$(OBJ_DIR)/Str.o: ../src/Str/Str.cpp	\
../src/Str/Str.h	\
../src/Str/Str.inline.h
	$(compile_source)

$(OBJ_DIR)/List.o: ../src/List/List.cpp	\
../src/List/List.h	\
../src/List/List.inline.h
	$(compile_source)

$(OBJ_DIR)/ptch.o: ../src_ptch/ptch.cpp	\
../src/List/List.h	\
../src/Str/Str.h
	$(compile_source)

