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
  C_PREPROC = -D_DEBUG -D__LINUX__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__
  CFLAGS = -pipe -Wall -W -g2 -O0 -frtti -Wno-variadic-macros -pg # -pedantic 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = -L"../bin/lib" 
  LIBS = -lrt -lasound -lpthread -les
  LDFLAGS = -pipe -pg
endif

# MOD
ifeq ($(CFG),Release)
  OBJ_DIR = Release
  OUTPUT_DIR = Release
  TARGET = prj
  C_INCLUDE_DIRS = 
  C_PREPROC = -D_RELEASE -D__LINUX__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__
  CFLAGS = -pipe -ffast-math -fno-builtin -W -g0 -O0 -fno-rtti -Wno-variadic-macros
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = -L"../bin/lib" 
  LIBS = -lrt -lasound -lpthread -les
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
	@echo ----------Configuration: eswav - $(CFG)----------

# MOD
directories:
	-@if [ ! -d "$(OUTPUT_DIR)" ]; then mkdir "$(OUTPUT_DIR)"; fi
	-@if [ ! -d "$(OBJ_DIR)" ]; then mkdir "$(OBJ_DIR)"; fi


# MOD
$(OBJ_DIR)/DEF.o: ../src_eswav/DEF.cpp

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

