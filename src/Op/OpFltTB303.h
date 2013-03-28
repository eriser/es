// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFltTB303.h

#ifndef _OP_FLT_TB303_H_
#define _OP_FLT_TB303_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_FLT_TB303

#include "../OpTk/OpTkFlt.h"

// TB303 Filter
////////////////////////////////////////////////////////////////////////////////
class COpFltTB303 : public COp
{
public:
	COpFltTB303();
	virtual ~COpFltTB303();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpFltTB303 )
	OP_GENERIC_METHODS_DEC( COpFltTB303 )
protected:
	COpTkFltTB303<double, OP_SAMPLE_FREQ>  m_oFlt;
	double m_dCutoff, m_dRes;
};

#endif // OP_INC_OP_FLT_TB303
#endif // _OP_FLT_TB303_H_
