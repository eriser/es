// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDetect.h

#ifndef _OP_DETECT_H_
#define _OP_DETECT_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_DETECT

// Detector
////////////////////////////////////////////////////////////////////////////////
class COpDetect : public COp
{
public:
	COpDetect();
	virtual ~COpDetect();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpDetect )
	OP_GENERIC_METHODS_DEC( COpDetect )
public:
	double m_dL0;
	double m_dL1;
};

#endif // OP_INC_OP_DETECT
#endif // _OP_DETECT_H_
