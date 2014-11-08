// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// VisOp.cpp

#include "VisOp.h"
#include "../File/File.h"
#include "../GLTool/GLTool.h"

// Evaluiert einfache Punkt-Rechnung in Form eines Strings aus codierten Fliesskommazahlen.
// Gültige Operationen: * und /. Whitespace wird gesamthaft ignoriert.
// Todo: -/+/e und Whitespace Position prüfen.
static bool Express_( const CStr &roStr, double *pdRet )
{
	CStr oCurr;
	CList<CStr> m_oLstNumbers;
	const unsigned int uiSize = roStr.GetSize();
	unsigned int i;
	for( i=0; i<uiSize; ++i )
	{
		const char c = roStr[i];
		if( isspace( (unsigned char)c ) )
			continue;
		if( isdigit( (unsigned char)c ) || c == '.' || c == '-' || c == '+' || c == 'e' )
		{
			oCurr += c;
		}
		else if( oCurr.GetSize() )
		{
			m_oLstNumbers.Append( oCurr );
			oCurr = c;
			if( oCurr.FindVec( 0, "*/" ) < 0 )
				return false;
		}
		else
			return false;
	}
	if( oCurr.GetSize() )
		m_oLstNumbers.Append( oCurr );

	i = 0;
	m_oLstNumbers.MoveToFront();
	while( m_oLstNumbers.Next() )
	{
		double dVal;
		const char *pcOp = m_oLstNumbers.Get().GetData();
		const char *pcNum = pcOp;
		if( i ) // Am Anfang kein Operatorzeichen.
			++pcNum;

		int iRet = VIS_OP_SSCANF( pcNum, "%lf", &dVal );
		if( iRet == EOF || iRet == -1 )
			return false;
		if( i )
		{
			switch( *pcOp )
			{
			case '/':
				*pdRet /= dVal;
			break;
			case '*':
				*pdRet *= dVal;
			break;
			}
		}
		else
			*pdRet = dVal;
		++i;
	}
	return true;
}


////////////////////////////////////////////////////////////////////////////////
// CVisOp::CItem
////////////////////////////////////////////////////////////////////////////////

GLubyte CVisOp::CItem::m_atColorStd_[4] =   { 0x00,    0x00,    0x00,   0xFF };
GLubyte CVisOp::CItem::m_atColorRoot_[4] =  { 0x00,    0x00,    0x00,   0xFF };
GLubyte CVisOp::CItem::m_atColorSub_[4] =   { 0x00,    0x00,	0x00,   0xFF };
bool CVisOp::CItem::m_bIsRender_ = false;

CVisOp::CItem::CItem( CItem *poItemOutFirst, unsigned int uiPortOut, COp *poOp, unsigned int uiType, GLfloat fX, GLfloat fY )
: m_poOp( poOp )
, m_bIsSelect( false )
, m_uiState( VIS_OP_ITEM_STATE_EDIT )
{
	m_afPos[0] = fX;
	m_afPos[1] = fY;
	UpdateOutItemLists();
	UpdateMetrics();
	SetType( uiType );
	if( m_poOp )
	{
		m_poOp->m_pvData = reinterpret_cast<void*>( this );
	}
	if( poItemOutFirst )
		InsertItemOut( uiPortOut, poItemOutFirst );
}

CVisOp::CItem::CItem( CVisOp::CItem & roItem )
{
	*this = roItem;
}

CVisOp::CItem::~CItem()
{
	DisconnectInAll();
	DisconnectOutAll();
	ClearNestedItemArray_( &m_oArrLstItemOut );
}

CVisOp::CItem & CVisOp::CItem::operator = ( CVisOp::CItem & roItem )
{
	// TODO: Herausfinden, wann genau poOp == 0...
	const COp *poOp = roItem.GetOp();
	if( poOp )
		m_poOp = poOp->Clone();
	else
		m_poOp = 0;

	if( m_poOp )
		m_poOp->m_pvData = reinterpret_cast<void*>( this );

	CStr oStr;
	roItem.Save( oStr, 0.0f, 0.0f );
	Load( oStr, 0.0f, 0.0f );

	// Achtung: m_bIsSelect, m_oLstItemOut werden bei Save/Load nicht berücksichtigt!
	m_bIsSelect = roItem.m_bIsSelect;

	unsigned int uiCountOut = roItem.m_oArrLstItemOut.GetSize();
	ClearNestedItemArray_( &m_oArrLstItemOut );
	m_oArrLstItemOut.Resize( uiCountOut );
	while( uiCountOut )
	{
		--uiCountOut;
		m_oArrLstItemOut[uiCountOut] = new CList<CItem *>( *(roItem.m_oArrLstItemOut[uiCountOut]) );
	}
	return *this;
}

void CVisOp::CItem::Render() const
{
	static char acTmp_[256];
	const bool bShowOutputs = bool( ( m_poOp->GetFlags() & OP_FLAG_SHOW_OUTPUTS ) != 0 );

	glPushName( GLuint( m_poOp->GetID() ) );

	//if( m_uiState == VIS_OP_ITEM_STATE_COLL )
	//{
		/*
		glPushMatrix();
		glTranslatef( 0.0f, 0.0f, -10.0f );
		*/
		/*
		unsigned int i = 0;
		do
		{
			if( i )
			{
				glColor3ubv( CVisOp::m_aubColorBG_ );
				glBegin( GL_LINE_LOOP );
			}
			else
			{
				glColor3ubv( CVisOp::m_aubColorBG_ );
				glBegin( GL_QUADS );
			}
			glVertex2f( m_fLeft, m_fBottom );
			glVertex2f( m_fLeft + VIS_OP_ITEM_ELEM_X, m_fBottom );
			glVertex2f( m_fLeft + VIS_OP_ITEM_ELEM_X, m_fBottom + VIS_OP_ITEM_ELEM_Y );
			glVertex2f( m_fLeft, m_fBottom + VIS_OP_ITEM_ELEM_Y );
			glEnd();
			++i;
		}
		while( i < 2 );

		glPopName();

		return;
		*/
	//}

	glDisable( GL_LINE_SMOOTH );
	glLineWidth( VIS_OP_LINE_WIDTH_STD );

	// Box
	if( !m_bIsRender_ )
	{
		glBegin( GL_TRIANGLE_STRIP );
		//glColor3ubv( CVisOp::CItem::m_atColorStd_ );
		glVertex2f( m_fLeft,  m_fBottom );
		glVertex2f( m_fRight, m_fBottom );
		glVertex2f( m_fLeft,  m_fTop );
		glVertex2f( m_fRight, m_fTop );

		// + Zusätzlicher Hintergrund für Text-Picking.
		if( !bShowOutputs )
		{
			glColor3ubv( CVisOp::m_aubColorBG_ );
			glVertex2f( m_fLeft,  m_fTop );
			glVertex2f( m_fRight, m_fTop );
			glVertex2f( m_fLeft,  m_fTop + VIS_OP_ITEM_ELEM_Y );
			glVertex2f( m_fRight, m_fTop + VIS_OP_ITEM_ELEM_Y );
		}

		glEnd();
	}

	// Status als einfaches Rechteck
	/*
	{
		GLubyte ubVal = (GLubyte)(m_poOp->GetValue());

		const GLfloat fStatExtX = VIS_OP_ITEM_ELEM_X - VIS_OP_ITEM_MARGIN_X*2.0f;
		const GLfloat fStatExtY = VIS_OP_ITEM_ELEM_Y - VIS_OP_ITEM_MARGIN_Y*2.0f;
		const GLfloat fStatLeft =   m_fRight - VIS_OP_ITEM_ELEM_X + VIS_OP_ITEM_MARGIN_X;
		const GLfloat fStatBottom = m_fBottom + VIS_OP_ITEM_MARGIN_Y;

		glBegin( GL_TRIANGLE_STRIP );
		glColor3ub( ubVal, ubVal, ubVal );
		glVertex2f( fStatLeft          , fStatBottom           );
		glVertex2f( fStatLeft+fStatExtX, fStatBottom           );
		glVertex2f( fStatLeft          , fStatBottom+fStatExtY );
		glVertex2f( fStatLeft+fStatExtX, fStatBottom+fStatExtY );
		glEnd();
	}
	*/

	/*
	// Status mit Color-Digits.
	{
		const unsigned int uiBits = 2;
		const unsigned int uiVal = (unsigned int)(m_poOp->GetValue());
		unsigned int uiMask = ( 1 << uiBits ) - 1;
		unsigned int uiShift = 0;
		const unsigned int uiScale = ( sizeof(GLubyte)*8 - uiBits );
		const unsigned int uiStps = ( sizeof(uiVal)*8 ) / uiBits;
		const GLfloat fStatLeft = m_fRight - VIS_OP_ITEM_MARGIN_X;
		const GLfloat fStatRight = m_fRight;
		const GLfloat fStatStpY = ( VIS_OP_ITEM_STD_X - 2.0f * VIS_OP_ITEM_MARGIN_X ) * ( 1.0f / uiStps );
		GLfloat fStatPosY = m_fBottom + VIS_OP_ITEM_MARGIN_Y;

		glBegin( GL_QUADS );
		for( unsigned int i=0; i<uiStps; ++i )
		{
			GLubyte tVal = ( uiVal & uiMask ) >> uiShift;
			tVal <<= uiScale;
			glColor3ub ( tVal, tVal, tVal );
			glVertex2f( fStatLeft, fStatPosY );
			glVertex2f( fStatRight, fStatPosY );
			fStatPosY += fStatStpY;
			glVertex2f( fStatRight, fStatPosY );
			glVertex2f( fStatLeft, fStatPosY );
			uiMask <<= uiBits;
			uiShift += uiBits;
		}
		glEnd();
	}

	*/

	/*
	// Status mit Grob- und Fein-Tuning.
	{
		GLfloat fStpX = 2.0f * CVisOp::m_fPixelExtX_;

		#define VIS_OP_ITEM_STAT_RES0 1.0f
		#define VIS_OP_ITEM_STAT_RES1 10.0f
		#define VIS_OP_ITEM_STAT_RES2 100.0f

		const GLfloat fVal = static_cast<GLfloat>( m_poOp->Out( 0 ) );
		GLfloat tBar0 = fmod( fVal, VIS_OP_ITEM_STAT_RES0 );
		GLfloat tBar1 = fmod( fVal-tBar0, VIS_OP_ITEM_STAT_RES1 );
		GLfloat tBar2 = fmod( fVal-tBar0-tBar1, VIS_OP_ITEM_STAT_RES2 );
		tBar0 *= ( 1.0f / VIS_OP_ITEM_STAT_RES0 );
		tBar1 *= ( 1.0f / VIS_OP_ITEM_STAT_RES1 );
		tBar2 *= ( 1.0f / VIS_OP_ITEM_STAT_RES2 );

		const GLfloat fStatExtY = 0.5f * ( VIS_OP_ITEM_STD_X - 2.0f * VIS_OP_ITEM_MARGIN_X );
		const GLfloat fStatMid = m_fBottom + 0.5f * VIS_OP_ITEM_STD_X;
		GLfloat fStatPosX = m_fRight - fStpX*4.0f;

		//glBegin( GL_QUADS );
		//glColor3ub( 0x0, 0x0, 0x0 );
		//glVertex2f( m_fRight, m_fBottom );
		//glVertex2f( m_fRight, m_fTop );
		//glVertex2f( m_fRight - VIS_OP_ITEM_MARGIN_X, m_fTop );
		//glVertex2f( m_fRight - VIS_OP_ITEM_MARGIN_X, m_fBottom );
		//glEnd();

		//glLineStipple( 1, VIS_OP_STIPPLE );
		//glEnable( GL_LINE_STIPPLE );
		glColor3ubv( CVisOp::m_aubColorFG_ );
		glBegin( GL_LINES );
		fStatPosX += fStpX;
		glVertex2f( fStatPosX, fStatMid );
		glVertex2f( fStatPosX, fStatMid + fStatExtY*tBar2 );
		fStatPosX += fStpX;
		glVertex2f( fStatPosX, fStatMid );
		glVertex2f( fStatPosX, fStatMid + fStatExtY*tBar1 );
		fStatPosX += fStpX;
		glVertex2f( fStatPosX, fStatMid );
		glVertex2f( fStatPosX, fStatMid + fStatExtY*tBar0 );
		glEnd();
		//glDisable( GL_LINE_STIPPLE );

	}
	*/

	if( !m_bIsRender_ )
	{
		glEnable( GL_LINE_SMOOTH );
		glPopName();
		return;
	}

	// Einfacher Rahmen.
	const bool bLineStipple = !m_poOp->IsValid();
	if( bLineStipple )
	{
		glLineStipple( 1, VIS_OP_STIPPLE );
		glEnable( GL_LINE_STIPPLE );
	}
	if( m_bIsSelect )
	{
		glLineWidth( VIS_OP_LINE_WIDTH_SELECT );
	}
	if( bShowOutputs )
	{
		glBegin( GL_LINE_STRIP );
		glColor4ubv( m_atColor );
		glVertex2f( m_fRight - VIS_OP_ITEM_MARGIN_X,  m_fBottom );
		glVertex2f( m_fRight, m_fBottom );
		glVertex2f( m_fRight, m_fTop    );
		glVertex2f( m_fRight - VIS_OP_ITEM_MARGIN_X,  m_fTop    );
		glEnd();
	}
	else
	{
		glBegin( GL_LINE_LOOP );
		glColor4ubv( m_atColor );
		glVertex2f( m_fLeft,  m_fBottom );
		glVertex2f( m_fRight, m_fBottom );
		glVertex2f( m_fRight, m_fTop    );
		glVertex2f( m_fLeft,  m_fTop    );
		glEnd();
	}
	if( bLineStipple )
	{
		glDisable( GL_LINE_STIPPLE );
	}
	if( m_bIsSelect )
	{
		glLineWidth( VIS_OP_LINE_WIDTH_STD );
	}

	/*
	if( m_bIsSelect )
	{
		glBegin( GL_TRIANGLES );
		glColor3ubv( CVisOp::m_aubColorFG_ );
		glVertex2f( m_fLeft, m_fTop );
		glVertex2f( m_fLeft, m_fTop-2.0f*VIS_OP_ITEM_MARGIN_Y );
		glVertex2f( m_fLeft+2.0f*VIS_OP_ITEM_MARGIN_X, m_fTop );
		glEnd();
	}
	*/

	glColor3ubv( CVisOp::m_aubColorFG_ );

	// Namen der Instanz.
	CVisOp::m_oGLTxtPrinter_.SetPos( m_fLeft + VIS_OP_ITEM_MARGIN_X, m_fTop + VIS_OP_ITEM_MARGIN_Y );
	CVisOp::m_oGLTxtPrinter_.Print( m_poOp->GetNameInstance() );

	const unsigned int uiPortIn = m_poOp->GetCountIn();
	const unsigned int uiPortOut = GetCountOut();
	const unsigned int uiRoutines = m_poOp->GetCountRoutine();
	const GLfloat fStatExtY = 0.5f * VIS_OP_ITEM_ELEM_Y;
	const GLfloat fStatPosX = m_fRight - GLfloat( 2.0 * m_oGLTool_.m_dPixelExtX );
	GLfloat fPosX = m_fLeft + VIS_OP_ITEM_MARGIN_X;
	GLfloat fPosY = m_fTop - VIS_OP_ITEM_ELEM_Y;
	unsigned int i = 0;

	// Namen der Inputs.
	i = 0;
	while( i < uiPortIn  )
	{
		CVisOp::m_oGLTxtPrinter_.SetPos( fPosX, fPosY );
		CVisOp::m_oGLTxtPrinter_.Print( m_poOp->GetNameInput( i ) );
		fPosY -= VIS_OP_ITEM_ELEM_Y;
		++i;
	}

	CVisOp::m_oGLTxtPrinter_.SetAlignment( CGLTxtPrinter::ALIGN_RIGHT );

	// Namen der Outputs.
	i = 0;
	fPosX = m_fRight - VIS_OP_ITEM_MARGIN_X;
	fPosY = m_fTop - VIS_OP_ITEM_ELEM_Y;
	while( i < uiPortOut  )
	{
		glColor3ubv( CVisOp::m_aubColorFG_ );
		CVisOp::m_oGLTxtPrinter_.SetPos( fPosX, fPosY );
		if( bShowOutputs )
		{
			VIS_OP_SPRINTF( acTmp_, sizeof(acTmp_)-1, "%.3g", m_poOp->Out( i ) );
			CVisOp::m_oGLTxtPrinter_.Print( acTmp_ );
		}
		else
		{
			// Status.
			{
				GLfloat fStatPosY0 = fPosY + 0.25f * VIS_OP_ITEM_ELEM_Y;
				GLfloat fStatPosY1;
				double dVal = m_poOp->Out( i );
				if( FINITE( dVal ) )
				{
					const GLfloat fVal = GLfloat(fmod( dVal, 1.0 ));
					fStatPosY1 = fStatPosY0 + fStatExtY * fVal;
					glColor3ubv( CVisOp::m_aubColorFG_ );
				}
				else
				{
					fStatPosY1 = fStatPosY0 + fStatExtY;
					fStatPosY0 -= fStatExtY;
					glColor3ubv( CVisOp::m_aubColorSignal1_ );
					m_poOp->Update();
				}

				glBegin( GL_LINES );
				glVertex2f( fStatPosX, fStatPosY0 );
				glVertex2f( fStatPosX, fStatPosY1 );
				glEnd();

				//double dTmp;
				//dVal = modf( dVal, &dTmp );
				//glColor3d( dVal, dVal, dVal );
			}

			CVisOp::m_oGLTxtPrinter_.Print( m_poOp->GetNameOutput( i ) );
		}

		fPosY -= VIS_OP_ITEM_ELEM_Y;
		++i;
	}

	glColor3ubv( CVisOp::m_aubColorFG_ );

	// Namen der Routinen.
	i = 0;
	while( i < uiRoutines )
	{
		CVisOp::m_oGLTxtPrinter_.SetPos( fPosX, fPosY );
		CVisOp::m_oGLTxtPrinter_.Print( m_poOp->GetNameRoutine( i ) );
		fPosY -= VIS_OP_ITEM_ELEM_Y;
		++i;
	}

	CVisOp::m_oGLTxtPrinter_.SetAlignment( CGLTxtPrinter::ALIGN_LEFT );

	/*
	if( m_uiState == VIS_OP_ITEM_STATE_COLL )
	{
		glPopMatrix();
	}
	*/

	glEnable( GL_LINE_SMOOTH );
	glPopName();
}


