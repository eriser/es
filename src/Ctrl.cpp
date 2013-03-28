// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Ctrl.cpp

#include "Ctrl.h"
#include "Main.h"

////////////////////////////////////////////////////////////////////////////////
// CCtrl
////////////////////////////////////////////////////////////////////////////////

CCtrl::CCtrl( CWnd *poWnd, COrgOp *poOrgOp, CASOut *poASOut )
: m_dWndSizeRef( 450.0 )
, m_oVisOp( poOrgOp )
, m_poOrgOp( poOrgOp )
, m_poASOut( poASOut )
{
	m_poWnd = poWnd;

	m_dZNearOrtho			= -1.00;
	m_dZFarOrtho			= +1.00;

	//m_dZNearPerspective		= 0.01;
	//m_dZFarPerspective		= 100.0;
	//m_dFieldOfViewY		= 45.0;

	//CRandom::m_uliSeed_		= 0;
	//srand( RANDOM_SEED0 );
	//srand( GetTickCount( ) );

	m_oGLGrab.SetFileNamePrefix( APP_NAME );
}

CCtrl::~CCtrl()
{
	OnClear();
}

bool CCtrl::OnInit()
{
	static bool m_bFirst_ = true;

	m_bIsDialog = false;
	m_bIsPicking = false;
	m_bIsContext = false;
	m_uiPickID = 0;

	m_uiModeTrans[0] = 0;
	m_uiModeTrans[1] = 0;
	m_fTransRef[0] = 0.0f;
	m_fTransRef[1] = 0.0f;
	m_fTransStp[0] = 0.0f;
	m_fTransStp[1] = 0.0f;
	m_dTransLast[0] = 0.0;
	m_dTransLast[1] = 0.0;

	m_oTime.Reset();

	//if( !CRandom::m_uliSeed_  )
	//	CRandom::Init_();
	//else
	//	CRandom::Reset_();

	//if( m_bIsGLEWUninit )
	//{
	//	GLenum err = glewInit();
	//	if (GLEW_OK != err)
	//		CSys::Message_( (const char*)glewGetErrorString(err) );
	//	m_bIsGLEWUninit = false;
	//}

	m_oVisOp.Setup();
	m_oVisOp.ShowCursor( true );

	if( m_bFirst_ )
	{
#ifdef USE_EDIT_PATCH
		if( !m_oVisOp.LoadPatch(
#include		EDIT_FILE_NAME_PATCH  // ... ptch.exe
		) )
			return false;
#else
		if( !m_oVisOp.FileNew( VIS_OP_FILE_NEW_TEMPLATE_STD0 ) )
			return false;
#endif // USE_EDIT_PATCH
		m_bFirst_ = false;
	}
	else
	{
		m_poASOut->Start();
	}

	return true;
}

void CCtrl::OnUpdate()
{
	m_oVisOp.UpdateFont( m_poWnd->m_poFont );
	m_oVisOp.Update();
}

void CCtrl::OnClear()
{
	//m_oASOut.Stop();
}

