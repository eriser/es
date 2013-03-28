#################################################################
## This Makefile Exported by MinGW Developer Studio
## Copyright (c) 2002-2004 by Parinya Thipchart
#################################################################

ifneq (,$(findstring Release, $(CFG)))
  override CFG = Release
else
  override CFG = Debug
endif

PROJECT = ptch
CC = g++

ifeq ($(CFG),Debug)
  OBJ_DIR = Debug
  OUTPUT_DIR = Debug
  TARGET = ptch.exe
  C_INCLUDE_DIRS = 
  C_PREPROC = 
  CFLAGS = -pipe  -Wall -W -g2 -O0 -frtti -fexceptions 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = 
  LIBS = 
  LDFLAGS = -pipe 
endif

ifeq ($(CFG),Release)
  OBJ_DIR = Release
  OUTPUT_DIR = Release
  TARGET = ptch.exe
  C_INCLUDE_DIRS = 
  C_PREPROC = 
  CFLAGS = -pipe  -Wall -W -g0 -O2 -fno-rtti -fno-exceptions 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = 
  LIBS = 
  LDFLAGS = -pipe -s 
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

define build_target
@echo Linking...
@$(CC) -o "$(OUTPUT_DIR)\$(TARGET)" $(SRC_OBJS) $(LIB_DIRS) $(LIBS) $(LDFLAGS)
endef

define compile_source
@echo Compiling $<
@$(CC) $(CFLAGS) $(C_PREPROC) $(C_INCLUDE_DIRS) -c "$<" -o "$@"
endef

.PHONY: print_header directories

$(TARGET): print_header directories $(SRC_OBJS)
	$(build_target)

.PHONY: clean cleanall

cleanall:
	@echo Deleting intermediate files for 'ptch - $(CFG)'
	-@del $(OBJ_DIR)\*.o
	-@del "$(OUTPUT_DIR)\$(TARGET)"
	-@rmdir "$(OUTPUT_DIR)"

clean:
	@echo Deleting intermediate files for 'ptch - $(CFG)'
	-@del $(OBJ_DIR)\*.o

print_header:
	@echo ----------Configuration: ptch - $(CFG)----------

directories:
	-@if not exist "$(OUTPUT_DIR)\$(NULL)" mkdir "$(OUTPUT_DIR)"
	-@if not exist "$(OBJ_DIR)\$(NULL)" mkdir "$(OBJ_DIR)"

$(OBJ_DIR)/Str.o: ../src/Str/Str.cpp	\
../src/Str/Str.h	\
../src/File/File.h	\
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

