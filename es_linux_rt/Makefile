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
PROJECT = prj_rt
CC = g++

# MOD
ifeq ($(CFG),Debug)
  OBJ_DIR = Debug
  OUTPUT_DIR = Debug
  TARGET = prj
  C_INCLUDE_DIRS = 
  C_PREPROC = -D_DEBUG -D__LINUX__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__  -DVERSION_RUNTIME 
  CFLAGS = -pipe -Wall -W -g2 -O0 -frtti -Wno-variadic-macros -pg # -pedantic 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS =
  LIBS = -lrt -lasound -lpthread
  LDFLAGS = -pipe -pg
endif

# MOD
ifeq ($(CFG),Release)
  OBJ_DIR = Release
  OUTPUT_DIR = Release
  TARGET = prj
  C_INCLUDE_DIRS = 
  C_PREPROC = -D_RELEASE -D__LINUX__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__  -DVERSION_RUNTIME 
  CFLAGS = -pipe -ffast-math -fno-builtin -W -g0 -O0 -fno-rtti -Wno-variadic-macros
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS =
  LIBS = -lrt -lasound -lpthread
  LDFLAGS = -pipe
endif

ifeq ($(OS),Windows_NT)
  NULL =
else
  NULL = nul
endif

SRC_OBJS = \
  $(OBJ_DIR)/DEF.o	\
  $(OBJ_DIR)/OrgOp.o	\
  $(OBJ_DIR)/Str.o	\
  $(OBJ_DIR)/Main.o	\
  $(OBJ_DIR)/List.o	\
  $(OBJ_DIR)/RtMidi.o	\
  $(OBJ_DIR)/Midi.o	\
  $(OBJ_DIR)/File.o	\
  $(OBJ_DIR)/FileStream.o	\
  $(OBJ_DIR)/FileStreamWav.o	\
  $(OBJ_DIR)/Op.o	\
  $(OBJ_DIR)/OpAMU.o	\
  $(OBJ_DIR)/OpAccum.o	\
  $(OBJ_DIR)/OpAdd.o	\
  $(OBJ_DIR)/OpBit.o	\
  $(OBJ_DIR)/OpBitSplit.o	\
  $(OBJ_DIR)/OpBuffer.o	\
  $(OBJ_DIR)/OpChorus.o	\
  $(OBJ_DIR)/OpClip.o	\
  $(OBJ_DIR)/OpCmp.o	\
  $(OBJ_DIR)/OpCombiner.o	\
  $(OBJ_DIR)/OpCompress.o	\
  $(OBJ_DIR)/OpConverter.o	\
  $(OBJ_DIR)/OpDEF.o	\
  $(OBJ_DIR)/OpDelay.o	\
  $(OBJ_DIR)/OpDemux.o	\
  $(OBJ_DIR)/OpDetect.o	\
  $(OBJ_DIR)/OpDistort.o	\
  $(OBJ_DIR)/OpDiv.o	\
  $(OBJ_DIR)/OpEnv1Bzr.o	\
  $(OBJ_DIR)/OpEnv1Exp.o	\
  $(OBJ_DIR)/OpEnv1Lin.o	\
  $(OBJ_DIR)/OpEnv1Sin.o	\
  $(OBJ_DIR)/OpEnv2Bzr.o	\
  $(OBJ_DIR)/OpEnv2Exp.o	\
  $(OBJ_DIR)/OpEnv2Lin.o	\
  $(OBJ_DIR)/OpEnvAdsr.o	\
  $(OBJ_DIR)/OpEnvGlider.o	\
  $(OBJ_DIR)/OpEventTable.o	\
  $(OBJ_DIR)/OpFader.o	\
  $(OBJ_DIR)/OpFlanger.o	\
  $(OBJ_DIR)/OpFltAllpass.o	\
  $(OBJ_DIR)/OpFltAllpass1.o	\
  $(OBJ_DIR)/OpFltAllpass2.o	\
  $(OBJ_DIR)/OpFltBiquad.o	\
  $(OBJ_DIR)/OpFltComb.o	\
  $(OBJ_DIR)/OpFltEq3.o	\
  $(OBJ_DIR)/OpFltFBack.o	\
  $(OBJ_DIR)/OpFltFFT.o	\
  $(OBJ_DIR)/OpFltFForw.o	\
  $(OBJ_DIR)/OpFltHighLow.o	\
  $(OBJ_DIR)/OpFltLow0.o	\
  $(OBJ_DIR)/OpFltMoog.o	\
  $(OBJ_DIR)/OpFltRes0.o	\
  $(OBJ_DIR)/OpFltTB303.o	\
  $(OBJ_DIR)/OpGate.o	\
  $(OBJ_DIR)/OpIf.o	\
  $(OBJ_DIR)/OpLadder.o	\
  $(OBJ_DIR)/OpLogic.o	\
  $(OBJ_DIR)/OpLoop.o	\
  $(OBJ_DIR)/OpMAD.o	\
  $(OBJ_DIR)/OpMathArcus.o	\
  $(OBJ_DIR)/OpMathExp.o	\
  $(OBJ_DIR)/OpMathFmod.o	\
  $(OBJ_DIR)/OpMathLog.o	\
  $(OBJ_DIR)/OpMathModf.o	\
  $(OBJ_DIR)/OpMathNum.o	\
  $(OBJ_DIR)/OpMathPow.o	\
  $(OBJ_DIR)/OpMathSqrt.o	\
  $(OBJ_DIR)/OpMathTrigo.o	\
  $(OBJ_DIR)/OpMidiDevIn.o	\
  $(OBJ_DIR)/OpMidiDevOut.o	\
  $(OBJ_DIR)/OpMidiIn.o	\
  $(OBJ_DIR)/OpMidiInKey.o	\
  $(OBJ_DIR)/OpMidiOut.o	\
  $(OBJ_DIR)/OpMix.o	\
  $(OBJ_DIR)/OpMul.o	\
  $(OBJ_DIR)/OpNop.o	\
  $(OBJ_DIR)/OpOscApprox.o	\
  $(OBJ_DIR)/OpOscCircle.o	\
  $(OBJ_DIR)/OpOscPls.o	\
  $(OBJ_DIR)/OpOscSaw.o	\
  $(OBJ_DIR)/OpOscSin.o	\
  $(OBJ_DIR)/OpOscTri.o	\
  $(OBJ_DIR)/OpOscVar.o	\
  $(OBJ_DIR)/OpPercDrum.o	\
  $(OBJ_DIR)/OpPhaser.o	\
  $(OBJ_DIR)/OpReducer.o	\
  $(OBJ_DIR)/OpRetrigger.o	\
  $(OBJ_DIR)/OpReverb.o	\
  $(OBJ_DIR)/OpRouter.o	\
  $(OBJ_DIR)/OpSH.o	\
  $(OBJ_DIR)/OpScale.o	\
  $(OBJ_DIR)/OpSelector.o	\
  $(OBJ_DIR)/OpSeqCell1D.o	\
  $(OBJ_DIR)/OpSeqColl1D.o	\
  $(OBJ_DIR)/OpSeqFibo.o	\
  $(OBJ_DIR)/OpSeqLog.o	\
  $(OBJ_DIR)/OpSeqMandel.o	\
  $(OBJ_DIR)/OpSeqPrime.o	\
  $(OBJ_DIR)/OpSeqRand.o	\
  $(OBJ_DIR)/OpSeqReca.o	\
  $(OBJ_DIR)/OpShaper.o	\
  $(OBJ_DIR)/OpStep.o	\
  $(OBJ_DIR)/OpSto.o	\
  $(OBJ_DIR)/OpSub.o	\
  $(OBJ_DIR)/OpSwitch.o	\
  $(OBJ_DIR)/OpSys.o	\
  $(OBJ_DIR)/OpTime.o	\
  $(OBJ_DIR)/OpTransFFT.o	\
  $(OBJ_DIR)/OpValue.o	\
  $(OBJ_DIR)/OpTkBuffer.o	\
  $(OBJ_DIR)/OpTkBufferDelay.o	\
  $(OBJ_DIR)/OpTkDEF.o	\
  $(OBJ_DIR)/OpTkEnv.o	\
  $(OBJ_DIR)/OpTkFFT.o	\
  $(OBJ_DIR)/OpTkFlt.o	\
  $(OBJ_DIR)/OpTkOsc.o	\
  $(OBJ_DIR)/OpTkSeq.o	\
  $(OBJ_DIR)/OpDiff.o	\
  $(OBJ_DIR)/OpEnv2Sin.o	\
  $(OBJ_DIR)/AS.o	\
  $(OBJ_DIR)/Sys.o	\
  $(OBJ_DIR)/FileBlock.o	\
  $(OBJ_DIR)/FileBlockMidi.o	\
  $(OBJ_DIR)/OpMidiFileIn.o	\
  $(OBJ_DIR)/OpInter.o	\
  $(OBJ_DIR)/OpTrigger.o	\
  $(OBJ_DIR)/OpFileWavIn.o	\
  $(OBJ_DIR)/OpFileWavOut.o	\
  $(OBJ_DIR)/OpReverbAL.o	\
  $(OBJ_DIR)/OpSampler.o	\
  $(OBJ_DIR)/OpCompressEx.o	\
  $(OBJ_DIR)/OpSeqAAS.o	\
  $(OBJ_DIR)/OpOscLUT.o

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
$(OBJ_DIR)/DEF.o: ../src/DEF.cpp	\
../src/DEF.h
	$(compile_source)

