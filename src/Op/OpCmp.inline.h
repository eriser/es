// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCmp.inline.h

////////////////////////////////////////////////////////////////////////////////
// COpCmp
////////////////////////////////////////////////////////////////////////////////

inline double COpCmp::FuncGreater_( double dX, double dY ) {
	return ( dX > dY )?( +1.0 ):( 0.0 );
}

inline double COpCmp::FuncGreaterOrEqual_( double dX, double dY ) {
	return ( dX >= dY )?( +1.0 ):( 0.0 );
}

inline double COpCmp::FuncLess_( double dX, double dY ) {
	return ( dX < dY )?( +1.0 ):( 0.0 );
}

inline double COpCmp::FuncLessOrEqual_( double dX, double dY ) {
	return ( dX <= dY )?( +1.0 ):( 0.0 );
}

inline double COpCmp::FuncEqual_( double dX, double dY ) {
	return ( dX == dY )?( +1.0 ):( 0.0 );
}

inline double COpCmp::FuncNotEqual_( double dX, double dY ) {
	return ( dX != dY )?( +1.0 ):( 0.0 );
}
