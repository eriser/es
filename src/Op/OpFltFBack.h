// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltFBack.h

#ifndef _OP_FLT_F_BACK_H_
#define _OP_FLT_F_BACK_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_F_BACK

// Generic Feedback Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltFBack : public COp
{
public:
	COpFltFBack();
	virtual ~COpFltFBack();
	COpFltFBack & operator = ( const COpFltFBack & roOp );
protected:
	void Init();
	void ClearBuffer();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltFBack )
	OP_GENERIC_METHODS_DEC( COpFltFBack )
protected:
	double *m_pdBuffer;
	unsigned int m_uiBufferSize;
	unsigned int m_uiBufferPos;
};

#endif // OP_INC_OP_FLT_F_BACK
#endif // _OP_FLT_F_BACK_H_