bool CCtrl::OnDisplay()
{
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

	const int iX = m_poWnd->m_iMousePosX;
	const int iY = m_poWnd->m_iMousePosY;

	const bool bLeft = m_poWnd->m_abKeyDown[WND_KEY_LEFT];
	const bool bRight = m_poWnd->m_abKeyDown[WND_KEY_RIGHT];
	const bool bDown = m_poWnd->m_abKeyDown[WND_KEY_DOWN];
	const bool bUp = m_poWnd->m_abKeyDown[WND_KEY_UP];

	static double dTLast_ = 0.0;
	const double dT = m_oTime.GetSeconds();
	const double dTDiff = dT - dTLast_;
	dTLast_ = dT;

	if( bLeft || bRight )
	{
		if( bLeft )
			m_fTransStp[0] = +CTRL_TRANS_STP;
		else if( bRight )
			m_fTransStp[0] = -CTRL_TRANS_STP;
		m_uiModeTrans[0] = 2;
		m_dTransLast[0] = dT;
	}
	if( bUp || bDown )
	{
		if( bDown )
			m_fTransStp[1] = -CTRL_TRANS_STP;
		else if( bUp )
			m_fTransStp[1] = +CTRL_TRANS_STP;
		m_uiModeTrans[1] = 2;
		m_dTransLast[1] = dT;
	}

	const float afCoord[2] = { float(iX), float(iY) };
	for( unsigned int i=0; i<2; ++i )
	{
		switch( m_uiModeTrans[i] )
		{
		case 1:
		{
			m_fTransStp[i] = afCoord[i] - m_fTransRef[i];
			if( i )
				m_oVisOp.CoordTranslate( 0.0f, -m_fTransStp[i] );
			else
				m_oVisOp.CoordTranslate( +m_fTransStp[i], 0.0f );
			m_fTransRef[i] = afCoord[i];
		}
		break;
		case 2:
		{
			float fDiff = float( dT - m_dTransLast[i] );
			if( fDiff > CTRL_MOVE_TIME_ROLLOFF )
				m_uiModeTrans[i] = 0;
			else
			{
				fDiff *= 1.0f / CTRL_MOVE_TIME_ROLLOFF;
				fDiff = 1.0f - fDiff;
				fDiff *= float( dTDiff );
				if( i )
					m_oVisOp.CoordTranslate( 0.0f, -m_fTransStp[i] * fDiff );
				else
					m_oVisOp.CoordTranslate( +m_fTransStp[i] * fDiff, 0.0f );
			}
		}
		break;
		}
	}

	if( m_bIsPicking )
	{
		m_oVisOp.PickProc( iX, iY );
	}
	m_oVisOp.Proc( iX, iY );

	glFlush();
	glFinish();

	SndProc();

	return true;
}

