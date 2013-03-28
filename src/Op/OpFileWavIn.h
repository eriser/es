// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFileWavIn.h

#ifndef _OP_FILE_WAV_IN_H_
#define _OP_FILE_WAV_IN_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FILE_WAV_IN

#include "../File/FileStreamWav.h"
#include "../OpTk/OpTkBuffer.h"

// Wave Input File
////////////////////////////////////////////////////////////////////////////////
class COpFileWavIn : public COp
{
public:
	COpFileWavIn();
	virtual ~COpFileWavIn();
	COpFileWavIn & operator = ( const COpFileWavIn &roO );
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpFileWavIn )
	OP_GENERIC_METHODS_DEC( COpFileWavIn )
protected:
	COpTkBufferVarInterpol<double> m_oBuffer;
	unsigned int m_uiFileNameLen;
	unsigned int m_auiFileName[64];
};

#endif // OP_INC_OP_FILE_WAV_IN
#endif // _OP_FILE_WAV_IN_H_
