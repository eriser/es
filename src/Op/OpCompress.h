// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCompress.h

#ifndef _OP_COMPRESS_H_
#define _OP_COMPRESS_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_COMPRESS

#include "../OpTk/OpTkBufferDelay.h"

#define OP_CMP_BUFFER_SIZE	( AUDIO_SAMPLE_FREQ / 5000 )

// Compressor
// ref: http://www.musicdsp.org/showArchiveComment.php?ArchiveID=169
////////////////////////////////////////////////////////////////////////////////
class COpCompress : public COp
{
public:
	COpCompress();
	virtual ~COpCompress();
protected:
	void Init();
	void SetAttRel( double &rdDst, double dSrc );
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpCompress )
	OP_GENERIC_METHODS_DEC( COpCompress )

protected:
	COpTkBufferFixedDelay<double, OP_CMP_BUFFER_SIZE> m_oDelay;
	double m_dSqSum;
	double m_dEnv;
	double m_dA, m_dR;
	double m_dAtt, m_dRel;
};

#endif // OP_INC_OP_COMPRESS
#endif // _OP_COMPRESS_H_