$(OBJ_DIR)/OrgOp.o: ../src/OrgOp/OrgOp.cpp	\
../src/OrgOp/OrgOp.h	\
../src/DEF.h	\
../src/List/List.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/OrgOp/OrgOp.inline.h	\
../src/Op/OpAccum.h	\
../src/Op/OpAdd.h	\
../src/Op/OpAMU.h	\
../src/Op/OpBit.h	\
../src/Op/OpBitSplit.h	\
../src/Op/OpBuffer.h	\
../src/Op/OpChorus.h	\
../src/Op/OpClip.h	\
../src/Op/OpCmp.h	\
../src/Op/OpCombiner.h	\
../src/Op/OpCompress.h	\
../src/Op/OpCompressEx.h	\
../src/Op/OpConverter.h	\
../src/Op/OpDelay.h	\
../src/Op/OpDemux.h	\
../src/Op/OpDetect.h	\
../src/Op/OpDiff.h	\
../src/Op/OpDistort.h	\
../src/Op/OpDiv.h	\
../src/Op/OpEnv1Bzr.h	\
../src/Op/OpEnv1Exp.h	\
../src/Op/OpEnv1Lin.h	\
../src/Op/OpEnv1Sin.h	\
../src/Op/OpEnv2Bzr.h	\
../src/Op/OpEnv2Exp.h	\
../src/Op/OpEnv2Lin.h	\
../src/Op/OpEnv2Sin.h	\
../src/Op/OpEnvAdsr.h	\
../src/Op/OpEnvGlider.h	\
../src/Op/OpEventTable.h	\
../src/Op/OpFader.h	\
../src/Op/OpFileWavIn.h	\
../src/Op/OpFileWavOut.h	\
../src/Op/OpFlanger.h	\
../src/Op/OpFltAllpass.h	\
../src/Op/OpFltAllpass1.h	\
../src/Op/OpFltAllpass2.h	\
../src/Op/OpFltBiquad.h	\
../src/Op/OpFltComb.h	\
../src/Op/OpFltEq3.h	\
../src/Op/OpFltFBack.h	\
../src/Op/OpFltFForw.h	\
../src/Op/OpFltFFT.h	\
../src/Op/OpFltHighLow.h	\
../src/Op/OpFltLow0.h	\
../src/Op/OpFltMoog.h	\
../src/Op/OpFltRes0.h	\
../src/Op/OpFltTB303.h	\
../src/Op/OpGate.h	\
../src/Op/OpIf.h	\
../src/Op/OpInter.h	\
../src/Op/OpLadder.h	\
../src/Op/OpLogic.h	\
../src/Op/OpLoop.h	\
../src/Op/OpMAD.h	\
../src/Op/OpMathArcus.h	\
../src/Op/OpMathExp.h	\
../src/Op/OpMathFmod.h	\
../src/Op/OpMathLog.h	\
../src/Op/OpMathModf.h	\
../src/Op/OpMathNum.h	\
../src/Op/OpMathPow.h	\
../src/Op/OpMathSqrt.h	\
../src/Op/OpMathTrigo.h	\
../src/Op/OpMidiDevIn.h	\
../src/Op/OpMidiDevOut.h	\
../src/Op/OpMidiFileIn.h	\
../src/Op/OpMidiIn.h	\
../src/Op/OpMidiInKey.h	\
../src/Op/OpMidiOut.h	\
../src/Op/OpMix.h	\
../src/Op/OpMul.h	\
../src/Op/OpNop.h	\
../src/Op/OpOscApprox.h	\
../src/Op/OpOscCircle.h	\
../src/Op/OpOscLUT.h	\
../src/Op/OpOscPls.h	\
../src/Op/OpOscSaw.h	\
../src/Op/OpOscSin.h	\
../src/Op/OpOscTri.h	\
../src/Op/OpOscVar.h	\
../src/Op/OpPercDrum.h	\
../src/Op/OpPhaser.h	\
../src/Op/OpReducer.h	\
../src/Op/OpRetrigger.h	\
../src/Op/OpReverb.h	\
../src/Op/OpReverbAL.h	\
../src/Op/OpRouter.h	\
../src/Op/OpSampler.h	\
../src/Op/OpScale.h	\
../src/Op/OpSelector.h	\
../src/Op/OpSeqAAS.h	\
../src/Op/OpSeqCell1D.h	\
../src/Op/OpSeqColl1D.h	\
../src/Op/OpSeqFibo.h	\
../src/Op/OpSeqLog.h	\
../src/Op/OpSeqMandel.h	\
../src/Op/OpSeqPrime.h	\
../src/Op/OpSeqRand.h	\
../src/Op/OpSeqReca.h	\
../src/Op/OpSH.h	\
../src/Op/OpShaper.h	\
../src/Op/OpStep.h	\
../src/Op/OpSto.h	\
../src/Op/OpSub.h	\
../src/Op/OpSwitch.h	\
../src/Op/OpSys.h	\
../src/Op/OpTime.h	\
../src/Op/OpTransFFT.h	\
../src/Op/OpTrigger.h	\
../src/Op/OpValue.h
	$(compile_source)