void CVisOp::CItem::RenderLinkInOut( unsigned int uiPortIn, CVisOp::CItem *poItem, unsigned int uiPortOut ) const
{
	glPushName( 0 ); // Kein Name!
	glBegin( GL_LINES );
	glColor3ubv( CVisOp::m_aubColorFG_ );

	/*
	if( m_uiState == VIS_OP_ITEM_STATE_COLL &&
	    m_uiState == poItem->m_uiState )
	{
		glVertex2f( m_fLeft, m_fBottom );
		glVertex2f( m_fLeft, poItem->m_fBottom );
		glVertex2f( m_fLeft, poItem->m_fBottom );
		glVertex2f( poItem->m_fLeft, poItem->m_fBottom );
	}
	else
	*/
	{
		//if( m_uiState == VIS_OP_ITEM_STATE_COLL )
		//	glVertex2f( m_fLeft, m_fBottom );
		//else
			glVertex2f( m_fLeft, m_fTop - uiPortIn * VIS_OP_ITEM_ELEM_Y - VIS_OP_ITEM_ELEM_Y * 0.85f ); // Input.
		//if( poItem->m_uiState == VIS_OP_ITEM_STATE_COLL )
		//	glVertex2f( poItem->m_fLeft, poItem->m_fBottom );
		//else
			glVertex2f( poItem->m_fRight, poItem->m_fTop - uiPortOut * VIS_OP_ITEM_ELEM_Y - VIS_OP_ITEM_ELEM_Y * 0.85f  ); // Output.

	}
	glEnd();
	glPopName();
}

void CVisOp::CItem::RenderLinkIn( unsigned int uiPortIn, GLfloat fX, GLfloat fY ) const
{
	glPushName( 0 ); // Kein Name!
	glBegin( GL_LINES );
	glColor3ubv( CVisOp::m_aubColorFG_ );
	/*
	if( m_uiState == VIS_OP_ITEM_STATE_COLL )
	{
		glVertex2f( m_fLeft, m_fBottom );
		glVertex2f( fX, m_fBottom );
		glVertex2f( fX, fY  );
	}
	else
	*/
	{
		glVertex2f( m_fLeft, m_fTop - uiPortIn * VIS_OP_ITEM_ELEM_Y - VIS_OP_ITEM_ELEM_Y  * 0.85f ); // Input.
		glVertex2f( fX, fY  );
	}
	glEnd();
	glPopName();
}

void CVisOp::CItem::RenderLinkOut( unsigned int uiPortOut, GLfloat fX, GLfloat fY ) const
{
	glPushName( 0 ); // Kein Name!
	glBegin( GL_LINES );
	glColor3ubv( CVisOp::m_aubColorFG_ );
	/*
	if( m_uiState == VIS_OP_ITEM_STATE_COLL )
	{
		glVertex2f( fX, fY );
		glVertex2f( fX, m_afPos[1] );
		glVertex2f( m_afPos[0], m_afPos[1] );
	}
	else
	*/
	{
		glVertex2f( fX, fY );
		glVertex2f( m_fRight, m_fTop - uiPortOut * VIS_OP_ITEM_ELEM_Y - VIS_OP_ITEM_ELEM_Y * 0.85f  ); // Input.
	}
	glEnd();
	glPopName();
}


int CVisOp::CItem::CallRoutine( GLfloat fX, GLfloat fY )
{
	const unsigned int uiRoutines = m_poOp->GetCountRoutine();
	if( uiRoutines && fX > m_fRight - VIS_OP_ITEM_ELEM_X )
	{
		GLfloat fY0 = m_fTop - m_poOp->GetCountOut() * VIS_OP_ITEM_ELEM_Y;
		GLfloat fY1 = fY0;
		unsigned int j = 0;
		while( j < uiRoutines )
		{
			fY1 -= VIS_OP_ITEM_ELEM_Y;
			if( fY < fY0 && fY > fY1 )
			{
				m_poOp->Routine( j );
				UpdateOutItemLists();
				UpdateMetrics();
				return int( j );
			}
			fY0 = fY1;
			++j;
		}
	}
	return -1;
}

int CVisOp::CItem::GetPortIn( GLfloat fX, GLfloat fY ) const
{
	// Es wird davon ausgegangen, dass (fX, fY) sicher im Item-Bereich liegt!
	if( fX < m_fLeft + VIS_OP_ITEM_ELEM_X )
	{
		GLfloat fY0 = m_fTop;
		GLfloat fY1 = fY0;
		const int iPorts = m_poOp->GetCountIn();
		int i = 0;
		while( i < iPorts )
		{
			fY1 -= VIS_OP_ITEM_ELEM_Y;
			if( fY < fY0 && fY > fY1 )
				return i;
			fY0 = fY1;
			++i;
		}
	}
	return -1;
}

int CVisOp::CItem::GetPortOut( GLfloat fX, GLfloat fY ) const
{
	// Es wird davon ausgegangen, dass (fX, fY) sicher im Item-Bereich liegt!
	if( fX > m_fRight - VIS_OP_ITEM_ELEM_X )
	{
		GLfloat fY0 = m_fTop;
		GLfloat fY1 = fY0;
		const int iPorts = GetCountOut();
		int i = 0;
		while( i < iPorts )
		{
			fY1 -= VIS_OP_ITEM_ELEM_Y;
			if( fY < fY0 && fY > fY1 )
				return i;
			fY0 = fY1;
			++i;
		}
	}
	return -1;
}

bool CVisOp::CItem::CheckPos( GLfloat fX, GLfloat fY ) const
{
	return ( fX > m_fLeft && fX < m_fRight && fY > m_fBottom && fY < m_fTop );
}

bool CVisOp::CItem::CheckArea( GLfloat fX0, GLfloat fY0, GLfloat fX1, GLfloat fY1 )
{
	return ( m_fLeft > fX0 && m_fRight < fX1 && m_fBottom > fY0 && m_fTop < fY1 );
}

int CVisOp::CItem::CheckText( GLfloat fX, GLfloat fY ) const
{
	UNUSED_PARAMETER( fX );
	if( fY > m_fTop )
	{
		return VIS_OP_ITEM_TEXT_INSTANCE_NAME;
	}
	//const int iPortIn = GetPortIn( fX, fY );
	//if( iPortIn >= 0 ){}
	const int iPortOut = GetPortOut( fX, fY );
	if( iPortOut >= 0 )
	{
		int iRet = VIS_OP_ITEM_TEXT_PORT_OUT + iPortOut;
		if( iRet <= VIS_OP_ITEM_TEXT_PORT_OUT_MAX )
			return iRet;
	}
	return -1;
}

CStr CVisOp::CItem::GetText( int iID ) const
{
	char acTmp[32];

	if( iID == VIS_OP_ITEM_TEXT_INSTANCE_NAME )
	{
		return m_poOp->GetNameInstance();
	}
	//if( iID >= VIS_OP_ITEM_TEXT_PORT_IN && iID <= VIS_OP_ITEM_TEXT_PORT_IN_MAX ){}
	if( iID >= VIS_OP_ITEM_TEXT_PORT_OUT && iID <= VIS_OP_ITEM_TEXT_PORT_OUT_MAX )
	{
		const unsigned int uiPortOut = iID - VIS_OP_ITEM_TEXT_PORT_OUT;
		if( uiPortOut < m_poOp->GetCountOut() )
		{
			VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%.14g", m_poOp->Out( uiPortOut ) );
			return CStr( acTmp );
		}
	}
	return CStr("");
}

void CVisOp::CItem::SetText( int iID, const CStr & roText )
{
	if( iID == VIS_OP_ITEM_TEXT_INSTANCE_NAME )
	{
		m_poOp->SetNameInstance( roText );
	}
	//if( iID >= VIS_OP_ITEM_TEXT_PORT_IN && iID <= VIS_OP_ITEM_TEXT_PORT_IN_MAX ){}
	if( iID >= VIS_OP_ITEM_TEXT_PORT_OUT && iID <= VIS_OP_ITEM_TEXT_PORT_OUT_MAX )
	{
		const unsigned int uiPortOut = iID - VIS_OP_ITEM_TEXT_PORT_OUT;
		const unsigned int uiSize = roText.GetSize();
		if( uiSize && uiPortOut < m_poOp->GetCountOut() )
		{
			double dVal = 0.0;
			if( Express_( roText, &dVal ) )
			{
				m_poOp->Out( uiPortOut ) = dVal;
			}
		}
	}
}

void CVisOp::CItem::SetPos( GLfloat fX, GLfloat fY )
{
	m_afPos[0] = fX;
	m_afPos[1] = fY;
	UpdateMetrics();
}

void CVisOp::CItem::TranslatePos( GLfloat fX, GLfloat fY )
{
	m_afPos[0] += fX;
	m_afPos[1] += fY;
	UpdateMetrics();
}

GLfloat CVisOp::CItem::GetPos( unsigned int uiIndex ) {
	return m_afPos[uiIndex];
}

GLfloat CVisOp::CItem::GetLeft() const {
	return m_fLeft;
}

GLfloat CVisOp::CItem::GetRight() const {
	return m_fRight;
}

GLfloat CVisOp::CItem::GetBottom() const {
	return m_fBottom;
}

GLfloat CVisOp::CItem::GetTop() const {
	return m_fTop;
}

GLfloat CVisOp::CItem::GetWidth() const {
	return m_fRight - m_fLeft;
}

GLfloat CVisOp::CItem::GetHeight() const {
	GLfloat fRet = ( m_fTop - m_fBottom );
	if( !( m_poOp->GetFlags() & OP_FLAG_COMPACT ) )
		return fRet + VIS_OP_ITEM_ELEM_Y;
	return fRet;
}

void CVisOp::CItem::SetColor( GLubyte tR, GLubyte tG, GLubyte tB, GLubyte tA )
{
	m_atColor[0] = tR;
	m_atColor[1] = tG;
	m_atColor[2] = tB;
	m_atColor[3] = tA;
}

void CVisOp::CItem::SetColorRGB( GLubyte *ptC )
{
	m_atColor[0] = ptC[0];
	m_atColor[1] = ptC[1];
	m_atColor[2] = ptC[2];
}

void CVisOp::CItem::SetColorRGBA( GLubyte *ptC )
{
	m_atColor[0] = ptC[0];
	m_atColor[1] = ptC[1];
	m_atColor[2] = ptC[2];
	m_atColor[3] = ptC[3];
}

GLubyte CVisOp::CItem::GetColor( unsigned int uiIndex ) {
	return m_atColor[uiIndex];
}


void CVisOp::CItem::SetType( unsigned int uiType )
{
	m_uiType = uiType;

	// Hierarchische Farbcodes: Je weiter unten, desto höhere Priorität!
	if( m_uiType & VIS_OP_ITEM_TYPE_STD )
		VIS_OP_MEM_COPY( m_atColor, m_atColorStd_, 4 * sizeof(m_atColor[0]) );
	if( m_uiType & VIS_OP_ITEM_TYPE_SUB )
		VIS_OP_MEM_COPY( m_atColor, m_atColorSub_, 4 * sizeof(m_atColor[0]) );
	if( m_uiType & VIS_OP_ITEM_TYPE_ROOT )
		VIS_OP_MEM_COPY( m_atColor, m_atColorRoot_, 4 * sizeof(m_atColor[0]) );
}

unsigned int CVisOp::CItem::GetType() const {
	return m_uiType;
}

COp * CVisOp::CItem::GetOp() const {
	return m_poOp;
}

void CVisOp::CItem::SetOp( COp *poOp ) {
	m_poOp = poOp;
}

bool CVisOp::CItem::GetSelect() const {
	return m_bIsSelect;
}

void CVisOp::CItem::SetSelect( bool bSelect ) {
	m_bIsSelect = bSelect;
}

unsigned int CVisOp::CItem::GetState() const {
	return m_uiState;
}

void CVisOp::CItem::SetState( unsigned int uiState ) {
	m_uiState = uiState;
}

void CVisOp::CItem::NextState() {
	++m_uiState;
	if( m_uiState == VIS_OP_ITEM_STATE_MAX )
		m_uiState = 0;
}

unsigned int CVisOp::CItem::GetCountOut() const {
	return m_oArrLstItemOut.GetSize();
}

unsigned int CVisOp::CItem::GetCountOutConnections( unsigned uiPortOut ) {
	return m_oArrLstItemOut[uiPortOut]->GetSize();
}

CVisOp::CItem * CVisOp::CItem::GetItemOut( unsigned int uiPort, unsigned int uiIndex ) const
{
	return m_oArrLstItemOut[uiPort]->At( uiIndex );
}

void CVisOp::CItem::SetItemOut( unsigned int uiPort, unsigned int uiIndex, CVisOp::CItem *poItem )
{
	m_oArrLstItemOut[uiPort]->At( uiIndex ) = poItem;
}

void CVisOp::CItem::InsertItemOut( unsigned int uiPortOut, CVisOp::CItem *poItem )
{
	if( uiPortOut < m_oArrLstItemOut.GetSize() )
	{
		m_oArrLstItemOut[uiPortOut]->Append( poItem );
	}
}

bool CVisOp::CItem::Save( CStr & roDst, GLfloat fRefX, GLfloat fRefY )
{
	char acTmp[64];

	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%g", m_afPos[0] - fRefX );
	roDst += VIS_OP_ITEM_DELIM;
	roDst += acTmp;
	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%g", m_afPos[1] - fRefY );
	roDst += VIS_OP_ITEM_DELIM;
	roDst += acTmp;

	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%.2x", m_atColor[0] );
	roDst += VIS_OP_ITEM_DELIM;
	roDst += acTmp;
	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%.2x", m_atColor[1] );
	roDst += VIS_OP_ITEM_DELIM;
	roDst += acTmp;
	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%.2x", m_atColor[2] );
	roDst += VIS_OP_ITEM_DELIM;
	roDst += acTmp;
	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%.2x", m_atColor[3] );
	roDst += VIS_OP_ITEM_DELIM;
	roDst += acTmp;

	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", m_uiType );
	roDst += VIS_OP_ITEM_DELIM;
	roDst += acTmp;

	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", m_uiState );
	roDst += VIS_OP_ITEM_DELIM;
	roDst += acTmp;

	roDst += VIS_OP_ITEM_DELIM;
	return true;
}

bool CVisOp::CItem::Load( CStr & roSrc, GLfloat fRefX, GLfloat fRefY )
{
	unsigned int uiToken = 0;
	int i;

	while( 1 )
	{
		i = roSrc.Find( 0, VIS_OP_ITEM_DELIM );
		if( i < 0 )
			break;
		if( i )
		{
			CStr oStrToken = roSrc.GetSub( 0, i );

			switch( uiToken )
			{
			case 0: // position x
			{
				GLfloat fPos;
				VIS_OP_SSCANF( oStrToken, "%g", &fPos );
				m_afPos[0] = fRefX + fPos;
			}
			break;
			case 1: // position y
			{
				GLfloat fPos;
				VIS_OP_SSCANF( oStrToken, "%g", &fPos );
				m_afPos[1] = fRefY + fPos;
			}
			break;
			case 2: // r
			{
				unsigned int c;
				VIS_OP_SSCANF( oStrToken, "%x", &c );
				m_atColor[0] = c;
			}
			break;
			case 3: // g
			{
				unsigned int c;
				VIS_OP_SSCANF( oStrToken, "%x", &c );
				m_atColor[1] = c;
			}
			break;
			case 4: // b
			{
				unsigned int c;
				VIS_OP_SSCANF( oStrToken, "%x", &c );
				m_atColor[2] = c;
			}
			break;
			case 5: // a
			{
				unsigned int c;
				VIS_OP_SSCANF( oStrToken, "%x", &c );
				m_atColor[3] = c;
			}
			break;
			case 6: // type
			{
				unsigned int uiType;
				VIS_OP_SSCANF( oStrToken, "%x", &uiType );
				m_uiType = uiType;
				//SetType( uiType );
			}
			break;
			case 7: // state
			{
				unsigned int uiState;
				VIS_OP_SSCANF( oStrToken, "%x", &uiState );
				SetState( uiState );
			}
			break;
			default:
				break;
			}
			++uiToken;
		}
		roSrc.Del( 0, i+1 );
	}

	UpdateMetrics();

	return true;
}

CStr CVisOp::CItem::Info()
{
	char acTmp[32];
	VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", m_poOp->GetID() );
	CStr oStr( "name: " );
	oStr += m_poOp->GetNameInstance();
	oStr += ", id: ";
	oStr += acTmp;
	oStr += ", class: ";
	oStr += m_poOp->GetNameClass();
	return oStr;
}

