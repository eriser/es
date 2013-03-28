// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// GLTool.inline.h

////////////////////////////////////////////////////////////////////////////////
// CGLTool
////////////////////////////////////////////////////////////////////////////////

template<class TVal>
static inline TVal RountToPxl_( TVal tV, GLdouble dPxl, GLdouble dPxl_2 )
{
	const GLdouble dRem = fmod( GLdouble(tV), dPxl );
	tV -= TVal(dRem);
	if( dRem < 0.0 )
	{
		if( dRem <= -dPxl_2 )
			tV -= TVal(dPxl);
	}
	else
	{
		if( dRem >= dPxl_2 )
			tV += TVal(dPxl);
	}
	return tV;
}

inline GLfloat CGLTool::RoundToPixelX( GLfloat fValue ) {
	return RountToPxl_<GLfloat>( fValue, m_dPixelExtX, m_dPixelExtX_2 );
}

inline GLdouble CGLTool::RoundToPixelX( GLdouble dValue ) {
	return RountToPxl_<GLdouble>( dValue, m_dPixelExtX, m_dPixelExtX_2 );
}

inline GLfloat CGLTool::RoundToPixelY( GLfloat fValue ) {
	return RountToPxl_<GLfloat>( fValue, m_dPixelExtY, m_dPixelExtY_2 );
}

inline GLdouble CGLTool::RoundToPixelY( GLdouble dValue ) {
	return RountToPxl_<GLdouble>( dValue, m_dPixelExtY, m_dPixelExtY_2 );
}
