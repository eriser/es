// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpLogic.inline.h

#include "OpLogic.h"

////////////////////////////////////////////////////////////////////////////////
// COpLogic
////////////////////////////////////////////////////////////////////////////////

inline bool COpLogic::FuncOR_( bool bX, bool bY ) {
	return bX || bY;
}

inline bool COpLogic::FuncAND_( bool bX, bool bY ) {
	return bX && bY;
}

inline bool COpLogic::FuncNOT_( bool bX, bool bY ) {
	UNUSED_PARAMETER( bY );
	return !bX;
}
