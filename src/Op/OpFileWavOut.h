// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFileWavOut.h

#ifndef _OP_FILE_WAV_OUT_H_
#define _OP_FILE_WAV_OUT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FILE_WAV_OUT

#include "../Time/Time.h"
#include "../File/FileStreamWav.h"

// Wave Output File
////////////////////////////////////////////////////////////////////////////////
class COpFileWavOut : public COp
{
public:
	COpFileWavOut();
	virtual ~COpFileWavOut();
public:
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpFileWavOut )
	OP_GENERIC_METHODS_DEC( COpFileWavOut )

protected:
	bool NewFile();
protected:
	unsigned m_uiIsOpen;
	CFileStreamWav m_oFileOut;
#ifdef OP_USE_RUNTIME_INFO
	CTime m_oTime;
#endif // OP_USE_RUNTIME_INFO
};

#endif // OP_INC_OP_FILE_WAV_OUT
#endif // _OP_FILE_WAV_OUT_H_
