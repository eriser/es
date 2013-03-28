// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTransFFT.h

#ifndef _OP_TRANS_FFT_H_
#define _OP_TRANS_FFT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_TRANS_FFT

#include "../OpTk/OpTkFFT.h"

// FFT Transformer
////////////////////////////////////////////////////////////////////////////////
class COpTransFFT : public COp
{
public:
	COpTransFFT();
	COpTransFFT( const COpTransFFT &rO );
	virtual ~COpTransFFT();
	COpTransFFT & operator = ( const COpTransFFT &roOp );
protected:
	void Init();
	void ClearBuffers();
public:	
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	virtual void Validate();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	//OP_GENERIC_COPY_CTOR_DEC( COpTransFFT )
	OP_GENERIC_METHODS_DEC( COpTransFFT )
	
protected:
	COpTkFFT<double> m_oFFT;
	unsigned int m_uiBufferSize;
	unsigned int m_uiBufferPos;
	unsigned int m_uiFuncNum;
	double *m_pdBufferSrc;
	double *m_pdBufferDst;
};

#endif // OP_INC_OP_TRANS_FFT
#endif // _OP_TRANS_FFT_H_