void CCtrl::OnKeyDown( unsigned int uiKey )
{
	if( m_bIsDialog )
	{
		switch( uiKey )
		{
		case WND_KEY_ESCAPE:
			m_bIsDialog = false;
			m_oVisOp.DialogCancel();
		break;
		case WND_KEY_RETURN:
		case WND_KEY_SPACE:
			m_bIsDialog = false;
			m_oVisOp.DialogOk();
		break;
		}
		return;
	}

	CStr oStrDialog;
	static const char acLoad[] =
#if defined( APP_LANGUAGE_ENGLISH )
		": load?";
#elif defined( APP_LANGUAGE_GERMAN )
		" laden?";
#endif // APP_LANGUAGE_*
	static const char acSave[] =
#if defined( APP_LANGUAGE_ENGLISH )
		": save?";
#elif defined( APP_LANGUAGE_GERMAN )
		" speichern?";
#endif // APP_LANGUAGE_*

	switch( uiKey )
	{
	case WND_KEY_ESCAPE:
#if defined( APP_LANGUAGE_ENGLISH )
		oStrDialog = "Quit?";
#elif defined( APP_LANGUAGE_GERMAN )
		oStrDialog = "Session beenden?";
#endif // APP_LANGUAGE_*
	break;
	case WND_KEY_F1:
		m_oVisOp.KeyFunctionInfo();
	break;
	case WND_KEY_F2:
		if( m_poWnd->m_abKeyDown[WND_KEY_CONTROL] )
			m_poOrgOp->m_bEnableCache_ = !m_poOrgOp->m_bEnableCache_;
		else
			m_oVisOp.ToggleOscilloscope();
	break;
	case WND_KEY_F3:
		m_oVisOp.ToggleLog();
	break;
	case WND_KEY_F4:
		oStrDialog = "Screenshot?";
	break;
	case WND_KEY_F5:
		oStrDialog = CStr(CTRL_FILE_NAME_STD0) + acSave;
	break;
	case WND_KEY_F6:
		oStrDialog = CStr(CTRL_FILE_NAME_STD1) + acSave;
	break;
	case WND_KEY_F7:
		oStrDialog = CStr(CTRL_FILE_NAME_STD0) + acLoad;
	break;
	case WND_KEY_F8:
		oStrDialog = CStr(CTRL_FILE_NAME_STD1) + acLoad;
	break;
	case WND_KEY_F9:
#if defined( APP_LANGUAGE_ENGLISH )
		oStrDialog = "Load template0?";
#elif defined( APP_LANGUAGE_GERMAN )
		oStrDialog = "Template0 laden?";
#endif // APP_LANGUAGE_*
	break;
	case WND_KEY_F10:
	break;
	case WND_KEY_F11:
	break;
	case WND_KEY_F12:
	break;
	case WND_KEY_Q:
	//	m_poASOut->Stop();
	break;
	case WND_KEY_W:
	//	m_poASOut->Start();
	break;
	};

	if( oStrDialog.GetSize() )
	{
		if( m_oVisOp.Dialog( oStrDialog.GetData(), HandleDialog_, (void *)this, uiKey ) )
		{
			m_bIsDialog = true;
			return;
		}
	}

	switch( uiKey )
	{
	case WND_KEY_ESCAPE:
		if( m_oVisOp.Dialog(
#if defined( APP_LANGUAGE_ENGLISH )
			"Quit?",
#elif defined( APP_LANGUAGE_GERMAN )
			"Session beenden?",
#endif // APP_LANGUAGE_*
			HandleDialog_, (void *)this, uiKey ) )
		{
			m_bIsDialog = true;
		}
	break;
	case WND_KEY_RETURN:
		if( !( m_poWnd->m_bIsMoving || m_poWnd->m_bIsResizing )
		 && m_poWnd->m_abKeyDown[WND_KEY_ALT] )
		{
			m_poASOut->Stop();
			m_poWnd->ToggleScreenMode();
		}
	break;
	case WND_KEY_SPACE:
		if( !m_poWnd->m_abKeyDown[WND_KEY_CONTROL] &&
		    !m_bIsPicking )
		{
 			m_bIsPicking = true;
			m_bIsContext = true;
			m_uiPickID = 0;
			m_oVisOp.PickBegin( VIS_OP_PICK_MODE_CONTEXT0, m_uiPickID,
				m_poWnd->m_iMousePosX, m_poWnd->m_iMousePosY );
		}
	break;
	case WND_KEY_SHIFT:
		m_oVisOp.KeyFunctionA( true );
	break;
	case WND_KEY_CONTROL:
		m_oVisOp.KeyFunctionB( true );
	break;
	}
}

void CCtrl::OnKeyUp( unsigned int uiKey )
{
	switch( uiKey )
	{
	case WND_KEY_SHIFT:
		m_oVisOp.KeyFunctionA( false );
	break;
	case WND_KEY_CONTROL:
		m_oVisOp.KeyFunctionB( false );
	break;
	}
}

void CCtrl::OnChar( char cKey )
{
	m_oVisOp.KeyChar( cKey );
}

