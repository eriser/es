// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSwitch.h

#ifndef _OP_SWITCH_H_
#define _OP_SWITCH_H_

#include "../Op/OpDEF.h"
#include "../Op/Op.h"

#ifdef OP_INC_OP_SWITCH

#include "../Array/Array.h"

// Switch
////////////////////////////////////////////////////////////////////////////////
class COpSwitch : public COp
{
protected:
	struct SVoice
	{
		SVoice();
		void Reset( unsigned int uiStamp );
		unsigned int m_uiStamp;
		int m_iIndex;
	};
public:
	COpSwitch();
	virtual ~COpSwitch();
protected:
	void Init();
	unsigned int GetVoice();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpSwitch )
	OP_GENERIC_METHODS_DEC( COpSwitch )
	
protected:
	CArray<SVoice> m_oArrVoice;
	unsigned int m_uiCountVal;
};

#endif // OP_INC_OP_SWITCH
#endif // _OP_SWITCH_H_