$(OBJ_DIR)/Str.o: ../src/Str/Str.cpp	\
../src/Str/Str.h	\
../src/File/File.h	\
../src/Str/Str.inline.h
	$(compile_source)

$(OBJ_DIR)/Main.o: ../src/Main.cpp	\
../src/Main.h	\
../src/DEF.h	\
../src/Ctrl.h	\
../src/AS/AS.h	\
../src/Str/Str.h	\
../src/File/File.h	\
../src/Str/Str.inline.h	\
../src/Wnd/Wnd.h	\
../src/File/File.h	\
../src/File/FileStreamWav.h	\
../src/File/FileStream.h	\
../src/Sys/Sys.h	\
../src/Str/Str.h	\
../src/Array/Array.h	\
../src/Op/OpNop.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Op/OpOscSin.h	\
../src/OpTk/OpTkOsc.h	\
../src/Op/OpValue.h
	$(compile_source)

$(OBJ_DIR)/List.o: ../src/List/List.cpp	\
../src/List/List.h	\
../src/List/List.inline.h
	$(compile_source)

# MOD
$(OBJ_DIR)/RtMidi.o: ../src/Midi/RtMidi/RtMidi.cpp	\
../src/Midi/RtMidi/RtMidi.h	\
../src/Midi/RtMidi/RtError.h
	$(compile_source)