void CVisOp::CItem::UpdateMetrics()
{
	// rem: Zur Höhe wird UNTEN ein halbes VIS_OP_ITEM_ELEM_Y zugerechnet.
	//      Wenn das "Raster" oder Grid auch auf ein halbes VIS_OP_ITEM_ELEM_Y gesetzt wird, geht das auf!
	const unsigned int uiCountIn = m_poOp->GetCountIn();
	const unsigned int uiCountOut = m_poOp->GetCountOut() + m_poOp->GetCountRoutine();
	m_fLeft =   m_afPos[0];
	m_fRight =  m_afPos[0] + VIS_OP_ITEM_STD_X;
	m_fBottom = m_afPos[1];
	m_fTop =    m_afPos[1] + ( MAX( uiCountIn, uiCountOut )
		* VIS_OP_ITEM_ELEM_Y + VIS_OP_ITEM_ELEM_Y * 0.5f );
}

void CVisOp::CItem::UpdateOutItemLists()
{
	const unsigned int uiCountOutOld = m_oArrLstItemOut.GetSize();
	const unsigned int uiCountOutNew = m_poOp->GetCountOut();
	if( uiCountOutOld != uiCountOutNew )
	{
		CArray< CList<CItem *> *> oArrLstBak = m_oArrLstItemOut;
		unsigned int uiMaxCopy;
		unsigned int uiMaxEmpty;
		if( uiCountOutOld < uiCountOutNew )
		{
			uiMaxCopy = uiCountOutOld;
			uiMaxEmpty = uiMaxCopy + uiCountOutNew - uiCountOutOld;
		}
		else // ( uiCountOutOld > uiCountOutNew )
		{
			uiMaxCopy = uiCountOutNew;
			uiMaxEmpty = 0;

			// Bei allen Items, welche mit den entfernten Ports verbunden warten,
			// dieses Item auf Input-Seite abmelden.
			unsigned int uiDestroy = uiCountOutOld - uiCountOutNew;
			while( uiDestroy  )
			{
				--uiDestroy;
				CList<CItem *> &roL = *oArrLstBak[uiCountOutNew+uiDestroy];
				roL.MoveToFront();
				while( roL.Next() )
				{
					RemoveInput_( roL.Get(), this );
				}
			}
		}

		m_oArrLstItemOut.Resize( uiCountOutNew );

		unsigned int i = 0;
		while( i < uiMaxCopy )
		{
			m_oArrLstItemOut[i] = new CList<CItem *>( *(oArrLstBak[i]) );
			++i;
		}
		while( i < uiMaxEmpty )
		{
			m_oArrLstItemOut[i] = new CList<CItem *>();
			++i;
		}

		ClearNestedItemArray_( &oArrLstBak );
	}
}

void CVisOp::CItem::CopyConnections( CItem *poItemSrc )
{
	if( !poItemSrc || poItemSrc == this || !poItemSrc->m_poOp )
		return;

	// Inputs.
	const unsigned int uiCountIn0 = m_poOp->GetCountIn();
	const unsigned int uiCountIn1 = poItemSrc->m_poOp->GetCountIn();
	unsigned int uiPortIn  = MIN( uiCountIn0, uiCountIn1 );
	while( uiPortIn )
	{
		--uiPortIn;
		const COp::CLink &roLinkIn = poItemSrc->m_poOp->In( uiPortIn );
		if( roLinkIn.IsValid() )
		{
			CItem *poItemOut = reinterpret_cast<CItem *>( roLinkIn.GetOp()->m_pvData );

			// Selbstreferenz.
			//if( poItemOut == poItemSrc )
			//{
			//}

			Connect_( this,
				  uiPortIn,
				  poItemOut,
				  roLinkIn.GetIndex() );
		}
	}

	// Outputs.

	// Die Liste der Outputs des zu "imitierenden" Items zur Sicherheit kopieren, da
	// in Connect_ diese mit RemoveOutput_ verändert wird (umständlich!).
	CArray< CList<CItem *> *> oArrLstItemOut;
	for( unsigned int i=0; i<poItemSrc->m_oArrLstItemOut.GetSize(); ++i )
		oArrLstItemOut.Append( new CList<CItem *>( *poItemSrc->m_oArrLstItemOut[i] ) );

	unsigned int uiPortOut = MIN( GetCountOut(), poItemSrc->GetCountOut() );
	while( uiPortOut )
	{
		--uiPortOut;
		unsigned int uiConnOut = poItemSrc->GetCountOutConnections( uiPortOut );
		while( uiConnOut )
		{
			--uiConnOut;
			CItem *poItemOut = oArrLstItemOut[uiPortOut]->At( uiConnOut );
			// poItemSrc->GetItemOut( uiPortOut, uiConnOut );

			// Input-Port auf Gegenseite suchen.
			COp *poOpThere = poItemOut->GetOp();
			unsigned int uiInThere = poOpThere->GetCountIn();
			while( uiInThere )
			{
				--uiInThere;
				if( poOpThere->In( uiInThere ).GetOp() == poItemSrc->GetOp() )
					break;
			}

			Connect_( poItemOut, uiInThere, this, uiPortOut );
		}
	}

	for( unsigned int i=0; i<oArrLstItemOut.GetSize(); ++i )
		if( oArrLstItemOut[i] )
			delete oArrLstItemOut[i];
}

void CVisOp::CItem::DisconnectIn( unsigned int uiPortIn )
{
	COp::CLink &roLinkIn = m_poOp->In( uiPortIn );
	if( roLinkIn.IsValid() )
	{
		// Beim Input-Item, dieses Item aus den Outputs spezifischen Port entfernen.
		COp *poOpIn = roLinkIn.GetOp();
		RemoveOutput_( reinterpret_cast<CItem *>( poOpIn->m_pvData ), roLinkIn.GetIndex(), this );

		m_poOp->In( uiPortIn ).Reset();
	}

	// Validieren.
	m_poOp->Validate();
	m_poOp->Update();
}

void CVisOp::CItem::DisconnectInAll()
{
	// Bei allen Input-Intems, dieses Item aus der Output-Liste entfernen.
	unsigned int uiPort = m_poOp->GetCountIn();
	while( uiPort-- )
	{
		DisconnectIn( uiPort );
	}
}

void CVisOp::CItem::DisconnectOutAll()
{
	// Beim allen verbundenen Output-Item-Operatoren, diesen Operator aus den Inputs entfernen.
	CItem *poItemOut;
	unsigned int uiPortOut = m_oArrLstItemOut.GetSize();
	while( uiPortOut )
	{
		--uiPortOut;
		m_oArrLstItemOut[uiPortOut]->MoveToFront();
		while( m_oArrLstItemOut[uiPortOut]->GetNext( &poItemOut ) )
		{
			if( poItemOut )
			{
				RemoveInput_( poItemOut, this );
			}
		}
	}
}

void CVisOp::CItem::ResetIn()
{
	unsigned int uiPort = m_poOp->GetCountIn();
	while( uiPort )
	{
		--uiPort;
		m_poOp->In( uiPort ).Reset();
	}
}

void CVisOp::CItem::ResetOut()
{
	unsigned int uiPortOut = m_oArrLstItemOut.GetSize();
	while( uiPortOut )
	{
		--uiPortOut;
		m_oArrLstItemOut[uiPortOut]->Clear();
	}
}

void CVisOp::CItem::Connect_( CItem *poItemIn, unsigned int uiPortIn, CItem *poItemOut, unsigned int uiPortOut )
{
	if( !poItemIn || !poItemOut
	    || uiPortIn >= poItemIn->GetOp()->GetCountIn() || uiPortOut >= poItemOut->GetCountOut() )
		return;

	// Der Operator auf Output-Seite ("links").
	COp *poOpOut = poItemOut->m_poOp;

	// Der Operator auf Input-Seite ("rechts").
	COp *poOpIn = poItemIn->m_poOp;

	// Output-Item bei vorigem Input-Item abmelden...
	//if( poItemOut->m_poItemOut )
	//	RemoveInputOp_( poItemOut->m_poItemOut->m_poOp, poOpOut );
	//poItemOut->DisconnectOutAll();

	// Beim Input-Item ("rechts"), das noch verbundene Input-Item am spezifischen Port abmelden.
	COp::CLink &roLinkOld = poOpIn->In( uiPortIn );
	if( roLinkOld.IsValid() )
	{
		CItem *poItemOldOut = static_cast<CItem *>( roLinkOld.GetOp()->m_pvData );
		//RemoveInput_( poItemOldOut, pItemIn );
		//poItemOldOut->m_poItemOut = 0;
		RemoveOutput_( poItemOldOut, roLinkOld.GetIndex(), poItemIn );
	}

	// Das jetzige Input-Item beim Output-Item anmelden.
	//poItemOut->m_poItemOut = pItemIn;
	poItemOut->InsertItemOut( uiPortOut, poItemIn );

	// Das jetzige Output-Item beim Input-Item anmelden.
	COp::CLink &roLink = poOpIn->In( uiPortIn );
	roLink.SetOp( poOpOut );
	roLink.SetIndex( uiPortOut );

	// Validieren.
	poOpIn->Validate();
	poOpOut->Validate();
	poOpIn->Update();
	poOpOut->Update();
}

void CVisOp::CItem::RemoveInput_( CItem *poDst, const CItem *poInput )
{
	// Beim Operator von poDst alle Input-Verbindungen zu poInput kappen.
	if( poDst && poInput )
	{
		COp *poOpDst = poDst->GetOp();
		const COp *poOpIn  = poInput->GetOp();
		if( poOpDst && poOpIn )
		{
 			unsigned int uiPort = poOpDst->GetCountIn();
			while( uiPort )
			{
				--uiPort;
				if( poOpDst->In( uiPort ).GetOp() == poOpIn )
				{
					poOpDst->In( uiPort ).Reset();
				}
			}
			poOpDst->Validate();
			poOpDst->Update();
		}
	}
}

void CVisOp::CItem::RemoveOutput_( CItem *poDst, unsigned int uiPortOut, CItem *poOutput )
{
	// Bei poDst alle Output-Verbindungen am Port uiPortOut zu poOutput kappen.
	if( poDst ) //&& poOutput )
	{
		if( uiPortOut < poDst->m_oArrLstItemOut.GetSize() )
		{
			CList<CItem *> *poLstItem = poDst->m_oArrLstItemOut[uiPortOut];
			if( poLstItem )
				poLstItem->RemoveEach( poOutput );
		}
	}
}

void CVisOp::CItem::RemoveOutputAll_( CItem *poDst, CItem *poOutput )
{
	// Bei poDst alle Output-Verbindungen an allen Output-Ports zu poOutput kappen.
	if( poDst ) //&& poOutput )
	{
		unsigned int uiPortOut = poDst->m_oArrLstItemOut.GetSize();
		while( uiPortOut )
		{
			--uiPortOut;
			RemoveOutput_( poDst, uiPortOut, poOutput );
		}
	}
}