void CCtrl::OnMouse( unsigned int uiButton, int iState )
{
	int iX = m_poWnd->m_iMousePosX;
	int iY = m_poWnd->m_iMousePosY;

	if( m_bIsContext )
	{
		m_oVisOp.PickEnd( VIS_OP_PICK_MODE_CONTEXT0 );
		m_uiPickID = 0;
		m_bIsPicking = false;
		m_bIsContext = false;
	}

	switch( uiButton )
	{
	case WND_MOUSE_BUTTON_LEFT:
	{
		switch( iState )
		{
		case WND_MOUSE_STATE_DOWN:
		{
			if( !( m_bIsPicking || m_poWnd->m_bIsMoving || m_poWnd->m_bIsResizing ) )
			{
				if( Pick( iX, iY ) )
				{
					m_oVisOp.PickBegin( VIS_OP_PICK_MODE_MOVE, m_uiPickID, iX, iY );
				}
				else
				{
					// Rechteck-Auswahl beginnen.
					m_oVisOp.PickBegin( VIS_OP_PICK_MODE_DEFPOS, 0, iX, iY );
					m_bIsPicking = true;
				}
				m_poWnd->SetMouseCapture();
			}
		}
		break;
		case WND_MOUSE_STATE_UP:
		{
			m_poWnd->ReleaseMouseCapture();

			if( m_uiPickID )
				m_oVisOp.PickEnd( VIS_OP_PICK_MODE_MOVE );
			if( !m_poWnd->m_bIsMoving ) // Rechteckauswahl: Ende.
				m_oVisOp.PickEnd( VIS_OP_PICK_MODE_DEFPOS );

			m_uiPickID = 0;
			m_bIsPicking = false;
		}
		break;
		case WND_MOUSE_STATE_DBLCLK:
		{
			if( Pick( iX, iY ) )
			{
				m_oVisOp.PickBegin( VIS_OP_PICK_MODE_ACTION0, m_uiPickID, iX, iY );
				m_oVisOp.PickProc( iX, iY );
				m_oVisOp.PickEnd( VIS_OP_PICK_MODE_ACTION0 );

				m_bIsPicking = false;
				m_uiPickID = 0;
			}
		}
		break;
		}
	}
	break;
	case WND_MOUSE_BUTTON_RIGHT:
	{
		switch( iState )
		{
		case WND_MOUSE_STATE_DOWN:
			if( !m_bIsPicking )
			{
				//if( Pick( iX, iY ) == false )
				//{
				//	m_bIsPicking = true; // Auch bei Rechts-Klick ins "Leere".
				//	m_uiPickID = 0;
				//	m_oVisOp.PickBegin( VIS_OP_PICK_MODE_CONTEXT0, m_uiPickID, iX, iY );
				//}
				//else
				if( Pick( iX, iY ) )
				{
					m_oVisOp.PickBegin( VIS_OP_PICK_MODE_TWIDDLE, m_uiPickID, iX, iY );
				}
				m_poWnd->SetMouseCapture();
			}
		break;
		case WND_MOUSE_STATE_UP:
			m_poWnd->ReleaseMouseCapture();

			if( m_uiPickID )
				m_oVisOp.PickEnd( VIS_OP_PICK_MODE_TWIDDLE );
			//else
			//	m_oVisOp.PickEnd( VIS_OP_PICK_MODE_CONTEXT0 );

			m_uiPickID = 0;
			m_bIsPicking = false;
		break;
		case WND_MOUSE_STATE_DBLCLK:
			if( Pick( iX, iY ) )
			{
				m_oVisOp.PickBegin( VIS_OP_PICK_MODE_ACTION1, m_uiPickID, iX, iY );
				m_oVisOp.PickProc( iX, iY );
				m_oVisOp.PickEnd( VIS_OP_PICK_MODE_ACTION1 );

				m_bIsPicking = false;
				m_uiPickID = 0;
			}
		break;
		}
	}
	break;
	case WND_MOUSE_BUTTON_MIDDLE:
	{
		if( !( m_poWnd->m_bIsMoving || m_poWnd->m_bIsResizing ) )
		switch( iState )
		{
		case WND_MOUSE_STATE_DOWN:
			m_uiModeTrans[0] = m_uiModeTrans[1] = 1;
			m_fTransRef[0] = float( m_poWnd->m_iMousePosX );
			m_fTransRef[1] = float( m_poWnd->m_iMousePosY );
			m_poWnd->SetMouseCapture();
		break;
		case WND_MOUSE_STATE_UP:
			m_poWnd->ReleaseMouseCapture();
			m_uiModeTrans[0] = m_uiModeTrans[1] = 2;
			m_dTransLast[0] = m_dTransLast[1] = m_oTime.GetSeconds();
		break;
		}
	}
	break;
	}
}

