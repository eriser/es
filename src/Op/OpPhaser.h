// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpPhaser.h

#ifndef _OP_PHASER_H_
#define _OP_PHASER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_PHASER

#include "../OpTk/OpTkFlt.h"
#include "../Array/Array.h"

// Phaser
////////////////////////////////////////////////////////////////////////////////
class COpPhaser : public COp
{
public:
	COpPhaser();
	virtual ~COpPhaser();
protected:
	void Init();
	inline void ClearFeed();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpPhaser )
	OP_GENERIC_METHODS_DEC( COpPhaser )
	
protected:
	CArray< COpTkFltAllpass1<double> > m_oArrFlt;
	double m_dStages;
	double m_dLast;
	unsigned int m_uiStages;
};

#endif // OP_INC_OP_PHASER
#endif // _OP_PHASER_H_
