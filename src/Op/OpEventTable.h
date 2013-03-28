// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEventTable.h

#ifndef _OP_EVENT_TABLE_H_
#define _OP_EVENT_TABLE_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_EVENT_TABLE

// Event Table
////////////////////////////////////////////////////////////////////////////////
class COpEventTable : public COp
{
public:
	COpEventTable();
	virtual ~COpEventTable();
public:	
	virtual void Proc();
	virtual void Update();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpEventTable )
	OP_GENERIC_METHODS_DEC( COpEventTable )
};

#endif // OP_INC_OP_EVENT_TABLE
#endif // _OP_EVENT_TABLE_H_