void CCtrl::OnSize()
{
	GLsizei tWidth  = m_poWnd->GetWidth();
	GLsizei tHeight = m_poWnd->GetHeight();
	if( tHeight == 0 )
		tHeight = 1;

	glViewport( 0, 0, tWidth, tHeight );

	m_dRatioWH =  GLdouble(tWidth) / tHeight;
	m_dOrthoW_2 = GLdouble(tWidth)  / m_dWndSizeRef;
	m_dOrthoH_2 = GLdouble(tHeight) / m_dWndSizeRef;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//gluPerspective( m_dFieldOfViewY, m_dRatioWH, m_dZNearPerspective, m_dZFarPerspective );

	glOrtho( -m_dOrthoW_2, +m_dOrthoW_2, -m_dOrthoH_2, +m_dOrthoH_2, m_dZNearOrtho, m_dZFarOrtho );

	//glOrtho( -m_dRatioWH, +m_dRatioWH, -1.0, +1.0, m_dZNearOrtho, m_dZFarOrtho ); // Vertikal von -1.0 bis +1.0
	//glOrtho( -1, +1, -1, +1, m_dZNearOrtho, m_dZFarOrtho ); // -1.0 bis +1.0

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_ACCUM_BUFFER_BIT );
}

void CCtrl::OnDrop()
{
#ifdef WND_USE_DND
	if( m_poWnd->m_oArrDrop.GetSize() )
	{
		const char *pcFileName = m_poWnd->m_oArrDrop[0].GetData();
		if( m_poWnd->m_abKeyDown[WND_KEY_CONTROL] )
			m_oVisOp.FileLoadMacro( pcFileName );
		else
			m_oVisOp.FileLoad( pcFileName );
	}
	m_poWnd->m_oArrDrop.Clear();
#endif // WND_USE_DND
}

void CCtrl::OnCursor( bool bInside )
{
	m_oVisOp.ShowCursor( bInside );
}

bool CCtrl::Pick( int iX, int iY )
{
	unsigned int uiHits;

	PickingStart( iX, iY );
	OnDisplay();
	uiHits = PickingStop();
	if( uiHits )
	{
		m_uiPickID = PickingProcessHits( uiHits );
		if( m_uiPickID ) // rem: ID == 0 als disable flag!
		{
			m_bIsPicking = true;
			return true;
		}
	}
	return false;
}

GLuint CCtrl::PickingProcessHits( GLint iHits )
{
	GLint i, iNumberOfNames = 0;
	GLuint uiNames, *puiPtr, uiMinZ, *puiNames = 0;
	//stringstream ssCode;
	//ssCode.setf( ios_base::hex ); // rem: Wahrscheinlich unnötig.

	puiPtr = (GLuint *) m_auiSelectBuffer;
	uiMinZ = 0xffffffff;
	for (i = 0; i < iHits; i++)
	{
		uiNames = *puiPtr; // 1. Feld des Hit-Records: Anzahl Namen.
		puiPtr++;
		if (*puiPtr < uiMinZ) // 2/3. Feld des Hit-Records: Min./Max. Tiefe.
		{
			iNumberOfNames = uiNames;
			uiMinZ = *puiPtr;
			puiNames = puiPtr + 2;
		}
		puiPtr += iNumberOfNames + 2;
	}

	/*
	// Setzt einen Code-String zusammen der hierarchisch beschreibt, welches Element am nächsten bei Cursor steht.
	// Hierarchische Selektions-Code in hex.
	if( iNumberOfNames > 0 )
	{
		puiPtr = puiNames;
		for( j = 0; j < iNumberOfNames; j++, puiPtr++ )
		{
			ssCode << hex << *puiPtr << " ";
		}
	}
	*/

	if ( iNumberOfNames > 0 )
	{
		return *(puiNames + iNumberOfNames - 1);
	}
	return 0;
}