$(OBJ_DIR)/Midi.o: ../src/Midi/Midi.cpp	\
../src/Midi/Midi.h	\
../src/List/List.h	\
../src/Str/Str.h	\
../src/File/FileBlockMidi.h	\
../src/Midi/RtMidi/RtMidi.h	\
../src/Midi/Midi.inline.h
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

$(OBJ_DIR)/Op.o: ../src/Op/Op.cpp	\
../src/Op/Op.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpAMU.o: ../src/Op/OpAMU.cpp	\
../src/Op/OpAMU.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpAccum.o: ../src/Op/OpAccum.cpp	\
../src/Op/OpAccum.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpAdd.o: ../src/Op/OpAdd.cpp	\
../src/Op/OpAdd.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpBit.o: ../src/Op/OpBit.cpp	\
../src/Op/OpBit.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Op/OpBit.inline.h
	$(compile_source)

$(OBJ_DIR)/OpBitSplit.o: ../src/Op/OpBitSplit.cpp	\
../src/Op/OpBitSplit.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpBuffer.o: ../src/Op/OpBuffer.cpp	\
../src/Op/OpBuffer.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkBuffer.h
	$(compile_source)

$(OBJ_DIR)/OpChorus.o: ../src/Op/OpChorus.cpp	\
../src/Op/OpChorus.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkBufferDelay.h
	$(compile_source)

$(OBJ_DIR)/OpClip.o: ../src/Op/OpClip.cpp	\
../src/Op/OpClip.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpCmp.o: ../src/Op/OpCmp.cpp	\
../src/Op/OpCmp.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Op/OpCmp.inline.h
	$(compile_source)