void CVisOp::CItem::ClearNestedItemArray_( CArray< CList<CItem *> *> *poArrNest )
{
	for( unsigned int i=0; i<poArrNest->GetSize(); ++i )
	{
		if( poArrNest->At(i) )
		{
			delete poArrNest->At(i);
			poArrNest->At(i) = 0;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// CVisOp
////////////////////////////////////////////////////////////////////////////////

CGLTool CVisOp::m_oGLTool_;
CGLTxtPrinter CVisOp::m_oGLTxtPrinter_;
GLubyte CVisOp::m_aubColorFG_[3] = { 0x00, 0x00, 0x00 };
GLubyte CVisOp::m_aubColorBG_[3] = { 0xFF, 0xFF, 0xFF };
GLubyte CVisOp::m_aubColorHigh_[3] = { 0x00, 0x00, 0x00 };
GLubyte CVisOp::m_aubColorSignal0_[3] = { 0x00, 0x00, 0xFF };
GLubyte CVisOp::m_aubColorSignal1_[3] = { 0xFF, 0x00, 0x00 };
unsigned int CVisOp::m_uiFrames_ = 0;

CVisOp::CVisOp( COrgOp *poOrgOp )
: m_poOrgOp( poOrgOp )
, m_poGLTxtFont( 0 )
{
	m_oGLTxtPrinter_.SetLineMulti();
	m_afPickDefPos[0] = 0.0f;
	m_afPickDefPos[1] = 0.0f;
 	m_afPickDefPos[2] = 0.0f;
	m_afPickDefPos[3] = 0.0f;
	m_iXLast = 0;
	m_iYLast = 0;
	m_bFuncKeyA = false;
	m_bFuncKeyB = false;
	m_bShowCursor = false;
	m_bShowLog = false;
	m_bShowOscilloscope = false;
	m_bIsDialog = false;
	m_bIsPickContext0 = false;
	m_oStrFileName = VIS_OP_FILE_NAME_STD;
}

CVisOp::~CVisOp()
{
	Clear();
}

void CVisOp::Setup()
{
	glLineWidth( VIS_OP_LINE_WIDTH_STD );
	glPointSize( 1.0f );

	glClearDepth( 1.0f );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );

	//glShadeModel( GL_SMOOTH );
	glShadeModel( GL_FLAT );

	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );

	glEnable( GL_BLEND );
	glEnable( GL_ALPHA_TEST );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc( GL_SRC_ALPHA_SATURATE, GL_ONE );

	//glEnable( GL_POINT_SMOOTH );	// Antialiasing für Punkte einschalten.
	glEnable( GL_LINE_SMOOTH );	// Antialiasing für Linien einschalten.
	//glEnable( GL_POLYGON_SMOOTH );

	glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	//glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	//glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	//glHint( GL_FOG_HINT, GL_NICEST );

	//glEnable( GL_POLYGON_OFFSET_POINT );
	//glEnable( GL_POLYGON_OFFSET_LINE );
	//glEnable( GL_POLYGON_OFFSET_FILL );
	//glPolygonOffset( 1.2f, 1.2f );

	glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
	glEnable( GL_COLOR_MATERIAL );
	glDisable( GL_TEXTURE_2D );

	glClearColor( m_aubColorBG_[0]/255.0f, m_aubColorBG_[1]/255.0f, m_aubColorBG_[2]/255.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void CVisOp::UpdateFont( CGLTxtFont *poGLTxtFont )
{
	m_poGLTxtFont = poGLTxtFont;
	m_oGLTxtPrinter_.Init( m_poGLTxtFont );
}

void CVisOp::Update()
{
	CoordUpdate();
	m_oGLTool_.Update();
	m_oGLTxtPrinter_.SetPixelWidth( GLfloat( m_oGLTool_.m_dPixelExtX ) );
	m_oGLTxtPrinter_.SetLineHeight(
		( 2.0f / m_oGLTool_.m_iViewportY ) *
		m_oGLTxtPrinter_.GetFont()->GetSizePxl() );
	m_poOrgOp->m_bEnableCache_ = m_bShowOscilloscope;

	// Mit RoundToPixel ist DefPos nicht mehr pixelgenau!
	m_oGLTool_.WndToGLCoord( m_iXLast, m_iYLast, &m_afPickDefPos[0], &m_afPickDefPos[1], 0 );
}

void CVisOp::Proc( GLint iX, GLint iY )
{
	GLfloat fX, fY;
	m_oGLTool_.WndToGLCoord( iX, iY, &fX, &fY, 0 );

	GLint iRenderMode;
	glGetIntegerv( GL_RENDER_MODE, &iRenderMode );
	bool bIsRender = ( iRenderMode == GL_RENDER );
	CItem::m_bIsRender_ = bIsRender;

	const GLfloat fPxlX2 = GLfloat( m_oGLTool_.m_dPixelExtX * 2.0 );
	const GLfloat fPxlX3 = GLfloat( m_oGLTool_.m_dPixelExtX * 3.0 );
	const GLfloat fPxlY2 = GLfloat( m_oGLTool_.m_dPixelExtY * 2.0 );
	const GLfloat fPxlY3 = GLfloat( m_oGLTool_.m_dPixelExtY * 3.0 );

	const GLfloat fPxlX5 = GLfloat( m_oGLTool_.m_dPixelExtX * 5.0 );
	const GLfloat fPxlX6 = GLfloat( m_oGLTool_.m_dPixelExtX * 6.0 );
	const GLfloat fPxlY5 = GLfloat( m_oGLTool_.m_dPixelExtY * 5.0 );
	const GLfloat fPxlY6 = GLfloat( m_oGLTool_.m_dPixelExtY * 6.0 );

	// "Fadenkreuz" (teuer).
	/*
	const GLfloat fXPxl = m_oGLTool_.RoundToPixelX( fX );
	const GLfloat fYPxl = m_oGLTool_.RoundToPixelY( fY );

	const GLfloat fExtX = GLfloat( m_oGLTool_.m_dPixelExtX * 3.0 );
	const GLfloat fExtY = GLfloat( m_oGLTool_.m_dPixelExtY * 3.0 );
	GLfloat fWndX0, fWndY0, fWndX1, fWndY1;
	m_oGLTool_.WndToGLCoord( 0, m_oGLTool_.m_iViewportY, &fWndX0, &fWndY0, 0 );
	m_oGLTool_.WndToGLCoord( m_oGLTool_.m_iViewportX, 0, &fWndX1, &fWndY1, 0 );
	*/

	CoordUpdate();

	if( bIsRender )
	{
		glDisable( GL_LINE_SMOOTH );

		// Fadenkreuz.
		/*
		glBegin( GL_LINES );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( fXPxl, fWndY0 );
		glVertex2f( fXPxl, fWndY0 + fExtY );
		glVertex2f( fXPxl, fWndY1 - fExtY );
		glVertex2f( fXPxl, fWndY1 );
		glVertex2f( fWndX0, fYPxl );
		glVertex2f( fWndX0 + fExtX, fYPxl );
		glVertex2f( fWndX1 - fExtX, fYPxl );
		glVertex2f( fWndX1, fYPxl );
		glEnd();
		*/

		// Aktuellen Pick-Position als X.
		/*
		glBegin( GL_LINES );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( fPickDefPosX0Pxl - fPxlX2, fPickDefPosY0Pxl + fPxlY3 );
		glVertex2f( fPickDefPosX0Pxl + fPxlX3, fPickDefPosY0Pxl - fPxlY2 );
		glVertex2f( fPickDefPosX0Pxl + fPxlX3, fPickDefPosY0Pxl + fPxlY3 );
		glVertex2f( fPickDefPosX0Pxl - fPxlX2, fPickDefPosY0Pxl - fPxlY2 );
		glEnd();
		*/

		// Aktuellen Pick-Position als Kreuz.
		const GLfloat fPX = m_afPickDefPos[0]; //m_oGLTool_.RoundToPixelX( m_afPickDefPos[0] );
		const GLfloat fPY = m_afPickDefPos[1]; //m_oGLTool_.RoundToPixelY( m_afPickDefPos[1] );
		glBegin( GL_LINES );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( fPX - fPxlX2, fPY );
		glVertex2f( fPX + fPxlX3, fPY );
		glVertex2f( fPX, fPY - fPxlY2 );
		glVertex2f( fPX, fPY + fPxlY3 );
		glEnd();
		/*
		glBegin( GL_POINTS );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( m_afPickDefPos[0], m_afPickDefPos[1] );
		glEnd();
		*/
	}

	CItem *poItem;
	m_oLstItem.MoveToFront();
	while( m_oLstItem.GetNext( &poItem ) )
	{
		if( !poItem->GetSelect() )
			ProcOp( poItem->GetOp() );
	}
	// Selektion zuletzt rendern, um verdeckte
	// Rahmen oder Ähnliches nicht zu verdecken.
	m_oLstSelection.MoveToFront();
	while( m_oLstSelection.GetNext( &poItem ) )
	{
		ProcOp( poItem->GetOp() );
	}

	if( !bIsRender )
		return;

	glEnable( GL_LINE_SMOOTH );

	if( m_bShowOscilloscope )
	{
		// Achtung: Hier wird gerundet. Die Ausgabe ist nicht perfekt, aber angemessen.
		glPushName( 0 );
		double *pdCache = m_poOrgOp->m_adCache_;
		const double dXSmpl = 2.0 * ( m_oGLTool_.m_adRightBottom[0] - m_oGLTool_.m_adLeftBottom[0] ) / ORG_OP_CACHE;
		const unsigned int uiSamplPerPxl = (unsigned int)( m_oGLTool_.m_dPixelExtX / dXSmpl ) + 1;
		const unsigned int uiSamplPerPxl2 = uiSamplPerPxl << 1;
		const double dXStp = uiSamplPerPxl * dXSmpl;

		unsigned int p = m_poOrgOp->m_uiCachePos_;
		double dX = m_oGLTool_.m_adLeftBottom[0];
		double dY = m_oGLTool_.m_adLeftBottom[1] + VIS_OP_OSCILLOSCOP_HEIGHT + VIS_OP_ITEM_ELEM_Y;

		glColor3ubv( m_aubColorFG_ );

		glBegin( GL_LINE_STRIP );
		for( unsigned int i=0; i<ORG_OP_CACHE; i+=uiSamplPerPxl )
		{
			glVertex2d( dX, dY + pdCache[p] * VIS_OP_OSCILLOSCOP_HEIGHT );
			p += uiSamplPerPxl2;
			if( p >= ORG_OP_CACHE )
				p = 0;
			dX += dXStp;
		}
		glEnd();

		p = m_poOrgOp->m_uiCachePos_ + 1;
		dX = m_oGLTool_.m_adLeftBottom[0];
		dY += VIS_OP_OSCILLOSCOP_HEIGHT * 2 + VIS_OP_ITEM_ELEM_Y;

		glBegin( GL_LINE_STRIP );
		for( unsigned int i=0; i<ORG_OP_CACHE; i+=uiSamplPerPxl )
		{
			glVertex2d( dX, dY + pdCache[p] * VIS_OP_OSCILLOSCOP_HEIGHT );
			p += uiSamplPerPxl2;
			if( p >= ORG_OP_CACHE )
				p = 1;
			dX += dXStp;
		}
		glEnd();
		glPopName();
	}

	glDisable( GL_LINE_SMOOTH );
	glPushName( 0 );

	// Koordinatenursprung.
	/*
	{
		const GLfloat fExtX = 2.0f * m_fPixelExtX_;
		const GLfloat fExtY = 2.0f * m_fPixelExtY_;

		glBegin( GL_POINTS );
		glVertex2f( 0.0f, 0.0f );
		glVertex2f( -fExtX, 0.0f );
		glVertex2f( +fExtX, 0.0f );
		glVertex2f( 0.0f, -fExtY );
		glVertex2f( 0.0f, +fExtY );
		glEnd();
	}
	*/

	if( m_bIsPickContext0 )
	{
		const GLfloat fContext0ExtY_ = VIS_OP_ITEM_ELEM_Y
					     * VIS_OP_CONTEXT0_MAX_Y
					     + VIS_OP_ITEM_ELEM_Y
					     + VIS_OP_ITEM_MARGIN_Y;

		GLfloat fBackgroundY = m_afPickContext0Start[1];
		GLfloat fBackgroundX = m_afPickContext0Start[0];
		GLfloat fPosX = fBackgroundX + VIS_OP_ITEM_MARGIN_X * 1.0f;
		GLfloat fPosY = fBackgroundY - VIS_OP_ITEM_ELEM_Y;
		bool bDrawBackground = true;
		unsigned int uiCol = 0;
		int i = 0;

		fBackgroundY += VIS_OP_ITEM_MARGIN_Y;

		while( 1 )
		{
			const char *pcName = m_poOrgOp->GetNameClassShort( i );
			if( !pcName )
				break;
			CStr oStrName( pcName );

			/*
			// String exakt hintermalen.
			const GLfloat fWidth = m_oGLTxtPrinter_.GetFont()->GetWidth( pcName ) + 2.0f * VIS_OP_ITEM_MARGIN_X;
			const GLfloat fHeight = VIS_OP_CONTEXT0_EXT_Y;
			const GLfloat fPosXBox = fPosX - VIS_OP_ITEM_MARGIN_X;
			glBegin( GL_TRIANGLE_STRIP );
			glColor3ubv( m_aubColorBG_ );
			glVertex2f( fPosXBox,		fPosY );
			glVertex2f( fPosXBox + fWidth,	fPosY );
			glVertex2f( fPosXBox,		fPosY + fHeight );
			glVertex2f( fPosXBox + fWidth,	fPosY + fHeight );
			glEnd();
			*/

			if( bDrawBackground )
			{
				glBegin( GL_TRIANGLE_STRIP );
				glColor4ub( m_aubColorBG_[0], m_aubColorBG_[1], m_aubColorBG_[2], VISOP_STD_TRANSPARENCY0 );
				glVertex2f( fBackgroundX + VIS_OP_CONTEXT0_EXT_X, fBackgroundY );
				glVertex2f( fBackgroundX,			  fBackgroundY );
				glVertex2f( fBackgroundX + VIS_OP_CONTEXT0_EXT_X, fBackgroundY - fContext0ExtY_ );
				glVertex2f( fBackgroundX,                         fBackgroundY - fContext0ExtY_ );
				glEnd();
				fBackgroundX += VIS_OP_CONTEXT0_EXT_X;
				bDrawBackground = false;
			}

			if( m_iPickContext0 == i )
			{
				/*
				if( VIS_OP_BLINK( m_uiFrames_ ) )
					glColor3ubv( m_aubColorHigh_ );
				else
					glColor4ub( m_aubColorBg_ );
				*/
				/*
				// Vertikale Linie neben Eintrag.
				const GLfloat fIndicatorX = fPosX - m_fStpX * 0.25f;
				glBegin( GL_LINES );
				glVertex2f( fIndicatorX, fPosY + VIS_OP_CONTEXT0_EXT_Y );
				glVertex2f( fIndicatorX, fPosY );
				glEnd();
				*/
				glColor3ubv( m_aubColorFG_ );
				oStrName = "[" + oStrName + "]";
			}
			else
			{
				glColor3ubv( m_aubColorFG_ );
				oStrName = " " + oStrName;
			}

			m_oGLTxtPrinter_.SetPos( fPosX, fPosY );
			m_oGLTxtPrinter_.Print( oStrName.GetData() );
			++i;
			fPosY -= VIS_OP_ITEM_ELEM_Y;

			if( i % VIS_OP_CONTEXT0_MAX_Y == 0 )
			{
				fPosX += VIS_OP_CONTEXT0_EXT_X;
				fPosY = m_afPickContext0Start[1] - VIS_OP_ITEM_ELEM_Y;

				bDrawBackground = true;
				++uiCol;
			}
		}

		/*
		// Eine Line oberhalb.
		glBegin( GL_LINES );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( m_afPickContext0Start[0], fBackgroundY );
		glVertex2f( fBackgroundX, fBackgroundY );
		glEnd();
		*/

		glBegin( GL_LINE_LOOP );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( fBackgroundX,		fBackgroundY );
		glVertex2f( m_afPickContext0Start[0],	fBackgroundY );
		glVertex2f( m_afPickContext0Start[0],	fBackgroundY - fContext0ExtY_ );
		glVertex2f( fBackgroundX,		fBackgroundY - fContext0ExtY_ );
		glEnd();
	}
	if( m_bIsPickDefpos )
	{
		glLineStipple( 1, VIS_OP_STIPPLE );
		glEnable( GL_LINE_STIPPLE );
		glBegin( GL_LINE_LOOP );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( m_afPickDefPos[0], m_afPickDefPos[1] );
		glVertex2f( m_afPickDefPos[2], m_afPickDefPos[1] );
		glVertex2f( m_afPickDefPos[2], m_afPickDefPos[3] );
		glVertex2f( m_afPickDefPos[0], m_afPickDefPos[3] );
		glEnd();
		glDisable( GL_LINE_STIPPLE );
	}

	// Textmode-Info / Log.
	if( m_uiTextMode != VIS_OP_TEXT_MODE_NONE || m_bShowLog )
	{
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();

		const GLfloat fTxtRefX = -0.975f;
		const GLfloat fTxtRefY = +0.875f;

		if( m_uiTextMode != VIS_OP_TEXT_MODE_NONE )
		{
			// Hintergrund.
			const unsigned int uiLineCount = m_oGLTxtPrinter_.GetLineCount( m_oStrBuffer.GetData() );
			const GLfloat fLineHeight = GLfloat(m_oGLTxtPrinter_.GetFont()->GetSizePxl() * m_oGLTool_.m_dPixelExtY);
			const GLfloat fBottom = fTxtRefY
				- fLineHeight * GLfloat( uiLineCount );

			if( uiLineCount == 1 )
			{
				glBegin( GL_TRIANGLE_STRIP );
				glColor4ub( m_aubColorBG_[0], m_aubColorBG_[1], m_aubColorBG_[2], VISOP_STD_TRANSPARENCY0 );
				glVertex2f( +1.0f, +1.0f );
				glVertex2f( -1.0f, +1.0f );
				glVertex2f( +1.0f, fBottom );
				glVertex2f( -1.0f, fBottom );
				glEnd();
			}
			else
			{
				RenderBGOverlay();
			}

			if( m_bTextSelectAll )
				glColor3ubv( m_aubColorSignal0_ );
			else
				glColor3ubv( m_aubColorFG_ );

			m_oGLTxtPrinter_.SetPos( fTxtRefX, fTxtRefY );
			m_oGLTxtPrinter_.Print( m_oStrBuffer.GetData() );

			glBegin( GL_LINES );
			glColor3ubv( m_aubColorFG_ );
			glVertex2f( -1.0f, +0.99f );
			glVertex2f( -0.8f, +0.99f );
			glEnd();

		}
		if( m_bShowLog )
		{
			m_oGLTxtPrinter_.SetAlignment( CGLTxtPrinter::ALIGN_RIGHT );
			m_oGLTxtPrinter_.SetPixelWidth( 2.0f / m_oGLTool_.m_iViewportX );
			const GLfloat fLineHeight = m_oGLTxtPrinter_.GetLineHeight();
			GLfloat fXL = -fTxtRefX, fYL = fTxtRefY;
			for( unsigned int i=0; i<OP_LOG_LINES && fYL > -fTxtRefY; ++i )
			{
				m_oGLTxtPrinter_.SetPos( fXL, fYL );
				m_oGLTxtPrinter_.Print( _Op_LogGet( i ) );
				fYL -= fLineHeight;
			}
			m_oGLTxtPrinter_.SetAlignment( CGLTxtPrinter::ALIGN_LEFT );
			m_oGLTxtPrinter_.SetPixelWidth( GLfloat( m_oGLTool_.m_dPixelExtX ) );

			glBegin( GL_LINES );
			glColor3ubv( m_aubColorFG_ );
			glVertex2f( +1.0f, +0.99f );
			glVertex2f( +0.8f, +0.99f );
			glEnd();
		}

		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
	}

	if( m_bIsDialog )
	{
		glPushMatrix();

		// x-/y-Translation rückgängig machen,
		// damit der Dialog schön in der Mitte auftaucht.
		// rem: Dies wäre einfacher lösbar mit Zugriff auf die
		//      Translate-Variablen...
		GLdouble m_adModelview[16];
		glGetDoublev( GL_MODELVIEW_MATRIX, m_adModelview );
		glTranslated( -m_adModelview[12], -m_adModelview[13], 0.0 );

		RenderBGOverlay();

		glBegin( GL_LINE_LOOP );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( -VIS_OP_DIALOG_EXT_X, -VIS_OP_DIALOG_EXT_Y );
		glVertex2f( +VIS_OP_DIALOG_EXT_X, -VIS_OP_DIALOG_EXT_Y );
		glVertex2f( +VIS_OP_DIALOG_EXT_X, +VIS_OP_DIALOG_EXT_Y );
		glVertex2f( -VIS_OP_DIALOG_EXT_X, +VIS_OP_DIALOG_EXT_Y );
		glEnd();

		m_oGLTxtPrinter_.SetPos( -VIS_OP_DIALOG_EXT_X + VIS_OP_ITEM_ELEM_X, VIS_OP_ITEM_ELEM_Y );
		m_oGLTxtPrinter_.Print( m_oStrBuffer.GetData() );

		GLfloat fX0 = -VIS_OP_DIALOG_EXT_X + VIS_OP_ITEM_ELEM_X;
		const GLfloat fY0 = -VIS_OP_DIALOG_EXT_Y * 0.5f;

		//GLfloat fX1 = fX0 + VIS_OP_ITEM_ELEM_X;
		//const GLfloat fY1 = fY0 + VIS_OP_ITEM_ELEM_Y;
		//glBegin( GL_LINE_LOOP );
		//glVertex2f( fX0, fY0 - VIS_OP_ITEM_ELEM_Y );
		//glVertex2f( fX1 + VIS_OP_ITEM_ELEM_X * 2.0f, fY0 - VIS_OP_ITEM_ELEM_Y );
		//glVertex2f( fX1 + VIS_OP_ITEM_ELEM_X * 2.0f, fY1 + VIS_OP_ITEM_ELEM_Y );
		//glVertex2f( fX0, fY1 + VIS_OP_ITEM_ELEM_Y );
		//glEnd();

		m_oGLTxtPrinter_.SetPos( fX0, fY0 );
		m_oGLTxtPrinter_.Print( m_tDialogFunc ? VIS_OP_DIALOG_STR_YES : VIS_OP_DIALOG_STR_OK );

		if( m_tDialogFunc )
		{
			fX0 = 0.0f + VIS_OP_ITEM_ELEM_X;
			//fX1 = fX0 + VIS_OP_ITEM_ELEM_X;
			//glBegin( GL_LINE_LOOP );
			//glVertex2f( fX0, fY0 - VIS_OP_ITEM_ELEM_Y );
			//glVertex2f( fX1 + VIS_OP_ITEM_ELEM_X * 2.0f, fY0 - VIS_OP_ITEM_ELEM_Y );
			//glVertex2f( fX1 + VIS_OP_ITEM_ELEM_X * 2.0f, fY1 + VIS_OP_ITEM_ELEM_Y );
			//glVertex2f( fX0, fY1 + VIS_OP_ITEM_ELEM_Y );
			//glEnd();

			m_oGLTxtPrinter_.SetPos( fX0, fY0 );
			m_oGLTxtPrinter_.Print( VIS_OP_DIALOG_STR_NO );
		}

		glPopMatrix();
	}

	if( m_bShowCursor )
	{
		glBegin( GL_LINES );
		glColor3ubv( m_aubColorFG_ );
		glVertex2f( fX - fPxlX5, fY );
		glVertex2f( fX + fPxlX6, fY );
		glVertex2f( fX, fY - fPxlY5 );
		glVertex2f( fX, fY + fPxlY6 );
		glEnd();
	}


#if defined( VIS_OP_USE_FRAME )
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	// Zusätzlicher Fensterrahmen.
	glLineWidth( 2.0 );
	glBegin( GL_LINE_LOOP );
	glColor3ub( 0x00, 0x00, 0x00 );
	glVertex2f( -1.0f, -1.0f );
	glVertex2f( +1.0f, -1.0f );
	glVertex2f( +1.0f, +1.0f );
	glVertex2f( -1.0f, +1.0f );
	glEnd();
	glLineWidth( VIS_OP_LINE_WIDTH_STD );

	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
#endif // VIS_OP_USE_FRAME

	glPopName();
	glEnable( GL_LINE_SMOOTH );

	++m_uiFrames_;
}

void CVisOp::Init()
{
	Clear();
	m_poItemPickMove = 0;
	m_poItemPickMoveLinkIn0 = 0;
	m_poItemPickMoveLinkIn1 = 0;
	m_poItemPickMoveLinkOut0 = 0;
	m_poItemPickMoveLinkOut1 = 0;
	m_poItemPickAction0 = 0;
	m_poItemPickAction1 = 0;
	m_poItemPickTwiddle = 0;
	m_poItemPickCurr = 0;
	m_uiTextMode = VIS_OP_TEXT_MODE_NONE;
	m_bTextProtected = false;
	m_bTextSelectAll = false;
	m_tDialogFunc = 0;
	m_pvDialogArg0 = 0;
	m_uiDialogArg1 = 0;

	m_fStpX = VIS_OP_ITEM_STD_X * 0.25f;
	m_fStpY = VIS_OP_ITEM_ELEM_Y * 0.5f;

	GLfloat fRefX = +1.0f - 2.0f * m_fStpX - VIS_OP_ITEM_STD_X;
	GLfloat fRefY = -1.0f + 2.0f * m_fStpY;
	GLfloat fPosY;

	SnapToGrid( &fRefX, &fRefY );

	// Behalte Zeiger für Vereinfachungen bei späteren Vergleichen...
	m_poItemRootR =
		new CItem( 0, 0, m_poOrgOp->GetOpRootRight_(), VIS_OP_ITEM_TYPE_ROOT,
			fRefX, fPosY = fRefY );

	m_poItemRootL =
		new CItem( 0, 0, m_poOrgOp->GetOpRootLeft_(), VIS_OP_ITEM_TYPE_ROOT,
			fRefX, fPosY += 2.0f * m_fStpY + m_poItemRootR->GetHeight() );

	InsertItem( m_poItemRootR );
	InsertItem( m_poItemRootL );

	// Auch unverbunden Operatoren werden berücksichtigt!
	InsertUnconnectedItems( fRefX, fPosY );

	UnSelectAllItems(); // ...denn in InsertItem() werden die neuen Items selektiert.

	_Op_LogClear();
}

void CVisOp::Clear()
{
	CItem *poItem;

	m_oLstItem.MoveToFront();
	while( m_oLstItem.GetNext( &poItem ) )
	{
		if( poItem )
			delete poItem;
	}
	m_oLstItem.Clear();
	m_oLstSelection.Clear();
	ClipboardDeleteAll();
}

bool CVisOp::SaveAll( CStr &roDst )
{
	// rem: Wird über OrgOp gelöst, um Reihenfolge beizubehalten.
	//     (Was aber nicht zwingend ist, aber die Lesbarkeit des Patches erhöht!)
	COp *poOp;
	m_poOrgOp->Rewind();
	while( m_poOrgOp->GetNext( &poOp ) )
	{
		if( !Save( roDst, reinterpret_cast<CItem *>( poOp->m_pvData ), 0.0f, 0.0f ) )
			return false;
	}

	char acTmp[256];
	const double adCoord[] = {
		double(m_afPickDefPos[0]), double(m_afPickDefPos[1]),
		double(m_afCoordTrans[0]), double(m_afCoordTrans[1]), double(m_afCoordTrans[2]),
		( m_bShowLog ? 1.0 : 0.0 ), ( m_bShowOscilloscope ? 1.0 : 0.0 ) };
	unsigned int j = 0;
	while( j < sizeof(adCoord) / sizeof(adCoord[0]) )
	{
		VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%g", adCoord[j] );
		roDst += VIS_OP_DELIM;
		roDst += acTmp;
		++j;
	}
	roDst += VIS_OP_DELIM;
	roDst += VIS_OP_NEWLINE;

	_Op_LogClear();

	return true;
}

bool CVisOp::Save( CStr &roDst, CItem *poItem, GLfloat fRefX, GLfloat fRefY )
{
	char acTmp[32];
	if( poItem )
	{
		VIS_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", poItem->GetOp()->GetID() );
		roDst += VIS_OP_ITEM_DELIM;
		roDst += acTmp;

		poItem->Save( roDst, fRefX, fRefY );

		roDst += VIS_OP_NEWLINE;
		return true;
	}
	return false;
}

bool CVisOp::Load( CStr &roSrc, unsigned int uiIDOffset, GLfloat fRefX, GLfloat fRefY )
{
	int i;
	while( 1 )
	{
		i = roSrc.Find( 0, VIS_OP_ITEM_DELIM );
		if( i < 0 )
			break;
		roSrc.Del( 0, i+1 );

		i = roSrc.Find( 0, VIS_OP_NEWLINE );
		if( i < 0 )
			break;
		CStr oStrLine = roSrc.GetSub( 0, i );
		roSrc.Del( 0, i+sizeof( VIS_OP_NEWLINE ) );

		i = oStrLine.Find( 0, VIS_OP_ITEM_DELIM );
		if( i < 0 )
			break;
		CStr oStrToken = oStrLine.GetSub( 0, i );
		oStrLine.Del( 0, i+1 );

		unsigned int uiID;
		VIS_OP_SSCANF( oStrToken, "%x", &uiID );
		COp *poOp = CVisOp::m_poOrgOp->GetOp( uiID + uiIDOffset );
		if( !poOp )
			return false;
		SearchItem( poOp )->Load( oStrLine, fRefX, fRefY );
	}

	return true;
}

CVisOp::CItem * CVisOp::SearchItem( const COp *poOp )
{
	CItem *poItem;
	m_oLstItem.MoveToFront();
	while( m_oLstItem.GetNext( &poItem ) )
	{
		if( poItem->GetOp() == poOp )
			return poItem;
	}
	return 0;
}

CVisOp::CItem * CVisOp::SearchItem( unsigned int uiID )
{
	CItem *poItem;
	m_oLstItem.MoveToFront();
	while( m_oLstItem.GetNext( &poItem ) )
	{
		if( poItem->GetOp()->GetID() == uiID )
			return poItem;
	}
	return 0;
}

void CVisOp::InsertItem( CItem *poItem )
{
	COp *poOp = poItem->GetOp();

	m_oLstItem.Append( poItem );

	VIS_OP_LOG( "insert: %s ", poItem->Info().GetData() );
	VIS_OP_LOG( "(item 0x%p) ", (void *)poItem );
	VIS_OP_LOG( "(op 0x%p)\n", (void *)poOp );

	SelectItem( poItem );

	GLfloat fPosX = poItem->GetPos( 0 );
	GLfloat fPosY = poItem->GetPos( 1 );
	unsigned int uiOpInCount = poOp->GetCountIn();
	while( uiOpInCount )
	{
		--uiOpInCount;
		COp::CLink &roLinkNew = poOp->In( uiOpInCount );
		COp *poOpNew = roLinkNew.GetOp();
		unsigned int uiPortOut = roLinkNew.GetIndex();
		if( poOpNew ) // Ersatz für IsValid().
		{
			CItem *poItemPresent = SearchItem( poOpNew );

			// Wenn nicht schon vorhanden: Rekursion.
			if( !poItemPresent )
			{
				CItem *poItemNext =
					new CItem( poItem, uiPortOut, poOpNew, VIS_OP_ITEM_TYPE_STD,
						fPosX - 5.0f * m_fStpX, fPosY );

				InsertItem( poItemNext );

				fPosY += poItemNext->GetHeight();
				if( !( poOpNew->GetFlags() & OP_FLAG_COMPACT ) )
					fPosY += 2.0f * m_fStpY;
			}
			// Sonst, beim schon vorhandenen Item als Output anmelden.
			else
			{
				poItemPresent->InsertItemOut( uiPortOut, poItem );
			}
		}

	}
}

void CVisOp::RemoveItem( CItem *poItem )
{
	COp *poOp = poItem->GetOp();
	if( !m_oLstItem.RemoveEach( poItem ) )
		return;

	VIS_OP_LOG( "remove: %s ", poItem->Info().GetData() );
	VIS_OP_LOG( "(item 0x%p) ", (void *)poItem );
	VIS_OP_LOG( "(op 0x%p)\n", (void *)poOp );

	delete poItem;

	if( poOp )
	{
		if( !m_poOrgOp->Delete( poOp ) )
		{
			// Nur wenn nicht bei OrgOp angemeldeldet!
			delete poOp;
		}
	}
}

void CVisOp::InsertUnconnectedItems( GLfloat fX, GLfloat fY )
{
	COp *poOp;
	CItem *poItem;
	bool bIsSearching;
	do
	{
		m_poOrgOp->Rewind();
		while( ( bIsSearching = m_poOrgOp->GetNext( &poOp ) ) )
		{
			if( !SearchItem( poOp ) )
			{
				InsertItem(
					poItem = new CItem( 0, 0, poOp, VIS_OP_ITEM_TYPE_STD, fX, fY )
				);
				fY += poItem->GetHeight();
				if( !( poOp->GetFlags() & OP_FLAG_COMPACT ) )
					fY += 2.0f * m_fStpY;
				break;
			}
		}
	}
	while( bIsSearching );
}

bool CVisOp::SelectItem( CVisOp::CItem *poItem )
{
	if( poItem )
	{
		int iIndex = m_oLstSelection.FindFirst( poItem );
		if( iIndex < 0 )
		{
			poItem->SetSelect( true );
			m_oLstSelection.Append( poItem );
			return true;
		}
	}
	return false;
}

void CVisOp::SelectAllItems()
{
	CItem *poItem;
	m_oLstItem.MoveToFront();
	while( m_oLstItem.GetNext( &poItem ) )
		SelectItem( poItem );
}

bool CVisOp::UnSelectItem( CVisOp::CItem *poItem )
{
	if( poItem )
	{
		poItem->SetSelect( false );
		return m_oLstSelection.RemoveFirst( poItem );
	}
	return false;
}

void CVisOp::UnSelectAllItems()
{
	CItem *poItem;
	m_oLstSelection.MoveToFront();
	while( m_oLstSelection.GetNext( &poItem ) )
		poItem->SetSelect( false );

	m_oLstSelection.Clear();
}

CVisOp::CItem * CVisOp::GetSelectionSingle() const
{
	if( m_poItemPickCurr )
		return m_poItemPickCurr;
	else if( m_oLstSelection.GetSize() == 1 )
		return m_oLstSelection[0];
	return 0;
}

void CVisOp::ClipboardCopyTo( CList<CItem *> *poLstItem )
{
	const unsigned int uiNum = poLstItem->GetSize();

	// Items aus der Liste auf das Clipboard kopieren und Verbindungen lösen.
	CItem *poItem;
	poLstItem->MoveToFront();
	while( poLstItem->GetNext( &poItem ) )
	{
		if( poItem )
		{
			CItem *poItemCopy = new CItem( *poItem );
			poItemCopy->ResetIn();
			poItemCopy->ResetOut();
			poItemCopy->SetSelect( false );
			m_oLstClipboard.Append( poItemCopy );
		}
	}

	if( m_oLstClipboard.GetSize() == 0 )
		return;

	// i/o-reconnect
	// Da in beiden Listen die Reihenfolge der korrespondierenden Items diesselbe ist,
	// können alle Verbindungen in den Originalen direkt mit den Indizes auf die
	// Kopien übertragen werden.
	unsigned int uiIndexCurr = 0;
	while( uiIndexCurr < uiNum )
	{
		CItem *poItemCurr = poLstItem->At( uiIndexCurr );
		COp *poOpCurr = poItemCurr->GetOp();

		CItem *poItemCorr = m_oLstClipboard.At( uiIndexCurr );
		COp *poOpCorr = poItemCorr->GetOp();

		// Input-Ops
		// Alle Input-Ports des aktuellen Ops durchgehen, und nach In-Patch-Connections suchen...
		// Diese Verbindungen werden über die kopierten Items neu erstellt.
		unsigned int uiPortIn = poOpCurr->GetCountIn();
		while( uiPortIn )
		{
			--uiPortIn;

			COp::CLink &roLink = poOpCurr->In( uiPortIn );
			COp *poOpIn = roLink.GetOp();
			unsigned int uiPortOut = roLink.GetIndex();
			if( poOpIn )
			{
				unsigned int uiIndexTst = 0;
				CItem *poItemTst;
				poLstItem->MoveToFront();
				while( poLstItem->GetNext( &poItemTst ) )
				{
					// Inputs die auf den Op des selben Items zeigen ausschliessen.
					//if( poItemTst != poItemCurr )
					//	continue;

					COp *poOpTst = poItemTst->GetOp();
					if( poOpIn == poOpTst )
					{
						poOpCorr->In( uiPortIn ).SetOp( m_oLstClipboard[uiIndexTst]->GetOp() );
						poOpCorr->In( uiPortIn ).SetIndex( uiPortOut );
						poOpCorr->Validate();
						poOpCorr->Update();
					}
					++uiIndexTst;
				}
			}
		}

		// Output-Items
		// Alle Output-Ports des aktuellen Items durchgehen...
		// -> Alle sonstigen Verbindung werden gelöscht (Grenze).
		unsigned int uiPortOut = poItemCurr->GetCountOut();
		while( uiPortOut )
		{
			--uiPortOut;

			// Die aktuelle Output-Liste des durchgehen, und nach In-Patch-Connections suchen...
			unsigned int uiPortOutConn = poItemCurr->GetCountOutConnections( uiPortOut );
			while( uiPortOutConn )
			{
				--uiPortOutConn;

				CItem *poItemOut = poItemCurr->GetItemOut( uiPortOut, uiPortOutConn );
				if( poItemOut )
				{
					unsigned int uiIndexTst = 0;
					CItem *poItemTst;
					poLstItem->MoveToFront();
					while( poLstItem->GetNext( &poItemTst ) )
					{
						// Inputs die auf den Op des selben Items zeigen ausschliessen.
						//if( poItemTst != poItemCurr )
						//	continue;

						if( poItemOut == poItemTst )
						{
							// nicht SetItemOut
							poItemCorr->InsertItemOut(
								uiPortOut,
								//uiPortOutConn,
								m_oLstClipboard[uiIndexTst]
							);
							poItemCorr->GetOp()->Validate();
							poItemCorr->GetOp()->Update();
						}
						++uiIndexTst;
					}
				}
			}
		}
		++uiIndexCurr;
	}
}

void CVisOp::ClipboardPasteFrom()
{
	CItem *poItem;
	bool bFirst = true;
	GLfloat fXTrans = 0.0f, fYTrans = 0.0f;

	m_oLstClipboard.MoveToFront();
	while( m_oLstClipboard.GetNext( &poItem ) )
	{
		if( bFirst )
		{
			fXTrans = m_afPickDefPos[0] - poItem->GetPos(0);
			fYTrans = m_afPickDefPos[1] - poItem->GetPos(1);
			bFirst = false;
		}
		poItem->TranslatePos( fXTrans, fYTrans );

		VIS_OP_LOG( "paste: %s\n", poItem->Info().GetData() );

		m_poOrgOp->Insert( poItem->GetOp() );
		m_oLstItem.Append( poItem ); // ACHTUNG: Nicht via InsertItem, da schon korrekt verknüpft!
		SelectItem( poItem );
	}
	m_oLstClipboard.Clear();
}

void CVisOp::ClipboardDeleteAll()
{
	CItem *poItem;
	m_oLstClipboard.MoveToFront();
	while( m_oLstClipboard.GetNext( &poItem ) )
	{
		if( poItem )
			delete poItem;
	}
	m_oLstClipboard.Clear();
}

void CVisOp::ProcOp( COp *poOp )
{
	const unsigned int uiOpInCount = poOp->GetCountIn();
	const CItem *poItem = reinterpret_cast<CItem *>( poOp->m_pvData );
	CItem *poItemNext;
	COp *poOpNext;

	poItem->Render();

	if( CVisOp::CItem::m_bIsRender_ )
	{
		for( unsigned int i=0; i<uiOpInCount; ++i )
		{
			COp::CLink & roLink = poOp->In( i );
			if( roLink.IsValid() )
			{
				unsigned int uiPortOut = roLink.GetIndex();
				poOpNext = roLink.GetOp();
				if( poOpNext )
				{
					poItemNext = reinterpret_cast<CItem *>( poOpNext->m_pvData );

					//if( m_bIsRender )
					if( poItemNext )
						poItem->RenderLinkInOut( i, poItemNext, uiPortOut );
				}
			}
		}
	}
}

void CVisOp::SnapToGrid( CList<CItem *> *poLstItem ) const
{
	CItem *poItem;
	poLstItem->MoveToFront();
	while( poLstItem->GetNext( &poItem ) )
	{
		SnapToGrid( poItem );
	}
}

void CVisOp::SnapToGrid( CItem *poItem ) const
{
	GLfloat fX = poItem->GetPos( 0 );
	GLfloat fY = poItem->GetPos( 1 );
	SnapToGrid( &fX, &fY );
	poItem->SetPos( fX, fY );
}

void CVisOp::SnapToGrid( GLfloat *pfX, GLfloat *pfY ) const
{
	*pfX = m_fStpX * rintf( *pfX * ( 1.0f / m_fStpX ) );
	*pfY = m_fStpY * rintf( *pfY * ( 1.0f / m_fStpY ) );
}

bool CVisOp::FileNew( unsigned int uiTemplate )
{
	Clear();
	m_poOrgOp->Clear();

	COp *poOpRootL = m_poOrgOp->CreateShort( "Nop" );
	poOpRootL->SetNameInstance( "l" );
	m_poOrgOp->Insert( poOpRootL );

	COp *poOpRootR = m_poOrgOp->CreateShort( "Nop" );
	poOpRootR->SetNameInstance( "r" );
	m_poOrgOp->Insert( poOpRootR );

	switch( uiTemplate )
	{
	case VIS_OP_FILE_NEW_TEMPLATE_STD0:
	{
		COp *poMin0 = m_poOrgOp->CreateShort( "Value" );
		//poMin0->SetNameInstance( "min" );
		poMin0->Out( 0 ) = -1.0;
		m_poOrgOp->Insert( poMin0 );
		
		COp *poMax0 = m_poOrgOp->CreateShort( "Value" );
		//poMax0->SetNameInstance( "max" );
		poMax0->Out( 0 ) = +1.0;
		m_poOrgOp->Insert( poMax0 );
		
		COp *poMix0 = m_poOrgOp->CreateShort( "Mix" );
		poMix0->SetNameInstance( "mix" );
		//poMix0->In( 0 ) = COp::CLink();
		//poMix0->In( 1 ) = COp::CLink();
		m_poOrgOp->Insert( poMix0 );
		
		COp *poClip0 = m_poOrgOp->CreateShort( "Clip" );
		poClip0->SetNameInstance( "clip" );
		poClip0->In( 0 ) = COp::CLink( poMix0, 1 );
		poClip0->In( 1 ) = COp::CLink( poMax0, 0 );
		poClip0->In( 2 ) = COp::CLink( poMin0, 0 );
		m_poOrgOp->Insert( poClip0 );
		
		COp *poWav0 = m_poOrgOp->CreateShort( "FileWavOut" );
		poWav0->SetNameInstance( "wav" );
		poWav0->In( 0 ) = COp::CLink( poClip0, 0 );
		poWav0->In( 1 ) = COp::CLink( poClip0, 0 );
		m_poOrgOp->Insert( poWav0 );
		
		poOpRootL->In( 0 ) = COp::CLink( poWav0, 0 );
		poOpRootR->In( 0 ) = COp::CLink( poWav0, 1 );
	}
	break;
	case VIS_OP_FILE_NEW_TEMPLATE_STD1:
	{
	}
	break;
	case VIS_OP_FILE_NEW_TEMPLATE_STD2:
	{
	}
	break;
	case VIS_OP_FILE_NEW_TEMPLATE_STD3:
	{
	}
	break;
	default:
		return false;
	}

	m_poOrgOp->SetOpRootLeft_( poOpRootL );
	m_poOrgOp->SetOpRootRight_( poOpRootR );

	m_poOrgOp->UpdateAll();
	m_poOrgOp->ValidateAll();

	m_afPickDefPos[0] = 0.0f;
	m_afPickDefPos[1] = 0.0f;
	m_afCoordTrans[0] = 0.0f;
	m_afCoordTrans[1] = 0.0f;
	m_afCoordTrans[2] = 0.0f;

	Init();

	return true;
}

bool CVisOp::FileLoad( const char *pcFileName )
{
	CFile oFile;
	if( !oFile.Open( pcFileName,  CFile::FLAG_READ ) )
	    return false;
	unsigned int uiSize = oFile.GetSize();
	char *pcPatch = new char[uiSize+1];
	oFile.Read( (void *)(pcPatch), uiSize );
	oFile.Close();
	pcPatch[uiSize] = '\0';

	bool bRet = LoadPatch( pcPatch );
	if( pcPatch )
		delete [] pcPatch;
	if( !bRet )
		FileNew( VIS_OP_FILE_NEW_TEMPLATE_STD0 );
	Update();
	return bRet;
}

bool CVisOp::FileSave( const char *pcFileName )
{
	CFile oFile;
	CStr oStrPatch;
	bool bError = false;
	if( !oFile.Open( pcFileName,  CFile::FLAG_WRITE | CFile::FLAG_TRUNC ) )
	{
		Dialog(
#if defined( VIS_OP_LANGUAGE_ENGLISH )
			"File Error!"
#elif defined( VIS_OP_LANGUAGE_GERMAN )
			"Datei fehlerhaft!"
#endif // VIS_OP_LANGUAGE_*
			, 0, 0, 0 );
		return false;
	}

	m_poOrgOp->SaveAll( oStrPatch );
	this->SaveAll( oStrPatch );

	oFile.Write( (void *)(oStrPatch.GetData()), oStrPatch.GetSize() );
	if( !oFile.GetSize() )
		bError = true;
	oFile.Close();

	VIS_OP_LOG( "save:\n%s\n", oStrPatch.GetData() );

	/*
	SelectAllItems();
	m_afPickDefPos[0] = 0.0f;
	m_afPickDefPos[1] = 0.0f;
	FileSaveMacro( pcFileName );
	UnSelectAllItems();
	*/

	if( bError )
		Dialog(
#if defined( VIS_OP_LANGUAGE_ENGLISH )
			"CAUTION: File is empty!"
#elif defined( VIS_OP_LANGUAGE_GERMAN )
			"ACHTUNG: Datei ist leer!"
#endif // VIS_OP_LANGUAGE_*
			, 0, 0, 0 );

	return bError;
}

bool CVisOp::FileLoadMacro( const char *pcFileName )
{
	CFile oFile;
	if( !oFile.Open( pcFileName,  CFile::FLAG_READ ) )
	    return false;
	unsigned int uiSize = oFile.GetSize();
	char *pcPatch = new char[uiSize+1];
	oFile.Read( (void *)(pcPatch), uiSize );
	oFile.Close();

	pcPatch[uiSize] = '\0';
	VIS_OP_LOG( "load macro:\n%s\n", pcPatch );

	CStr oStrPatch( pcPatch );
	if( pcPatch )
		delete [] pcPatch;

	//this->Clear();
	const unsigned int uiIDMax = COp::GetIDMax_() - 1;
	if( !m_poOrgOp->Load( oStrPatch, uiIDMax ) )
	{
		VIS_OP_LOG( "error: load macro (OrgOp)\n" );
		return false;
	}
	//this->Init();
	InsertUnconnectedItems( 0.0f, 0.0f );

	if( !this->Load( oStrPatch, uiIDMax, m_afPickDefPos[0], m_afPickDefPos[1] ) )
	{
		VIS_OP_LOG( "error: load macro (VisOp)\n" );
		return false;
	}
	return true;
}

bool CVisOp::FileSaveMacro( const char *pcFileName )
{
	CFile oFile;
	CStr oStrPatch;
	if( !oFile.Open( pcFileName,  CFile::FLAG_WRITE | CFile::FLAG_TRUNC ) )
	    return false;
	const unsigned int uiSize = m_oLstSelection.GetSize();
	if( uiSize )
	{
		/*
		CItem *poItem;
		m_oLstSelection.MoveToFront();
		while( m_oLstSelection.GetNext( &poItem ) )
		{
			m_poOrgOp->Save( oStrPatch, poItem->GetOp() );
		}
		*/

		COp **apoOp = new COp *[uiSize];
		unsigned int i = 0;
		CItem *poItem;
		m_oLstSelection.MoveToFront();
		while( m_oLstSelection.GetNext( &poItem ) )
		{
			apoOp[i] = poItem->GetOp();
			++i;
		}
		m_poOrgOp->Save( oStrPatch, apoOp, uiSize );
		delete [] apoOp;

		m_oLstSelection.MoveToFront();
		while( m_oLstSelection.GetNext( &poItem ) )
		{
			this->Save( oStrPatch, poItem, m_afPickDefPos[0], m_afPickDefPos[1] );
		}
	}
	//m_poOrgOp->SaveAll( oStrPatch );
	//this->Save( oStrPatch );
	oFile.Write( (void *)(oStrPatch.GetData()), oStrPatch.GetSize() );
	oFile.Close();
	VIS_OP_LOG( "save macro:\n%s\n", oStrPatch.GetData() );
	return true;
}

bool CVisOp::LoadPatch( const char *pcPatch )
{
	VIS_OP_LOG( "load:\n%s\n", pcPatch );

	CStr oStrPatch( pcPatch );
	this->Clear();
	if( !m_poOrgOp->LoadAll( oStrPatch ) )
	{
		VIS_OP_LOG( "error: load (OrgOp)\n" );
		return false;
	}
	this->Init();
	if( !this->Load( oStrPatch, 0, 0.0f, 0.0f ) )
	{
		VIS_OP_LOG( "error: load (VisOp)\n" );
		return false;
	}

	double adCoord[7];
	unsigned int j = 0;
	while( j < sizeof(adCoord) / sizeof(adCoord[0]) )
	{
		int i = oStrPatch.Find( 0, VIS_OP_DELIM );
		if( i < 0 )
			break;
		oStrPatch.Del( 0, i+1 );
		i = oStrPatch.Find( 0, VIS_OP_DELIM );
		if( i < 0 )
			break;
		CStr oStrNum = oStrPatch.GetSub( 0, i );

		double dNum;
		VIS_OP_SSCANF( oStrNum, "%lf", &dNum );
		adCoord[j] = dNum;
		++j;
	}

	m_afPickDefPos[0] = GLfloat( adCoord[0] );
	m_afPickDefPos[1] = GLfloat( adCoord[1] );
	m_afCoordTrans[0] = GLfloat( adCoord[2] );
	m_afCoordTrans[1] = GLfloat( adCoord[3] );
	m_afCoordTrans[2] = GLfloat( adCoord[4] );
	m_bShowLog = bool( adCoord[5] != 0.0 );
	m_bShowOscilloscope = bool( adCoord[6] != 0.0 );

	return true;
}

bool CVisOp::Dialog( const char *pcMsg, TFuncDialog tFunc, void *ptArg0, unsigned int uiArg1 )
{
	if( m_bIsDialog )
		return false;
	m_bIsDialog = true;
	m_oStrBuffer = pcMsg;
	m_tDialogFunc = tFunc;
	m_pvDialogArg0 = ptArg0;
	m_uiDialogArg1 = uiArg1;
	m_uiTextMode = VIS_OP_TEXT_MODE_NONE;
	return true;
}

void CVisOp::DialogOk()
{
	if( m_bIsDialog )
	{
		m_bIsDialog = false;
		if( m_tDialogFunc )
			m_tDialogFunc( m_pvDialogArg0, m_uiDialogArg1 );
	}
}

void CVisOp::DialogCancel()
{
	m_bIsDialog = false;
}

void CVisOp::PickBegin( unsigned int uiPickMode, unsigned int uiPickID, int iX, int iY )
{
	GLfloat fX, fY;
	m_oGLTool_.WndToGLCoord( iX, iY, &fX, &fY, 0 );
	m_iXLast = iX;
	m_iYLast = iY;

	m_bIsPickDefpos = false;
	VIS_OP_LOG( "pick: " );

	if( m_bIsDialog )
	{
		// x-/y-Translation rückgängig machen.
		GLdouble m_adModelview[16];
		glGetDoublev( GL_MODELVIEW_MATRIX, m_adModelview );
		fX += GLfloat(m_adModelview[12]);
		fY += GLfloat(m_adModelview[13]);

		GLfloat fX0 = -VIS_OP_DIALOG_EXT_X + VIS_OP_ITEM_ELEM_X;
		GLfloat fX1 = fX0 + VIS_OP_ITEM_ELEM_X * 3.0f;
		const GLfloat fY0 = -VIS_OP_DIALOG_EXT_Y * 0.5f - VIS_OP_ITEM_ELEM_Y;
		const GLfloat fY1 = fY0 + VIS_OP_ITEM_ELEM_Y * 3.0f;
		if( fX > fX0 &&
		    fX < fX1 &&
		    fY > fY0 &&
		    fY < fY1 )
		{
			DialogOk();
		}

		fX0 = 0.0f + VIS_OP_ITEM_ELEM_X;
		fX1 = fX0 + VIS_OP_ITEM_ELEM_X * 3.0f;

		if( fX > fX0 &&
		    fX < fX1 &&
		    fY > fY0 &&
		    fY < fY1 )
		{
			m_bIsDialog = false;
			if( m_tDialogFunc )
				m_tDialogFunc( m_pvDialogArg0, 0 );
		}
		return;
	}

	//if( m_poItemPickAction0 && m_iTextID >= 0 )
	if( m_uiTextMode != VIS_OP_TEXT_MODE_NONE )
	{
		if( m_poItemPickAction0 && m_oStrBuffer.GetSize() )
		{
			m_poItemPickAction0->SetText( m_iTextID, m_oStrBuffer );
		}
		m_poItemPickAction0 = 0;
		m_poItemPickAction1 = 0;
		m_iTextID = -1;
		m_oStrBuffer = "";
		m_uiTextMode = VIS_OP_TEXT_MODE_NONE;
	}

	switch( uiPickMode )
	{
	case VIS_OP_PICK_MODE_MOVE:
	{
		m_poItemPickMove = SearchItem( uiPickID );
		m_afPickMovePos[0] = fX;
		m_afPickMovePos[1] = fY;
		m_iPickItemPortIn = m_poItemPickMove->GetPortIn( fX, fY );
		m_iPickItemPortOut = m_poItemPickMove->GetPortOut( fX, fY );
		const int iRoutine = m_poItemPickMove->CallRoutine( fX, fY );

		// Port-Funktion betätigt; verarbeiten, und weiter Verarbeitung verbieten.
		if( iRoutine >= 0 )
		{
			VIS_OP_LOG( "routine: " ); // %s\n", m_poItemPickMove->GetOp()->GetNameRoutine( iRoutine ) );
			//m_poItemPickCurr = m_poItemPickMove; // ACHTUNG: Neu!
			m_poItemPickMove = 0;
		}
		// Link (out->in)
		else if( m_iPickItemPortOut >= 0 )
		{
			m_poItemPickMoveLinkOut0 = m_poItemPickMove;
			m_poItemPickMoveLinkOut1 = 0;
			VIS_OP_LOG( "move link (out %d): ", m_iPickItemPortOut );

			m_poItemPickCurr = m_poItemPickMove;
			m_poItemPickMove = 0;

			UnSelectAllItems();
		}
		// Link (in->out, oder out->in, wenn schon präsent)
		else if( m_iPickItemPortIn >= 0 )
		{
			COp::CLink &roLink = m_poItemPickMove->GetOp()->In( m_iPickItemPortIn );
			COp *poInputPresent = roLink.GetOp();

			// An Input-Port ist schon ein Item.
			if( poInputPresent )
			{
				//m_iPickItemPortIn = -1;
				m_iPickItemPortOut = roLink.GetIndex();
				m_poItemPickMoveLinkOut0 =
					reinterpret_cast<CItem *>( poInputPresent->m_pvData );
				m_poItemPickMoveLinkOut1 = 0;
				m_poItemPickMove->DisconnectIn( m_iPickItemPortIn );
				VIS_OP_LOG( "move link (in %d, out %d): ", m_iPickItemPortIn, m_iPickItemPortOut );
			}
			// Input-Port ist noch frei.
			else
			{
				m_iPickItemPortOut = -1;
				m_poItemPickMoveLinkIn0 = m_poItemPickMove;
				m_poItemPickMoveLinkIn1 = 0;
				VIS_OP_LOG( "move link (in %d): ", m_iPickItemPortIn );
			}
			m_poItemPickCurr = m_poItemPickMove;
			m_poItemPickMove = 0;

			UnSelectAllItems();
		}
		// Item(s) verschieben und für andere Operationen selektieren.
		else
		{
			if( m_bFuncKeyA )
			{
				if( m_poItemPickCurr && !m_oLstSelection.GetSize() )
					SelectItem( m_poItemPickCurr );

				if( !SelectItem( m_poItemPickMove ) )
				{
					UnSelectItem( m_poItemPickMove );
					m_poItemPickMove = 0;
				}
			}
			// Item ausserhalb der Auswahl wird ohne Funktions-Taste angewählt.
			else if( m_oLstSelection.FindFirst( m_poItemPickMove ) < 0 )
			{
				UnSelectAllItems();
				//if( m_poItemPickCurr )
				//	UnSelectItem( m_poItemPickCurr );
				SelectItem( m_poItemPickMove );
			}

			// Alle Position bezügliche Move-Action auf Grid setzen.
			SnapToGrid( &m_afPickMovePos[0], &m_afPickMovePos[1] );
			if( m_oLstSelection.GetSize() )
				SnapToGrid( &m_oLstSelection );
			else if( m_poItemPickMove )
				SnapToGrid( m_poItemPickMove );

			m_poItemPickCurr = m_poItemPickMove;

			// Wenn es sich um eine einzelnen Operator mit dem OP_FLAG_SHOW_OUTPUTS
			// (i.d.R COpValue) handelt, kann der erste Output-Wert direkt verändert werden.
			if( m_poItemPickMove
			 && m_oLstSelection.GetSize() <= 1
		         && m_poItemPickMove->CheckPos( fX, fY ) // Nicht im Titelfeld.
			 && m_poItemPickMove->GetOp()->GetFlags() & OP_FLAG_SHOW_OUTPUTS )
			{
				m_poItemPickAction0 = m_poItemPickMove;
				m_iTextID = VIS_OP_ITEM_TEXT_PORT_OUT + 0;
				m_oStrBuffer = m_poItemPickAction0->GetText( m_iTextID );
				m_uiTextMode = VIS_OP_TEXT_MODE_ITEM;
				m_bTextSelectAll = true;
				m_bTextProtected = false;
			}

			VIS_OP_LOG( "move: " );
		}
	}
	break;
	case VIS_OP_PICK_MODE_ACTION0:
	{
		m_poItemPickCurr = SearchItem( uiPickID );
		m_iTextID = m_poItemPickCurr->CheckText( fX, fY );
		if( m_iTextID >= 0 )
		{
			m_poItemPickAction0 = m_poItemPickCurr;
			m_oStrBuffer = m_poItemPickAction0->GetText( m_iTextID );
			m_uiTextMode = VIS_OP_TEXT_MODE_ITEM;
			m_bTextSelectAll = true;
			m_bTextProtected = false;

			VIS_OP_LOG( "action0: " );
		}
		else
		{
			m_poItemPickCurr = m_poItemPickAction0 = 0;
			m_iTextID = -1;
		}
	}
	break;
	case VIS_OP_PICK_MODE_ACTION1:
	{
		m_poItemPickCurr = m_poItemPickAction1 = SearchItem( uiPickID );
	}
	break;
	case VIS_OP_PICK_MODE_TWIDDLE:
	{
		if( uiPickID )
		{
			m_afPickTwiddleStart[0] = fX;
			m_afPickTwiddleStart[1] = fY;
			m_poItemPickCurr = m_poItemPickTwiddle = SearchItem( uiPickID );
			VIS_OP_LOG( "twiddle: " );
		}
	}
	break;
	case VIS_OP_PICK_MODE_CONTEXT0:
	{
		if( m_poItemPickMoveLinkOut0 || m_poItemPickMoveLinkIn0 )
			break;
		const GLfloat fXMin = GLfloat(m_oGLTool_.m_adRightBottom[0])
			- VIS_OP_CONTEXT0_EXT_X * ( 1 + m_poOrgOp->GetCountOp() / VIS_OP_CONTEXT0_MAX_Y )
			- VIS_OP_ITEM_ELEM_X;
		const GLfloat fYMin = GLfloat(m_oGLTool_.m_adRightBottom[1])
			+ VIS_OP_CONTEXT0_EXT_Y * ( 1 + VIS_OP_CONTEXT0_MAX_Y )
			+ VIS_OP_ITEM_ELEM_Y;
		m_afPickContext0Start[0] = MIN( fX, fXMin );
		m_afPickContext0Start[1] = MAX( fY, fYMin );
		m_bIsPickContext0 = true;
		VIS_OP_LOG( "context0: " );
	}
	break;
	case VIS_OP_PICK_MODE_DEFPOS:
	{
		m_bIsPickDefpos = true;
		m_afPickDefPos[0] = fX;
		m_afPickDefPos[1] = fY;
		m_afPickDefPos[2] = m_afPickDefPos[3] = 0.0f;
		VIS_OP_LOG( "defpos: %f, %f\n", fX, fY );
		m_poItemPickCurr = 0;

		// Mehrfach selektion bei FuncKeyA erlauben.
		if( !m_bFuncKeyA )
		{
			UnSelectAllItems();
		}
	}
	break;
	};

	if( m_poItemPickCurr )
	{
		//VIS_OP_LOG( "%s\n", m_poItemPickCurr->Info().GetData() );
	}
}

void CVisOp::PickProc( int iX, int iY )
{
	GLfloat fX, fY;
	m_oGLTool_.WndToGLCoord( iX, iY, &fX, &fY, 0 );

	if( m_poItemPickMove )
	{
		if( !m_bFuncKeyB )
			SnapToGrid( &fX, &fY );

		const GLfloat fXDiff = fX - m_afPickMovePos[0];
		const GLfloat fYDiff = fY - m_afPickMovePos[1];

		if( m_oLstSelection.GetSize() )
		{
			CItem *poItem;
			m_oLstSelection.MoveToFront();
			while( m_oLstSelection.GetNext( &poItem ) )
			{
				poItem->TranslatePos( fXDiff, fYDiff );
			}
		}
		else
		{
			m_poItemPickMove->TranslatePos( fXDiff, fYDiff );
		}

		m_afPickMovePos[0] = fX;
		m_afPickMovePos[1] = fY;
	}
	else if( m_poItemPickMoveLinkIn0 )
	{
		m_poItemPickMoveLinkIn1 = 0;

		CItem *poItem;
		m_poItemPickMoveLinkIn0->RenderLinkIn( m_iPickItemPortIn, fX, fY );
		m_oLstItem.MoveToFront();
		while( m_oLstItem.GetNext( &poItem ) )
		{
			if( poItem->CheckPos( fX, fY ) )
			{
				int iPortOut = poItem->GetPortOut( fX, fY );
				if( iPortOut >= 0 )
				{
					VIS_OP_LOG( "port out %d\n", iPortOut );
					m_iPickItemPortOut = iPortOut;
					m_poItemPickMoveLinkIn1 = poItem;
					break;
				}
			}
		}

		m_afPickMoveLink[0] = fX;
		m_afPickMoveLink[1] = fY;
	}
	else if ( m_poItemPickMoveLinkOut0 )
	{
		m_poItemPickMoveLinkOut1 = 0; // So können links auch gelöscht werden.

		CItem *poItem;
		m_poItemPickMoveLinkOut0->RenderLinkOut( m_iPickItemPortOut, fX, fY );
		m_oLstItem.MoveToFront();
		while( m_oLstItem.GetNext( &poItem ) )
		{
			if( poItem->CheckPos( fX, fY ) )
			{
				int iPortIn = poItem->GetPortIn( fX, fY );
				if( iPortIn >= 0 )
				{
					VIS_OP_LOG( "port in %d\n", iPortIn );
					m_iPickItemPortIn = iPortIn;
					m_poItemPickMoveLinkOut1 = poItem;
					break;
				}
			}
		}

		m_afPickMoveLink[0] = fX;
		m_afPickMoveLink[1] = fY;
	}
	if( m_poItemPickAction0 )
	{
	}
	if( m_poItemPickAction1 )
	{
		//m_poItemPickAction1->GetOp()->Reset();
	}
	if( m_poItemPickTwiddle )
	{
		//GLfloat fDiffX = fX - m_afPickTwiddleStart[0];
		GLfloat fDiffY = fY - m_afPickTwiddleStart[1];
		// TODO: Auch andere Output-Ports zugänglich machen!
		double &rdVal = m_poItemPickTwiddle->GetOp()->Out( 0 );
		if( m_bFuncKeyA )
			fDiffY *= VIS_OP_TWIDDLE_SMALL;
		if( m_bFuncKeyB )
			fDiffY *= VIS_OP_TWIDDLE_LARGE;
		else
			fDiffY *= VIS_OP_TWIDDLE_MEDIUM;

		rdVal += fDiffY;

		if( m_bFuncKeyB )
			modf( rdVal, &rdVal );

		VIS_OP_LOG( "\t%.14g (out 0)\n", m_poItemPickTwiddle->GetOp()->Out( 0 ) );
	}
	if( m_bIsPickContext0 )
	{
		m_iPickContext0 = -1;

		if( fX > m_afPickContext0Start[0] && fY < m_afPickContext0Start[1]
		 && fY > ( m_afPickContext0Start[1] - VIS_OP_CONTEXT0_MAX_Y * VIS_OP_ITEM_ELEM_Y ) )
		{
			unsigned int uiCol = int( ( fX - m_afPickContext0Start[0] ) / VIS_OP_CONTEXT0_EXT_X );
			unsigned int uiRow = int( ( m_afPickContext0Start[1] - fY ) / VIS_OP_CONTEXT0_EXT_Y );
			m_iPickContext0 = uiCol * VIS_OP_CONTEXT0_MAX_Y + uiRow;
			m_afPickContext0Insert[0] = fX - VIS_OP_ITEM_STD_X * 0.5f;
			m_afPickContext0Insert[1] = fY - VIS_OP_ITEM_ELEM_Y;
		}
	}
	if( m_bIsPickDefpos )
	{
		GLfloat fX0 = MIN( fX, m_afPickDefPos[0] );
		GLfloat fX1 = MAX( fX, m_afPickDefPos[0] );
		GLfloat fY0 = MIN( fY, m_afPickDefPos[1] );
		GLfloat fY1 = MAX( fY, m_afPickDefPos[1] );
		CItem *poItem;

		if( !m_bFuncKeyA )
		{
			UnSelectAllItems();
		}

		m_oLstItem.MoveToFront();
		while( m_oLstItem.GetNext( &poItem ) )
		{
			if( poItem->CheckArea( fX0, fY0, fX1, fY1 ) )
			{
				SelectItem( poItem );
			}
		}
		m_afPickDefPos[2] = fX;
		m_afPickDefPos[3] = fY;
	}
}

void CVisOp::PickEnd( unsigned int uiPickMode )
{
	switch( uiPickMode )
	{
	case VIS_OP_PICK_MODE_MOVE:
		if( m_poItemPickMoveLinkIn0 && m_poItemPickMoveLinkIn1 )
		{
			VIS_OP_LOG( "\t-> %s", m_poItemPickMoveLinkIn1->Info().GetData() );
			VIS_OP_LOG( " (out %d)\n", m_iPickItemPortOut );

			CItem::Connect_( m_poItemPickMoveLinkIn0, m_iPickItemPortIn, m_poItemPickMoveLinkIn1, m_iPickItemPortOut );
		}
		if( m_poItemPickMoveLinkOut0 && m_poItemPickMoveLinkOut1 )
		{
			VIS_OP_LOG( "\t-> %s", m_poItemPickMoveLinkOut1->Info().GetData() );
			VIS_OP_LOG( " (in %d)\n", m_iPickItemPortIn );

			CItem::Connect_( m_poItemPickMoveLinkOut1, m_iPickItemPortIn, m_poItemPickMoveLinkOut0, m_iPickItemPortOut );
		}

	// Wenn Link gezogen wird (MODE_MOVE) und dann Rechtsclick folgt wird dies als MODE_TWIDDLE interpretiert,
	// somit müssen hier die Link-Zeiger auch resetet werden.
	case VIS_OP_PICK_MODE_TWIDDLE:
		m_poItemPickMove = 0;
		m_poItemPickMoveLinkIn0 = 0;
		m_poItemPickMoveLinkIn1 = 0;
		m_poItemPickMoveLinkOut0 = 0;
		m_poItemPickMoveLinkOut1 = 0;
		m_iPickItemPortIn = 0;

		m_poItemPickTwiddle = 0;
	break;
	case VIS_OP_PICK_MODE_ACTION0:
		// rem: Wird in PickBegin() deaktiviert, sobald eine andere Pick-Meldung eintrifft.
		//m_poItemPickAction0 = 0;
		// Text-Modus nicht hier deaktivieren.
	break;
	case VIS_OP_PICK_MODE_ACTION1:
		//siehe m_poItemPickAction0.
		//m_poItemPickAction1 = 0;
	break;
	case VIS_OP_PICK_MODE_CONTEXT0:
		m_bIsPickContext0 = false;
		if( m_iPickContext0 >= 0 && (unsigned int)m_iPickContext0 < m_poOrgOp->GetCountOp() )
		{
			VIS_OP_LOG( "class %d\n", m_iPickContext0 );
			COp *poOpNew = m_poOrgOp->CreateShort( m_poOrgOp->GetNameClassShort( m_iPickContext0 ) );
			if( poOpNew )
			{
				//CStr oName = poOpNew->GetNameClass();
				//oName.Del( 0, 3 );
				//poOpNew->SetNameInstance( oName );
				m_afPickDefPos[0] = m_afPickContext0Insert[0];
				m_afPickDefPos[1] = m_afPickContext0Insert[1];

				SnapToGrid( &m_afPickDefPos[0], &m_afPickDefPos[1] );

				UnSelectAllItems();
				m_poOrgOp->Insert( poOpNew );
				CItem *poItemNew = new CItem( 0, 0, poOpNew, VIS_OP_ITEM_TYPE_STD,
					m_afPickDefPos[0], m_afPickDefPos[1] );
				InsertItem( poItemNew );
				/*
				COp *poOpValue = m_poOrgOp->CreateShort( "Value" );
				poOpValue->SetNameInstance( "_" );
				m_poOrgOp->Insert( poOpValue );

				if( m_poItemPickMoveLinkIn0 )
				{
					m_afPickDefPos[0] = m_afPickMoveLink[0] - VIS_OP_ITEM_STD_X * 0.9f;
					m_afPickDefPos[1] = m_afPickMoveLink[1] - VIS_OP_ITEM_ELEM_Y;
				}
				else
				{
					m_afPickDefPos[0] -= VIS_OP_ITEM_STD_X * 0.5f;
					m_afPickDefPos[1] -= VIS_OP_ITEM_ELEM_Y;
				}
				CItem *poItem = new CItem( 0, 0, poOpValue, VIS_OP_ITEM_TYPE_STD, m_afPickDefPos[0], m_afPickDefPos[1] );
				InsertItem( poItem );
				*/
				if( !( poOpNew->GetFlags() & OP_FLAG_COMBINATIVE ) )
				{
					const GLfloat fXPos = m_afPickDefPos[0] - VIS_OP_ITEM_STD_X - m_fStpX;
					GLfloat fYPos = m_afPickDefPos[1] + poItemNew->GetHeight()
						- 6.0f * m_fStpY - VIS_OP_ITEM_ELEM_Y;
					const unsigned int uiPortInMax = poOpNew->GetCountIn();
					unsigned int uiPortIn = 0;
					while( ++uiPortIn < uiPortInMax )
					{
						if( poOpNew->In( uiPortIn ).GetFlags() & OP_LINK_FLAG_OPTIONAL )
							continue;

						COp *poOpValue = m_poOrgOp->CreateShort( "Value" );
						poOpValue->SetNameInstance( "" );
						m_poOrgOp->Insert( poOpValue );

						CItem *poItemValue = new CItem( 0, 0, poOpValue, VIS_OP_ITEM_TYPE_STD,
									fXPos,
									fYPos );
						InsertItem( poItemValue );

						CItem::Connect_( poItemNew, uiPortIn, poItemValue, 0 );
						fYPos -= m_fStpY * 3.0f;
					}
				}
			}
		}
		else
		{
			VIS_OP_LOG( "-\n" );
		}
		m_iPickContext0 = -1;
	break;
	case VIS_OP_PICK_MODE_DEFPOS:
		m_bIsPickDefpos = false;
	break;
	};
}

void CVisOp::CoordTranslate( float fX, float fY )
{
	m_afCoordTrans[0] += fX * GLfloat( m_oGLTool_.m_dPixelExtX ); //iXUnit * VIS_OP_ITEM_ELEM_X;
	m_afCoordTrans[1] += fY * GLfloat( m_oGLTool_.m_dPixelExtY ); //iYUnit * VIS_OP_ITEM_ELEM_Y;
	Update();
}

void CVisOp::CoordUpdate()
{
	glLoadIdentity();
	glTranslatef( m_afCoordTrans[0], m_afCoordTrans[1], m_afCoordTrans[2] );
}

void CVisOp::KeyFunctionA( bool bState )
{
	// Schutz vor Halte-Funktion.
	//if( !m_bFuncKeyA && bState )
	//{
	//	m_po...Trigg = true;
	//}
	m_bFuncKeyA = bState;
}

void CVisOp::KeyFunctionB( bool bState ) {
	m_bFuncKeyB = bState;
}

void CVisOp::KeyFunctionInfo()
{
	CItem *poItemPick = GetSelectionSingle();

	if( m_uiTextMode == VIS_OP_TEXT_MODE_NONE ) // !m_bTextProtected )
	{
		if( poItemPick )
		{
			COp *poOp = poItemPick->GetOp();
			if( poOp )
			{
				unsigned int uiMax, uiPort;

				m_uiTextMode = VIS_OP_TEXT_MODE_INFO;
				m_oStrBuffer = poOp->GetInfoClass();
				m_oStrBuffer += "\n";
				
				uiMax = poOp->GetCountIn();
				uiPort = 0;
				if( uiMax )
					m_oStrBuffer += "\nIN:\n";
				while( uiPort < uiMax )
				{
					m_oStrBuffer += poOp->GetNameInput( uiPort );
					m_oStrBuffer += ": ";
					m_oStrBuffer += poOp->GetInfoInput( uiPort );
					m_oStrBuffer += "\n";
					++uiPort;
				}

				uiMax = poOp->GetCountOut();
				uiPort = 0;
				if( uiMax )
					m_oStrBuffer += "\nOUT:\n";
				while( uiPort < uiMax )
				{
					m_oStrBuffer += poOp->GetNameOutput( uiPort );
					m_oStrBuffer += ": ";
					m_oStrBuffer += poOp->GetInfoOutput( uiPort );
					m_oStrBuffer += "\n";
					++uiPort;
				}

				uiMax = poOp->GetCountRoutine();
				uiPort = 0;
				if( uiMax )
					m_oStrBuffer += "\nROUTINE:\n";
				while( uiPort < uiMax )
				{
					m_oStrBuffer += poOp->GetNameRoutine( uiPort );
					m_oStrBuffer += ": ";
					m_oStrBuffer += poOp->GetInfoRoutine( uiPort );
					m_oStrBuffer += "\n";
					++uiPort;
				}

				m_oStrBuffer += "\n(";
				m_oStrBuffer += poItemPick->Info();
				m_oStrBuffer += ")";

				m_bTextSelectAll = false;
				m_bTextProtected = true;
			}
		}
		else
		{
			static char acTmp_[256];
			
			m_uiTextMode = VIS_OP_TEXT_MODE_INFO;

			m_oStrBuffer = APP_NAME;
			m_oStrBuffer += "\n";
			
			m_oStrBuffer += "op/VisOp version: ";
			m_oStrBuffer += OP_VERSION;
			m_oStrBuffer += "/";
			m_oStrBuffer += VIS_OP_VERSION;
			m_oStrBuffer += "\n";
			
			m_oStrBuffer += "build: ";
			m_oStrBuffer += __DATE__;
#if defined( __WINDOWS__ )
#if	defined( __GNUC__ )
#if		defined( __MINGW32_VERSION )
			VIS_OP_SPRINTF( acTmp_, sizeof( acTmp_ ), " (GCC/MinGW %d.%d)\n", __MINGW32_MAJOR_VERSION, __MINGW32_MINOR_VERSION );
			m_oStrBuffer += acTmp_;
#else
			VIS_OP_SPRINTF( acTmp_, sizeof( acTmp_ ), " (GCC/MinGW %d)\n", __GNUC__ );
			m_oStrBuffer += acTmp_;
#endif // __MINGW32_VERSION
#elif	defined( _MSC_VER )
			VIS_OP_SPRINTF( acTmp_, sizeof( acTmp_ ), " (MSVC %d)\n", _MSC_VER ); // _MSC_FULL_VER );
			m_oStrBuffer += acTmp_;
			
#else
			m_oStrBuffer += " (unkown compiler)\n";
#endif // compiler
#elif defined( __LINUX__ )
#if	defined( __GNUC__ )
			VIS_OP_SPRINTF( acTmp_, sizeof( acTmp_ ), " (GCC %d.%d.%d)\n",
				__GNUC__,
				__GNUC_MINOR__,
				__GNUC_PATCHLEVEL__ );
			m_oStrBuffer += acTmp_;
#else
			m_oStrBuffer += " (unkown compiler)\n";
#endif // compiler
#else
#error platform not supported
#endif // platform
			
			//VIS_OP_SPRINTF( acTmp_, sizeof( acTmp_ ), "%d", AUDIO_SAMPLE_FREQ );
			//m_oStrBuffer += "audio sampling frequency: ";
			//m_oStrBuffer += acTmp_;
			//m_oStrBuffer += "\n";
			
			VIS_OP_SPRINTF( acTmp_, sizeof( acTmp_ ), "%d", AUDIO_BUFFER_SIZE );
			m_oStrBuffer += "audio buffer size: ";
			m_oStrBuffer += acTmp_;
			m_oStrBuffer += "\n";
			
			m_oStrBuffer += "author: ";
			m_oStrBuffer += APP_AUTHOR;
			m_oStrBuffer += "\n";
			
			m_oStrBuffer += "website: ";
			m_oStrBuffer += APP_WEBSITE;
			m_oStrBuffer += "\n";
			
			m_oStrBuffer += "\n";
			
			m_oStrBuffer += "  * the patching interface:\n";
			m_oStrBuffer += "    - create an operator -> SPACE / MOUSE-L\n";
			m_oStrBuffer += "    - connect and move operators -> MOUSE-L (click / drag)\n";
			m_oStrBuffer += "    - delete operators -> BACKSPACE\n";
			m_oStrBuffer += "    - create value operators -> V-KEY (shortcut)\n";
			m_oStrBuffer += "    - change values -> MOUSE-R (CTRL/SHIT modifies step size)\n";
			m_oStrBuffer += "    - move the patch -> MOUSE-M or ARROW-KEYs\n";
			m_oStrBuffer += "    - save the patch -> CTRL-S, CTRL-SHIFT-S or F5/6-KEY\n";
			m_oStrBuffer += "    - load patches -> DRAG'N'DROP files (hold CTRL to add it only)\n";
			m_oStrBuffer += "    - load patches -> F7/8-KEY or CTRL-O\n";
			m_oStrBuffer += "    - select/move operators -> MOUSE-L / DRAG\n";
			m_oStrBuffer += "    - select all -> CTRL-A\n";
			m_oStrBuffer += "    - cut/copy/paste operators -> CTRL-X/-C/-V (+ replace single operators)\n";
			m_oStrBuffer += "    - reset/resync patch or selected operators if there are any -> U-KEY\n";
			m_oStrBuffer += "    - exit the program -> ESCAPE\n";
			m_oStrBuffer += "\n";
			
			m_oStrBuffer += "  * all operators have a simple common interface:\n";
			m_oStrBuffer += "    - inlets on the left (optional inputs are indicated with ')\n";
			m_oStrBuffer += "    - outlets on the right\n";
			m_oStrBuffer += "    - routines on the right (indicated with '()') -> MOUSE-L\n";
			m_oStrBuffer += "    - info for a selected operator -> F1-KEY\n";
			m_oStrBuffer += "    - rename by MOUSE-L-DOUBLE-CLICK on the title\n";
			m_oStrBuffer += "    - see the current output value by MOUSE-L-DOUBLE-CLICK on the output name\n";
			m_oStrBuffer += "    - MOUSE-L-CLICK on value operators to enter a number (fractions can be\n";
			m_oStrBuffer += "      entered like x/y), or MOUSE-R-CLICK-AND-DRAG (+CTRL/SHIT).\n";
			m_oStrBuffer += "    - assign colors to selected operators with [0,9]-KEYs\n";
			m_oStrBuffer += "\n";
			
			m_oStrBuffer += "  * some special functions:\n";
			m_oStrBuffer += "    - toggle fullscreen mode -> ALT-ENTER\n";
			m_oStrBuffer += "    - program info -> F1-KEY\n";
			m_oStrBuffer += "    - toggle oscilloscope -> F2-KEY\n";
			m_oStrBuffer += "    - toggle console -> F3-KEY\n";
			m_oStrBuffer += "    - make screenshot -> F4-KEY\n";
			m_oStrBuffer += "    - load the standard template -> F9-KEY\n";
			m_oStrBuffer += "    - record wave files with OpFileWav\n";
			m_oStrBuffer += "    - whole MIDI functionality including file support\n";
			m_oStrBuffer += "      over OpMidi*\n";
			m_oStrBuffer += "    - OpSys/OpInter provides some special functionality\n";
			m_oStrBuffer += "      (quit trigger, signal, console output, library-access...)\n";
			m_oStrBuffer += "\n";
			
			m_bTextSelectAll = false;
			m_bTextProtected = true;
		}
	}
}

void CVisOp::KeyChar( char cKey )
{
	static bool bIsCopy_;

	// Text-Modus.
	if( m_uiTextMode != VIS_OP_TEXT_MODE_NONE && !m_bTextProtected ) // && m_poItemPickAction0 && m_iTextID >= 0 )
	{
		if( cKey == 0x04 ) // ASCII: Ctrl-D.
			m_bTextSelectAll = false;
		else if( m_bTextSelectAll && ( cKey == 0x08 || cKey >= 32 )) // ASCII Backspace oder normales Zeichen.
			m_oStrBuffer.Clear();

		if( cKey == 0x08) // ASCII Backspace
		{
			m_oStrBuffer.Pop();
		}
		else if( cKey == '\r' || cKey == '\n' )
		{
			if( m_oStrFileName.GetSize() )
			{
				if( m_uiTextMode & VIS_OP_TEXT_MODE_FILE_NAME_SAVE )
				{
					m_oStrFileName = m_oStrBuffer;
					FileSave( m_oStrBuffer.GetData() ); // FileSaveMacro
				}
				else if( m_uiTextMode & VIS_OP_TEXT_MODE_FILE_NAME_LOAD )
				{
					m_oStrFileName = m_oStrBuffer;
					FileLoad( m_oStrBuffer.GetData() ); // FileLoadMacro
				}
			}

			if( m_poItemPickAction0 )
			{
				m_poItemPickAction0->SetText( m_iTextID, m_oStrBuffer );
			}
			else
			{
				m_iTextID = 0;
				m_uiTextMode = VIS_OP_TEXT_MODE_NONE;
			}
			return;
		}
		else if( cKey >= 32 ) // Kein ASCII Sonder-Zeichen.
		{
			m_oStrBuffer += cKey;
		}

		m_bTextSelectAll = ( cKey == 0x01 ); // ASCII: Ctrl-A.
	}
	// Wenn nicht im Text-Modus.
	else
	{
		if( cKey == 0x13 || cKey == 0x0F ) // ASCII: Ctrl-S/-O.
		{
			if( cKey == 0x13 && !m_bFuncKeyA )
			{
				m_oStrFileName = "patch_";
				m_oTime.Update();
				m_oStrFileName += m_oTime.GetString();
				m_oStrFileName += ".txt";
				FileSave( m_oStrFileName.GetData() );
			}
			else
			{
				m_oStrBuffer = m_oStrFileName;
				m_uiTextMode =
					( cKey == 0x13 )
					?( VIS_OP_TEXT_MODE_FILE_NAME_SAVE )
					:( VIS_OP_TEXT_MODE_FILE_NAME_LOAD );

				m_bTextSelectAll = true;
				m_bTextProtected = false;
				m_poItemPickAction0 = 0;
			}
		}
		else if( cKey == 0x08 ) // ASCII: Backspace.
		{
			if( m_oLstSelection.GetSize() )
			{
				if( UnSelectItem( m_poItemRootL ) )
					m_poItemRootL->DisconnectIn( 0 );
				if( UnSelectItem( m_poItemRootR ) )
					m_poItemRootR->DisconnectIn( 0 );

				CItem *poItem;
				m_oLstSelection.MoveToFront();
				while( m_oLstSelection.GetNext( &poItem ) )
				{
					RemoveItem( poItem );
				}
				// Hier kein UnSelectAllItems(), da bereits gelöscht!
				m_oLstSelection.Clear();
			}
			else if( m_poItemPickCurr && m_poItemPickCurr->GetType() & VIS_OP_ITEM_TYPE_ROOT )
			{
				RemoveItem( m_poItemPickCurr );
			}
			m_poItemPickCurr = 0;
		}
		else if( cKey == 0x03 || cKey == 0x18 )// ASCII: Ctrl-C. oder Ctrl-X.
		{
			// rem: Auch bei keiner Auswahl!
			ClipboardDeleteAll();

			UnSelectItem( m_poItemRootL );
			UnSelectItem( m_poItemRootR );

			m_oLstSelection.MoveToFront();
			while( m_oLstSelection.Next() )
			{
				CItem *poItem = m_oLstSelection.Get();
				COp *poOp = poItem->GetOp();
				if( poOp && ( poOp->GetFlags() & OP_FLAG_SINGLETON ) )
				{
					UnSelectItem( poItem );
				}
			}

			if( m_oLstSelection.GetSize() )
			{
				ClipboardCopyTo( &m_oLstSelection );
			}
			else if( m_poItemPickCurr )
			{
				COp *poOp = m_poItemPickCurr->GetOp();
				if( poOp && ( poOp->GetFlags() & OP_FLAG_SINGLETON ) )
					return;

				CList<CItem *> oLstItem;
				oLstItem.Append( m_poItemPickCurr );
				ClipboardCopyTo( &oLstItem );
			}
			
			if( cKey == 0x03 ) // Ctrl-C.
			{
				bIsCopy_ = true;
				VIS_OP_LOG( "key: ctrl-c: %d items\n", m_oLstClipboard.GetSize() );
			}
			else // Ctrl-X.
			{
				CItem *poItem;
				m_oLstSelection.MoveToFront();
				while( m_oLstSelection.GetNext( &poItem ) )
				{
					RemoveItem( poItem );
				}
				m_oLstSelection.Clear(); // Gegen (Ctrl-X dann Ctrl-C)-Fehler. Nicht UnSelectAllItems(), da gelöscht!
				m_poItemPickCurr = 0; // ACHTUNG: Neu! (2009-11-28)
				bIsCopy_ = false;
				VIS_OP_LOG( "key: ctrl-x: %d items\n", m_oLstClipboard.GetSize() );
			}
			//UnSelectAllItems();
		}
		else if( cKey == 0x16 && m_oLstClipboard.GetSize() ) // ASCII: Ctrl-V.
		{
			VIS_OP_LOG( "key: ctrl-v: %d item(s)\n", m_oLstClipboard.GetSize() );

			// Ein Item ersetzen.
			if( m_oLstClipboard.GetSize() == 1 )
			{
				CItem *poItemPick = GetSelectionSingle();

				if( poItemPick
				 && poItemPick != m_poItemRootL
				 && poItemPick != m_poItemRootR )
				{
					CItem *poItem = m_oLstClipboard[0];
					const GLfloat fX = poItemPick->GetPos( 0 );
					const GLfloat fY = poItemPick->GetPos( 1 );
					poItem->SetPos( fX, fY );
					m_afPickDefPos[0] = fX;
					m_afPickDefPos[1] = fY;

					poItem->CopyConnections( poItemPick );

					// Output-Werte kopieren.
					COp *poOp  = poItem->GetOp();
					COp *poOpPick = poItemPick->GetOp();
					if( poOp && poOpPick )
					{
						const unsigned int uiC0 = poOp->GetCountOut();
						const unsigned int uiC1 = poOpPick->GetCountOut();
						const unsigned int uiCount = MIN( uiC0, uiC1 );
						
						VIS_OP_MEM_COPY(
							&poOpPick->Out( 0 ),
							&poOp->Out( 0 ),
							uiCount * sizeof(double) );
					}

					UnSelectItem( poItemPick );
					RemoveItem( poItemPick );
					bIsCopy_ = false;
				}

				m_poItemPickCurr = 0; // ACHTUNG: Neu!
				m_poItemPickMove = 0; // ACHTUNG: Neu!
			}

			UnSelectAllItems();
			ClipboardPasteFrom(); // rem: Paste-Items werden selektiert. roText
			if( bIsCopy_ )
			{
				m_afPickDefPos[0] += m_fStpX;
				m_afPickDefPos[1] += m_fStpY;
			}
		}
		else if( cKey == 0x01 ) // ASCII: Ctrl-A.
		{
			VIS_OP_LOG( "key: ctrl-a: %d item(s)\n", m_oLstItem.GetSize() );
			SelectAllItems();
		}
		else if( cKey == 0x04 ) // ASCII: Ctrl-D.
		{
			VIS_OP_LOG( "key: ctrl-d: %d item(s)\n", m_oLstSelection.GetSize() );
			UnSelectAllItems();
		}
		/*
		else if( cKey == ' ' ) // ASCII: Space.
		{
			if( m_oLstSelection.GetSize() )
			{
				m_oLstSelection.MoveToFront();
				while( m_oLstSelection.Next() )
					m_oLstSelection.Get()->NextState();
			}
			else if( m_poItemPickCurr )
			{
				m_poItemPickCurr->NextState();
			}
		}
		*/
		else if( isdigit( (unsigned char)cKey ) )
		{
			static GLubyte aubColorMap_[] =
			{
				0xFF, 0xFF, 0xFF,
				0x00, 0x00, 0x00,
				0xFF, 0x00, 0x00,
				0x00, 0xFF, 0x00,
				0xFF, 0xFF, 0x00,
				0x00, 0x00, 0xFF,
				0xFF, 0x00, 0xFF,
				0x00, 0xFF, 0xFF,
			};
			static const unsigned int uiMapCount_ = sizeof(aubColorMap_) / sizeof(aubColorMap_[0]);
			static GLubyte aubColorRandom[3];
			const unsigned int uiIndex = ( cKey - '0' ) * 3;
			GLubyte *pubColor;
			if( uiIndex >= uiMapCount_ )
			{
				for( unsigned int i=0; i<3; ++i )
					aubColorRandom[i] = rand() % 256;
				pubColor = aubColorRandom;
			}
			else
				pubColor = &aubColorMap_[uiIndex ];
			if( m_oLstSelection.GetSize() )
			{
				m_oLstSelection.MoveToFront();
				while( m_oLstSelection.Next() )
				{
					m_oLstSelection.Get()->SetColorRGB( pubColor );
				}
			}
			else if( m_poItemPickCurr )
			{
				m_poItemPickCurr->SetColorRGB( pubColor );
			}
		}

		// Wert-Operator.
		else if( cKey == 'v' || cKey == 'V' )
		{
			COp *poOpValue = m_poOrgOp->CreateShort( "Value" );
			poOpValue->SetNameInstance( "" );
			m_poOrgOp->Insert( poOpValue );

			if( m_poItemPickMoveLinkIn0 )
			{
				m_afPickDefPos[0] = m_afPickMoveLink[0] - VIS_OP_ITEM_STD_X * 0.9f;
				m_afPickDefPos[1] = m_afPickMoveLink[1] - VIS_OP_ITEM_ELEM_Y;
			}
			else
			{
				m_afPickDefPos[0] -= VIS_OP_ITEM_STD_X * 0.5f;
				m_afPickDefPos[1] -= VIS_OP_ITEM_ELEM_Y;
			}
			CItem *poItem = new CItem( 0, 0, poOpValue, VIS_OP_ITEM_TYPE_STD,
				m_afPickDefPos[0], m_afPickDefPos[1] );
			InsertItem( poItem );
			//PickBegin( VIS_OP_PICK_MODE_ACTION0, reinterpret_cast<unsigned int>( poItem ),
			//	m_afPickDefPos[0], m_afPickDefPos[1] ); // ???
		}
		else if( cKey == 'u' || cKey == 'U' )
		{
			if( m_oLstSelection.GetSize() )
			{
				m_oLstSelection.MoveToFront();
				while( m_oLstSelection.Next() )
				{
					m_oLstSelection.Get()->GetOp()->Reset();
					m_oLstSelection.Get()->GetOp()->Update();
				}
			}
			else if( m_poItemPickCurr )
			{
				m_poItemPickCurr->GetOp()->Reset();
				m_poItemPickCurr->GetOp()->Update();
			}
			else
			{
				m_poOrgOp->ResetAll();
				m_poOrgOp->UpdateAll();
			}
		}
	}
}

void CVisOp::ShowCursor( bool bShow )
{
	m_bShowCursor = bShow;
}

void CVisOp::ToggleLog()
{
	m_bShowLog = !m_bShowLog;
}

void CVisOp::ToggleOscilloscope()
{
	m_bShowOscilloscope = !m_bShowOscilloscope;
	Update();
}

void CVisOp::RenderBGOverlay() const
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glBegin( GL_QUADS );
	glColor4ub( m_aubColorBG_[0], m_aubColorBG_[1], m_aubColorBG_[2], VISOP_STD_TRANSPARENCY0 );
	glVertex2f( -1.0f, -1.0f );
	glVertex2f( +1.0f, -1.0f );
	glVertex2f( +1.0f, +1.0f );
	glVertex2f( -1.0f, +1.0f );
	glEnd();

	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}
