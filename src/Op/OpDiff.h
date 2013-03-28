// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpAdd.h

#ifndef OpDiff
#define OpDiff

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_DIFF

// Difference
////////////////////////////////////////////////////////////////////////////////
class COpDiff : public COp
{
public:
	COpDiff();
	virtual ~COpDiff();
protected:
	void Init();
public:
	virtual void Proc();
	virtual void Reset();
	OP_GENERIC_COPY_CTOR_DEC( COpDiff )
	OP_GENERIC_METHODS_DEC( COpDiff )

protected:
	double m_dLast;
};

#endif // OP_INC_OP_DIFF
#endif // OpDiff