$(OBJ_DIR)/OpCombiner.o: ../src/Op/OpCombiner.cpp	\
../src/Op/OpCombiner.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpCompress.o: ../src/Op/OpCompress.cpp	\
../src/Op/OpCompress.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkBufferDelay.h
	$(compile_source)

$(OBJ_DIR)/OpConverter.o: ../src/Op/OpConverter.cpp	\
../src/Op/OpConverter.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Midi/Midi.h
	$(compile_source)

$(OBJ_DIR)/OpDEF.o: ../src/Op/OpDEF.cpp	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h
	$(compile_source)

$(OBJ_DIR)/OpDelay.o: ../src/Op/OpDelay.cpp	\
../src/Op/OpDelay.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkBufferDelay.h
	$(compile_source)

$(OBJ_DIR)/OpDemux.o: ../src/Op/OpDemux.cpp	\
../src/Op/OpDemux.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpDetect.o: ../src/Op/OpDetect.cpp	\
../src/Op/OpDetect.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpDistort.o: ../src/Op/OpDistort.cpp	\
../src/Op/OpDistort.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpDiv.o: ../src/Op/OpDiv.cpp	\
../src/Op/OpDiv.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpEnv1Bzr.o: ../src/Op/OpEnv1Bzr.cpp	\
../src/Op/OpEnv1Bzr.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEnv1Exp.o: ../src/Op/OpEnv1Exp.cpp	\
../src/Op/OpEnv1Exp.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEnv1Lin.o: ../src/Op/OpEnv1Lin.cpp	\
../src/Op/OpEnv1Lin.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEnv1Sin.o: ../src/Op/OpEnv1Sin.cpp	\
../src/Op/OpEnv1Sin.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEnv2Bzr.o: ../src/Op/OpEnv2Bzr.cpp	\
../src/Op/OpEnv2Bzr.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEnv2Exp.o: ../src/Op/OpEnv2Exp.cpp	\
../src/Op/OpEnv2Exp.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEnv2Lin.o: ../src/Op/OpEnv2Lin.cpp	\
../src/Op/OpEnv2Lin.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEnvAdsr.o: ../src/Op/OpEnvAdsr.cpp	\
../src/Op/OpEnvAdsr.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEnvGlider.o: ../src/Op/OpEnvGlider.cpp	\
../src/Op/OpEnvGlider.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/OpEventTable.o: ../src/Op/OpEventTable.cpp	\
../src/Op/OpEventTable.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpFader.o: ../src/Op/OpFader.cpp	\
../src/Op/OpFader.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpFlanger.o: ../src/Op/OpFlanger.cpp	\
../src/Op/OpFlanger.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkOsc.h	\
../src/OpTk/OpTkBufferDelay.h
	$(compile_source)

$(OBJ_DIR)/OpFltAllpass.o: ../src/Op/OpFltAllpass.cpp	\
../src/Op/OpFltAllpass.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkBufferDelay.h
	$(compile_source)

$(OBJ_DIR)/OpFltAllpass1.o: ../src/Op/OpFltAllpass1.cpp	\
../src/Op/OpFltAllpass1.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpFltAllpass2.o: ../src/Op/OpFltAllpass2.cpp	\
../src/Op/OpFltAllpass2.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpFltBiquad.o: ../src/Op/OpFltBiquad.cpp	\
../src/Op/OpFltBiquad.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpFltComb.o: ../src/Op/OpFltComb.cpp	\
../src/Op/OpFltComb.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkBufferDelay.h
	$(compile_source)

$(OBJ_DIR)/OpFltEq3.o: ../src/Op/OpFltEq3.cpp	\
../src/Op/OpFltEq3.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpFltFBack.o: ../src/Op/OpFltFBack.cpp	\
../src/Op/OpFltFBack.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpFltFFT.o: ../src/Op/OpFltFFT.cpp	\
../src/Op/OpFltFFT.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFFT.h
	$(compile_source)

$(OBJ_DIR)/OpFltFForw.o: ../src/Op/OpFltFForw.cpp	\
../src/Op/OpFltFForw.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpFltHighLow.o: ../src/Op/OpFltHighLow.cpp	\
../src/Op/OpFltHighLow.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpFltLow0.o: ../src/Op/OpFltLow0.cpp	\
../src/Op/OpFltLow0.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpFltMoog.o: ../src/Op/OpFltMoog.cpp	\
../src/Op/OpFltMoog.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpFltRes0.o: ../src/Op/OpFltRes0.cpp	\
../src/Op/OpFltRes0.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpFltTB303.o: ../src/Op/OpFltTB303.cpp	\
../src/Op/OpFltTB303.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpGate.o: ../src/Op/OpGate.cpp	\
../src/Op/OpGate.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h	\
../src/OpTk/OpTkBufferDelay.h
	$(compile_source)

