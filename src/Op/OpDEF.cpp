// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpDEF.cpp

#include "OpDEF.h"

#ifdef OP_USE_LOG

#include <stdio.h>

static char g_acLog_[ OP_LOG_LINES ][ OP_LOG_CHARS ];
static unsigned int g_uiLogIndex_ = 0;

void _Op_LogClear()
{
	g_uiLogIndex_ = 0;
	for( unsigned int i=0; i<OP_LOG_LINES; ++i )
		g_acLog_[i][0] = '\0';
}

const char * _Op_LogGet( unsigned int uiLine )
{
	unsigned int i = g_uiLogIndex_ + uiLine;
	if( i >= OP_LOG_LINES )
		i %= OP_LOG_LINES;
	return g_acLog_[i];
}

void _Op_LogFlt( const char *pcMsg, double dFlt )
{
	if( !g_uiLogIndex_ )
		g_uiLogIndex_ = OP_LOG_LINES;
	--g_uiLogIndex_;
#ifdef _MSC_VER
	sprintf_s
#else	
	sprintf
#endif // _MSC_VER
		( g_acLog_[g_uiLogIndex_], 
#ifdef _MSC_VER
		OP_LOG_CHARS, 
#endif // _MSC_VER
		"%s: %g\n", pcMsg, dFlt );
	printf( "%s: %g\n", pcMsg, dFlt );
	
}

#endif // USE_LOG
