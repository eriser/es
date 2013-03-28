// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltFFT.h

#ifndef _OP_FLT_FFT_H_
#define _OP_FLT_FFT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_FFT

#include "../OpTk/OpTkFFT.h"

// FFT Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltFFT : public COp
{
public:
	COpFltFFT();
	COpFltFFT( const COpFltFFT &roO );
	virtual ~COpFltFFT();
	COpFltFFT & operator = ( const COpFltFFT &roO );
protected:
	void Init();
	void ClearBuffers();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	//OP_GENERIC_COPY_CTOR_DEC( COpFltFFT )
	OP_GENERIC_METHODS_DEC( COpFltFFT )
protected:
	COpTkFFT<double> m_oFFT;
	unsigned int m_uiBufferSize;
	unsigned int m_uiBufferPos;
	double *m_pdBufferCache;
	double *m_pdBufferTmp;
	double *m_pdBufferOut;
};

#endif // OP_INC_OP_FLT_FFT
#endif // _OP_FLT_FFT_H_