$(OBJ_DIR)/OpIf.o: ../src/Op/OpIf.cpp	\
../src/Op/OpIf.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpLadder.o: ../src/Op/OpLadder.cpp	\
../src/Op/OpLadder.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpLogic.o: ../src/Op/OpLogic.cpp	\
../src/Op/OpLogic.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Op/OpLogic.inline.h
	$(compile_source)

$(OBJ_DIR)/OpLoop.o: ../src/Op/OpLoop.cpp	\
../src/Op/OpLoop.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMAD.o: ../src/Op/OpMAD.cpp	\
../src/Op/OpMAD.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathArcus.o: ../src/Op/OpMathArcus.cpp	\
../src/Op/OpMathArcus.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathExp.o: ../src/Op/OpMathExp.cpp	\
../src/Op/OpMathExp.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathFmod.o: ../src/Op/OpMathFmod.cpp	\
../src/Op/OpMathFmod.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathLog.o: ../src/Op/OpMathLog.cpp	\
../src/Op/OpMathLog.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathModf.o: ../src/Op/OpMathModf.cpp	\
../src/Op/OpMathModf.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathNum.o: ../src/Op/OpMathNum.cpp	\
../src/Op/OpMathNum.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathPow.o: ../src/Op/OpMathPow.cpp	\
../src/Op/OpMathPow.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathSqrt.o: ../src/Op/OpMathSqrt.cpp	\
../src/Op/OpMathSqrt.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMathTrigo.o: ../src/Op/OpMathTrigo.cpp	\
../src/Op/OpMathTrigo.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMidiDevIn.o: ../src/Op/OpMidiDevIn.cpp	\
../src/Op/OpMidiDevIn.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Midi/Midi.h
	$(compile_source)

$(OBJ_DIR)/OpMidiDevOut.o: ../src/Op/OpMidiDevOut.cpp	\
../src/Op/OpMidiDevOut.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Midi/Midi.h
	$(compile_source)

$(OBJ_DIR)/OpMidiIn.o: ../src/Op/OpMidiIn.cpp	\
../src/Op/OpMidiIn.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Op/OpMidiDevIn.h	\
../src/Midi/Midi.h	\
../src/Op/OpMidiFileIn.h	\
../src/File/FileBlockMidi.h
	$(compile_source)

$(OBJ_DIR)/OpMidiInKey.o: ../src/Op/OpMidiInKey.cpp	\
../src/Op/OpMidiInKey.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Op/OpMidiDevIn.h	\
../src/Midi/Midi.h	\
../src/Op/OpMidiFileIn.h	\
../src/File/FileBlockMidi.h
	$(compile_source)

$(OBJ_DIR)/OpMidiOut.o: ../src/Op/OpMidiOut.cpp	\
../src/Op/OpMidiOut.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Op/OpMidiDevOut.h	\
../src/Midi/Midi.h
	$(compile_source)

$(OBJ_DIR)/OpMix.o: ../src/Op/OpMix.cpp	\
../src/Op/OpMix.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpMul.o: ../src/Op/OpMul.cpp	\
../src/Op/OpMul.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpNop.o: ../src/Op/OpNop.cpp	\
../src/Op/OpNop.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpOscApprox.o: ../src/Op/OpOscApprox.cpp	\
../src/Op/OpOscApprox.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkOsc.h
	$(compile_source)

$(OBJ_DIR)/OpOscCircle.o: ../src/Op/OpOscCircle.cpp	\
../src/Op/OpOscCircle.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpOscPls.o: ../src/Op/OpOscPls.cpp	\
../src/Op/OpOscPls.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpOscSaw.o: ../src/Op/OpOscSaw.cpp	\
../src/Op/OpOscSaw.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpOscSin.o: ../src/Op/OpOscSin.cpp	\
../src/Op/OpOscSin.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkOsc.h
	$(compile_source)

