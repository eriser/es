#################################################################
## This Makefile Exported by MinGW Developer Studio
## Copyright (c) 2002-2004 by Parinya Thipchart
#################################################################

ifneq (,$(findstring Release, $(CFG)))
  override CFG = Release
else
  override CFG = Debug
endif

PROJECT = eswav
CC = g++

ifeq ($(CFG),Debug)
  OBJ_DIR = Debug
  OUTPUT_DIR = Debug
  TARGET = eswav.exe
  C_INCLUDE_DIRS = 
  C_PREPROC = -D__WINDOWS__ 
  CFLAGS = -pipe  -Wall -g2 -O0 -frtti -fexceptions 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = -L"../bin/lib" 
  LIBS = -les8bII_mingw_static -lwinmm 
  LDFLAGS = -pipe 
endif

ifeq ($(CFG),Release)
  OBJ_DIR = Release
  OUTPUT_DIR = Release
  TARGET = eswav.exe
  C_INCLUDE_DIRS = 
  C_PREPROC = -D__WINDOWS__ 
  CFLAGS = -pipe  -Wall -g0 -O2 -frtti -fexceptions 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = -L"../bin/lib" 
  LIBS = -les8bII_mingw_static -lwinmm 
  LDFLAGS = -pipe 
endif

ifeq ($(OS),Windows_NT)
  NULL =
else
  NULL = nul
endif

SRC_OBJS = \
  $(OBJ_DIR)/DEF.o	\
  $(OBJ_DIR)/Main.o	\
  $(OBJ_DIR)/Str.o	\
  $(OBJ_DIR)/List.o	\
  $(OBJ_DIR)/File.o	\
  $(OBJ_DIR)/FileStream.o	\
  $(OBJ_DIR)/FileStreamWav.o	\
  $(OBJ_DIR)/Sys.o

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
	@echo Deleting intermediate files for 'eswav - $(CFG)'
	-@del $(OBJ_DIR)\*.o
	-@del "$(OUTPUT_DIR)\$(TARGET)"
	-@rmdir "$(OUTPUT_DIR)"

clean:
	@echo Deleting intermediate files for 'eswav - $(CFG)'
	-@del $(OBJ_DIR)\*.o

print_header:
	@echo ----------Configuration: eswav - $(CFG)----------

directories:
	-@if not exist "$(OUTPUT_DIR)\$(NULL)" mkdir "$(OUTPUT_DIR)"
	-@if not exist "$(OBJ_DIR)\$(NULL)" mkdir "$(OBJ_DIR)"

$(OBJ_DIR)/DEF.o: ../src_eswav/DEF.cpp	\
../src_eswav/DEF.h
	$(compile_source)

$(OBJ_DIR)/Main.o: ../src_eswav/Main.cpp	\
../src_eswav/Main.h	\
../src_eswav/DEF.h	\
../src/Sys/Sys.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src_lib/es.h	\
../src/File/FileStreamWav.h
	$(compile_source)

$(OBJ_DIR)/Str.o: ../src/Str/Str.cpp	\
../src/Str/Str.h	\
../src/File/File.h	\
../src/Str/Str.inline.h
	$(compile_source)

$(OBJ_DIR)/List.o: ../src/List/List.cpp	\
../src/List/List.h	\
../src/List/List.inline.h
	$(compile_source)

$(OBJ_DIR)/File.o: ../src/File/File.cpp	\
../src/File/File.h
	$(compile_source)

$(OBJ_DIR)/FileStream.o: ../src/File/FileStream.cpp	\
../src/File/FileStream.h	\
../src/File/File.h
	$(compile_source)

$(OBJ_DIR)/FileStreamWav.o: ../src/File/FileStreamWav.cpp	\
../src/File/FileStreamWav.h	\
../src/File/FileStream.h	\
../src/File/File.h
	$(compile_source)

$(OBJ_DIR)/Sys.o: ../src/Sys/Sys.cpp	\
../src/Sys/Sys.h	\
../src/Str/Str.h	\
../src/Array/Array.h
	$(compile_source)

