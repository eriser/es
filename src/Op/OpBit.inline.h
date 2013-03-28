// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpBit.inline.h

////////////////////////////////////////////////////////////////////////////////
// COpBit
////////////////////////////////////////////////////////////////////////////////

inline int COpBit::FuncOR_( int iX, int iY ) {
	return iX | iY;
}

inline int COpBit::FuncXOR_( int iX, int iY ) {
	return iX ^ iY;
}

inline int COpBit::FuncAND_( int iX, int iY ) {
	return iX & iY;
}

inline int COpBit::FuncNOT_( int iX, int iY ) {
	UNUSED_PARAMETER( iY );
	return ~iX;
}

inline int COpBit::FuncSAL_( int iX, int iY ) {
	return iX << iY;
}

inline int COpBit::FuncSAR_( int iX, int iY ) {
	return iX >> iY;
}