void CCtrl::PickingStart( int iX, int iY )
{
	GLint aiViewport[4];
	//GLdouble dRatio;

	glPushAttrib( GL_POLYGON_BIT  );
	glPolygonMode( GL_FRONT, GL_FILL );

	// Selektionspuffer anmelden.
	glSelectBuffer( m_uiSelectBufferSize_, m_auiSelectBuffer );

	// Viewport abfragen.
	glGetIntegerv( GL_VIEWPORT, aiViewport );

	// Selektionsmodus aktivieren.
	glRenderMode( GL_SELECT );

	// Leeren Selektion-Stack generieren.
	glInitNames();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(
		iX,
		aiViewport[3]-iY,
		1,
		1,
		aiViewport
		);

	//dRatio = GLdouble(aiViewport[3]) / aiViewport[2]; // rem: Unkonventionell, eigentlich w/h, hier wegen Pan.
	//dRatio = GLdouble(aiViewport[2]) / aiViewport[3];

	//gluPerspective( m_fFieldOfViewY, m_fRatio, m_dZNearPerspective, m_dZFarPerspective );
	//glOrtho( -1.0, +1.0, -m_dRatio, +m_dRatio, m_dZNearOrtho, m_dZFarOrtho );
	//glOrtho( -m_dRatioWH, +m_dRatioWH, -1.0, +1.0, m_dZNearOrtho, m_dZFarOrtho ); // Vertikal von -1.0 bis +1.0
	glOrtho( -m_dOrthoW_2, +m_dOrthoW_2, -m_dOrthoH_2, +m_dOrthoH_2, m_dZNearOrtho, m_dZFarOrtho );

	glMatrixMode(GL_MODELVIEW);
}

GLint CCtrl::PickingStop()
{
	// Originale Matrix restaurieren.
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glFlush();

	glPopAttrib();

	// Normalen Render-Mode aktivieren.
	return glRenderMode( GL_RENDER );
}

void CCtrl::HandleDialog_( void *pvArg0, unsigned int uiArg1 )
{
	CCtrl *poCtrl = (CCtrl *)pvArg0;
	poCtrl->m_bIsDialog = false;
	if( !uiArg1 )
		return;

	const bool bLoad = ( uiArg1 == WND_KEY_F7 || uiArg1 == WND_KEY_F8 || uiArg1 == WND_KEY_F9 );
	if( bLoad )
		poCtrl->m_poASOut->Stop();

	switch( uiArg1 )
	{
	case WND_KEY_ESCAPE:
		poCtrl->m_poWnd->Terminate();
	break;
	case WND_KEY_F1:
	break;
	case WND_KEY_F2:
	break;
	case WND_KEY_F3:
	break;
	case WND_KEY_F4:
		poCtrl->OnDisplay();
		poCtrl->m_oGLGrab.Grab();
		poCtrl->m_oGLGrab.Save();
		//poCtrl->m_oVisOp.FileNew( VIS_OP_FILE_NEW_TEMPLATE_STD1 );
	break;
	case WND_KEY_F5:
		poCtrl->m_oVisOp.FileSave( CTRL_FILE_NAME_STD0 );
	break;
	case WND_KEY_F6:
		poCtrl->m_oVisOp.FileSave( CTRL_FILE_NAME_STD1 );
	break;
	case WND_KEY_F7:
		poCtrl->m_oVisOp.FileLoad( CTRL_FILE_NAME_STD0 );
		poCtrl->m_oVisOp.Update();
	break;
	case WND_KEY_F8:
		poCtrl->m_oVisOp.FileLoad( CTRL_FILE_NAME_STD1 );
		poCtrl->m_oVisOp.Update();
	break;
	case WND_KEY_F9:
		poCtrl->m_oVisOp.FileNew( VIS_OP_FILE_NEW_TEMPLATE_STD0 );
		poCtrl->m_oVisOp.Update();
	break;
	case WND_KEY_F10:
	break;
	case WND_KEY_F11:
	break;
	case WND_KEY_F12:
	break;
	}

	if( bLoad )
		poCtrl->m_poASOut->Start();
}