$(OBJ_DIR)/OpOscTri.o: ../src/Op/OpOscTri.cpp	\
../src/Op/OpOscTri.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpOscVar.o: ../src/Op/OpOscVar.cpp	\
../src/Op/OpOscVar.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkOsc.h
	$(compile_source)

$(OBJ_DIR)/OpPercDrum.o: ../src/Op/OpPercDrum.cpp	\
../src/Op/OpPercDrum.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h	\
../src/OpTk/OpTkFlt.h	\
../src/OpTk/OpTkOsc.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpPhaser.o: ../src/Op/OpPhaser.cpp	\
../src/Op/OpPhaser.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFlt.h
	$(compile_source)

$(OBJ_DIR)/OpReducer.o: ../src/Op/OpReducer.cpp	\
../src/Op/OpReducer.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpRetrigger.o: ../src/Op/OpRetrigger.cpp	\
../src/Op/OpRetrigger.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpReverb.o: ../src/Op/OpReverb.cpp	\
../src/Op/OpReverb.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkBufferDelay.h
	$(compile_source)

$(OBJ_DIR)/OpRouter.o: ../src/Op/OpRouter.cpp	\
../src/Op/OpRouter.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpSH.o: ../src/Op/OpSH.cpp	\
../src/Op/OpSH.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpScale.o: ../src/Op/OpScale.cpp	\
../src/Op/OpScale.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpSelector.o: ../src/Op/OpSelector.cpp	\
../src/Op/OpSelector.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpSeqCell1D.o: ../src/Op/OpSeqCell1D.cpp	\
../src/Op/OpSeqCell1D.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpSeqColl1D.o: ../src/Op/OpSeqColl1D.cpp	\
../src/Op/OpSeqColl1D.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpSeqFibo.o: ../src/Op/OpSeqFibo.cpp	\
../src/Op/OpSeqFibo.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpSeqLog.o: ../src/Op/OpSeqLog.cpp	\
../src/Op/OpSeqLog.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpSeqMandel.o: ../src/Op/OpSeqMandel.cpp	\
../src/Op/OpSeqMandel.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpSeqPrime.o: ../src/Op/OpSeqPrime.cpp	\
../src/Op/OpSeqPrime.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpSeqRand.o: ../src/Op/OpSeqRand.cpp	\
../src/Op/OpSeqRand.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpSeqReca.o: ../src/Op/OpSeqReca.cpp	\
../src/Op/OpSeqReca.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpShaper.o: ../src/Op/OpShaper.cpp	\
../src/Op/OpShaper.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpStep.o: ../src/Op/OpStep.cpp	\
../src/Op/OpStep.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpSto.o: ../src/Op/OpSto.cpp	\
../src/Op/OpSto.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpSub.o: ../src/Op/OpSub.cpp	\
../src/Op/OpSub.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpSwitch.o: ../src/Op/OpSwitch.cpp	\
../src/Op/OpSwitch.h	\
../src/Op/OpDEF.h	\
../src/Op/Op.h	\
../src/Array/Array.h
	$(compile_source)

$(OBJ_DIR)/OpSys.o: ../src/Op/OpSys.cpp	\
../src/Op/OpSys.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTime.o: ../src/Op/OpTime.cpp	\
../src/Op/OpTime.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTransFFT.o: ../src/Op/OpTransFFT.cpp	\
../src/Op/OpTransFFT.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkFFT.h
	$(compile_source)

$(OBJ_DIR)/OpValue.o: ../src/Op/OpValue.cpp	\
../src/Op/OpValue.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTkBuffer.o: ../src/OpTk/OpTkBuffer.cpp	\
../src/OpTk/OpTkBuffer.h	\
../src/OpTk/OpTkDEF.h	\
../src/Op/OpDEF.h	\
../src/OpTk/OpTkBuffer.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTkBufferDelay.o: ../src/OpTk/OpTkBufferDelay.cpp	\
../src/OpTk/OpTkBufferDelay.h	\
../src/OpTk/OpTkDEF.h	\
../src/Op/OpDEF.h	\
../src/OpTk/OpTkBuffer.h	\
../src/OpTk/OpTkBuffer.inline.h	\
../src/OpTk/OpTkBufferDelay.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTkDEF.o: ../src/OpTk/OpTkDEF.cpp	\
../src/OpTk/OpTkDEF.h	\
../src/Op/OpDEF.h
	$(compile_source)

