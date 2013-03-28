// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// es.cpp

#include "es.h"
#include "../src/Str/Str.h"
#include "../src/OrgOp/OrgOp.h"
#include "../src/Op/OpInter.h"

static COrgOp l_oOrgOp_;

const char * esVersion()
{
	return APP_NAME " compile time: " APP_DATE " " APP_TIME " (lib version " _ES_VERSION_ ")";
}

bool esLoad( const char *pcData )
{
	CStr oData( pcData );
	return l_oOrgOp_.LoadAll( oData );
}

void esReset()
{
	l_oOrgOp_.ResetAll();
	l_oOrgOp_.UpdateAll();
}

bool esProc( double *pdL, double *pdR )
{
	COp::Prepare_();
	COrgOp::Proc_( pdL, pdR );
	return ( COp::m_uiSignal_ > 0 );
}

void esInter( double **ppdIn, unsigned int *puiInCount, double **ppdOut, unsigned int *puiOutCount )
{
	*ppdIn = COpInter::m_pdBufferIn_;
	*ppdOut = COpInter::m_pdBufferOut_;
	*puiInCount = COpInter::m_uiBufferInCount_;
	*puiOutCount = COpInter::m_uiBufferOutCount_;
}
