// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Ctrl.h

#ifndef _CTRL_H_
#define _CTRL_H_

#include "DEF.h"
#include "Wnd/Wnd.h"
#include "Sys/Sys.h"
#include "VisOp/VisOp.h"
#include "GLTxt/GLTxtPrinter.h"
#include "GLGrab/GLGrab.h"
#include "GLTool/GLTool.h"
#include "OrgOp/OrgOp.h"
#include "Time/Time.h"
#include "AS/AS.h"

#include <GL/gl.h>
#include <GL/glu.h>

#define CTRL_FILE_NAME_STD0		"patch0.txt"
#define CTRL_FILE_NAME_STD1		"patch1.txt"
#define CTRL_FILE_NAME_STD2		"patch2.txt"
#define CTRL_FILE_NAME_STD3		"patch3.txt"
#define CTRL_TRANS_STP			( 500.0f )
#define CTRL_MOVE_TIME_ROLLOFF		( 0.25f )
#define CTRL_MOVE_SPEED			( 10.0f )


// Kontrollinstanz zwischen Betriebsystem/Windowsystem und Applikation.
// -> Keine systemabhängige Funktionen, alles extern gewrappt!
////////////////////////////////////////////////////////////////////////////////
class CCtrl : public CWnd::CHandler
{
public:
	CCtrl( CWnd *poWnd, COrgOp *poOrgOp, CASOut *poASOut );
	virtual ~CCtrl();

public:
	virtual bool OnInit();
	virtual void OnUpdate();
	virtual	void OnClear();
	virtual bool OnDisplay();
	virtual void OnKeyDown( unsigned int uiKey );
	virtual void OnKeyUp( unsigned int uiKey );
	virtual void OnChar( char cKey );
	virtual void OnMouse( unsigned int uiButton, int iState );
	virtual	void OnSize();
	virtual void OnDrop();
	virtual void OnCursor( bool bInside );

protected:
	GLuint PickingProcessHits( GLint iHits );
	void PickingStart( int iX, int iY );
	GLint PickingStop();
	bool Pick( int iX, int iY  );
	static void HandleDialog_( void *pvArg0, unsigned int uiArg1 );

protected:
	static const GLuint m_uiSelectBufferSize_ = 512;
	GLuint m_auiSelectBuffer[ m_uiSelectBufferSize_ ];

	GLdouble m_dZNearOrtho, m_dZFarOrtho;
	//GLdouble m_dZNearPerspective, m_dZFarPerspective;
	//GLdouble m_dFieldOfViewY;
	GLdouble m_dRatioWH;
	GLdouble m_dOrthoW_2, m_dOrthoH_2;
	GLdouble m_dWndSizeRef; // rem: in Anzahl Pixel, die in OpenGL-Koordinaten 1.0 entsprechen.

	bool m_bIsDialog;
	bool m_bIsPicking;
	bool m_bIsContext;
	GLuint m_uiPickID;
	unsigned int m_uiCursorLast;

	float m_fTransRef[2];
	float m_fTransStp[2];
	unsigned int m_uiModeTrans[2];
	double m_dTransLast[2];

	CTime m_oTime;
	CVisOp m_oVisOp;
	CGLGrab m_oGLGrab;
	COrgOp *m_poOrgOp;
	CASOut *m_poASOut;
};

#endif // _CTRL_H_