$(OBJ_DIR)/OpTkEnv.o: ../src/OpTk/OpTkEnv.cpp	\
../src/OpTk/OpTkEnv.h	\
../src/OpTk/OpTkDEF.h	\
../src/Op/OpDEF.h	\
../src/OpTk/OpTkEnv.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTkFFT.o: ../src/OpTk/OpTkFFT.cpp

	$(compile_source)

$(OBJ_DIR)/OpTkFlt.o: ../src/OpTk/OpTkFlt.cpp	\
../src/OpTk/OpTkFlt.h	\
../src/OpTk/OpTkDEF.h	\
../src/Op/OpDEF.h	\
../src/OpTk/OpTkFlt.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTkOsc.o: ../src/OpTk/OpTkOsc.cpp	\
../src/OpTk/OpTkOsc.h	\
../src/OpTk/OpTkDEF.h	\
../src/Op/OpDEF.h	\
../src/OpTk/OpTkOsc.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTkSeq.o: ../src/OpTk/OpTkSeq.cpp	\
../src/OpTk/OpTkSeq.h	\
../src/OpTk/OpTkDEF.h	\
../src/Op/OpDEF.h	\
../src/OpTk/OpTkSeq.inline.h
	$(compile_source)

$(OBJ_DIR)/OpDiff.o: ../src/Op/OpDiff.cpp	\
../src/Op/OpDiff.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpEnv2Sin.o: ../src/Op/OpEnv2Sin.cpp	\
../src/Op/OpEnv2Sin.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkEnv.h
	$(compile_source)

$(OBJ_DIR)/AS.o: ../src/AS/AS.cpp	\
../src/AS/AS.h
	$(compile_source)

$(OBJ_DIR)/Sys.o: ../src/Sys/Sys.cpp	\
../src/Sys/Sys.h	\
../src/Str/Str.h	\
../src/Array/Array.h
	$(compile_source)

$(OBJ_DIR)/FileBlock.o: ../src/File/FileBlock.cpp	\
../src/File/FileBlock.h	\
../src/File/File.h
	$(compile_source)

$(OBJ_DIR)/FileBlockMidi.o: ../src/File/FileBlockMidi.cpp	\
../src/File/FileBlockMidi.h	\
../src/File/FileBlock.h	\
../src/File/File.h	\
../src/Array/Array.h	\
../src/List/List.h
	$(compile_source)

$(OBJ_DIR)/OpMidiFileIn.o: ../src/Op/OpMidiFileIn.cpp	\
../src/Op/OpMidiFileIn.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Midi/Midi.h	\
../src/File/FileBlockMidi.h
	$(compile_source)

$(OBJ_DIR)/OpInter.o: ../src/Op/OpInter.cpp	\
../src/Op/OpInter.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpTrigger.o: ../src/Op/OpTrigger.cpp	\
../src/Op/OpTrigger.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpFileWavIn.o: ../src/Op/OpFileWavIn.cpp	\
../src/Op/OpFileWavIn.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/File/FileStreamWav.h	\
../src/OpTk/OpTkBuffer.h
	$(compile_source)

$(OBJ_DIR)/OpFileWavOut.o: ../src/Op/OpFileWavOut.cpp	\
../src/Op/OpFileWavOut.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Time/Time.h	\
../src/File/FileStreamWav.h
	$(compile_source)

$(OBJ_DIR)/OpReverbAL.o: ../src/Op/OpReverbAL.cpp	\
../src/Op/OpReverbAL.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h
	$(compile_source)

$(OBJ_DIR)/OpSampler.o: ../src/Op/OpSampler.cpp	\
../src/Op/OpSampler.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/File/FileStreamWav.h	\
../src/OpTk/OpTkBuffer.h
	$(compile_source)

$(OBJ_DIR)/OpCompressEx.o: ../src/Op/OpCompressEx.cpp	\
../src/Op/OpCompressEx.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/Op/OpCompressEx.inline.h
	$(compile_source)

$(OBJ_DIR)/OpSeqAAS.o: ../src/Op/OpSeqAAS.cpp	\
../src/Op/OpSeqAAS.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkSeq.h
	$(compile_source)

$(OBJ_DIR)/OpOscLUT.o: ../src/Op/OpOscLUT.cpp	\
../src/Op/OpOscLUT.h	\
../src/Op/OpDEF.h	\
../src/DEF.h	\
../src/Array/Array.h	\
../src/Str/Str.h	\
../src/Op/Op.h	\
../src/Op/Op.inline.h	\
../src/OpTk/OpTkOsc.h
	$(compile_source)
