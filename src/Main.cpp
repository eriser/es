// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Main.cpp

#include "Main.h"

#include "Ctrl.h"
#include "AS/AS.h"
#include "Str/Str.h"
#include "Wnd/Wnd.h"
#include "File/File.h"
#include "File/FileStreamWav.h"
#include "Sys/Sys.h"

#if defined( __WINDOWS__ )
typedef CWndMS TWnd;
#elif defined( __LINUX__ )
typedef CWndX11 TWnd;
#endif // platform

//#define VERSION_RUNTIME_MODE_CODE // Nützlich für Sound-Test. Funktioniert nur in Runtime-Version (VERSION_RUNTIME).
#define USE_SOUND defined( VERSION_RUNTIME_MODE_REALTIME ) || !defined( VERSION_RUNTIME_MODE_RENDER_TO_FILE )

#ifdef VERSION_RUNTIME_MODE_CODE
#include "Op/OpNop.h"
#include "Op/OpOscSin.h"
#include "Op/OpValue.h"

CList<COp *> g_oLstOp;
COpNop g_oOpRootL;
COpNop g_oOpRootR;
#endif // VERSION_RUNTIME_MODE_CODE


#if defined( USE_SOUND )
static CASOut l_oASOut_;

void SndProc()
{
	const unsigned int uiSamples = l_oASOut_.Prepare();
	if( uiSamples > 441 )
	{
		CAS::TValue *ptBuffer = l_oASOut_.GetBuffer();
		const CAS::TValue *ptBufferEnd = ptBuffer + ( uiSamples << 1 );
		do
		{
			COp::Prepare_();

			double dR, dL;
#ifdef VERSION_RUNTIME_MODE_CODE
			g_oOpRootL.Proc();
			g_oOpRootR.Proc();
			dR = g_oOpRootL.Out( 0 );
			dL = g_oOpRootR.Out( 0 );
#else
			COrgOp::Proc_( &dL, &dR );
#endif // VERSION_RUNTIME_MODE_CODE

			*ptBuffer++ = CAS::TValue( dL * AUDIO_VALUE_MAX );
			*ptBuffer++ = CAS::TValue( dR * AUDIO_VALUE_MAX );
		}
		while( ptBuffer < ptBufferEnd );
		l_oASOut_.Write( uiSamples );
	}
}
#endif // USE_SOUND


void Main()
{
#ifndef _DEBUG
	static const char acAppN_[] = APP_NAME;
	static const char acAppA_[] = APP_AUTHOR;
	static const char acAppD_[] = APP_DATE;
#endif // _DEBUG

	CSys::Init_();
        CSys::SetThreadPriority_( CSys::THREAD_PRIO_HIGH );
	
#if defined( USE_SOUND )
	l_oASOut_.SetFormat( 2, AUDIO_SAMPLE_FREQ, 16, AUDIO_BUFFER_SIZE );
	if( !( l_oASOut_.Init() && l_oASOut_.Start() ) )
		return;
#endif // USE_SOUND

#ifdef VERSION_RUNTIME_MODE_CODE
	// run.def.txt muss manuell angepasst werden.
	// Diese Methode ist zu Test-Zwecken gedacht!

	COp *poFreq0 = new COpValue();
	poFreq0->Out( 0 ) = +440.0;
	g_oLstOp.Append( poFreq0 );

	COp* poOsc0 = new COpOscSin();
	poOsc0->In( 0 ) = COp::CLink( poFreq0, 0 );
	poOsc0->Validate();
	g_oLstOp.Append( poOsc0 );

	g_oOpRootL.In( 0 ) = COp::CLink( poOsc0, 0 );
	g_oOpRootR.In( 0 ) = COp::CLink( poOsc0, 0 );
	g_oOpRootL.Validate();
	g_oOpRootR.Validate();

	g_oLstOp.MoveToFront();
	while( g_oLstOp.Next() )
	{
		COp *poOp = g_oLstOp.Get();
		poOp->Update();
		poOp->Validate();
		if( !poOp->IsValid() )
			return;
	}

	do
	{
		SndProc();
	}
	while( !CSys::GetCharNonBlock_( 27 ) );

	g_oLstOp.MoveToFront();
	while( g_oLstOp.Next() )
	{
		delete g_oLstOp.Get();
	}
#else
	static COrgOp oOrgOp_;

#ifdef VERSION_RUNTIME
#if defined( VERSION_RUNTIME_MODE_RENDER_TO_FILE )
	CFile oFile;
	double dR, dL;

	/*
	// todo: CommandLine-Parser.
	const char * pcCmdLine = GetCommandLine();
	unsigned int uiLen = lstrlen(pcCmdLine);
	bool bIsString = false;
	do
	{
		if( *pcCmdLine == '"' )
			bIsString = !bIsString;
		if( !bIsString && *pcCmdLine == ' ' && uiLen != 1 )
			break;
		++pcCmdLine;
	}
	while( --uiLen );
	if( !uiLen )
		return;
	const double dSeconds = atof( pcCmdLine );
	*/

	CFileStreamWav oFileWav;
	oFileWav.m_uiSampleFreq = AUDIO_SAMPLE_FREQ;
	oFileWav.m_uiChannels = 2;
	oFileWav.Open( RUNTIME_FILE_NAME_RENDER, FILE_FLAG_WRITE | FILE_FLAG_TRUNC );
#endif // VERSION_RUNTIME_MODE_RENDER_TO_FILE

	CStr oStrPatch(
#include RUNTIME_FILE_NAME_PATCH  // ... ptch.exe
	);
	oOrgOp_.LoadAll( oStrPatch );

	unsigned int i = OP_SAMPLE_FREQ;
	double d;
	while( --i )
	{
		COp::Prepare_();
		COrgOp::Proc_( &d, &d );
	}

	oOrgOp_.ResetAll();
	oOrgOp_.UpdateAll();
	//oOrgOp_.ValidateAll();

#ifdef VERSION_RUNTIME_MODE_REALTIME
	l_oASOut_.Start();
	do
	{
		SndProc();
	}
	while( !CSys::GetCharNonBlock_( 27 ) );

#endif // VERSION_RUNTIME_MODE_REALTIME
#ifdef VERSION_RUNTIME_MODE_RENDER_TO_FILE
	do
	{
		COp::Prepare_();
		COrgOp::Proc_( &dL, &dR );
		// <mod date="2010-12-10">
		const signed short int assiLR[] = { 
			(signed short int)( dL * AUDIO_VALUE_MAX ),
			(signed short int)( dR * AUDIO_VALUE_MAX ) };
		oFileWav.WriteValues( assiLR, 2 );
		//const signed short int ssiL = (signed short int)( dL * AUDIO_VALUE_MAX );
		//const signed short int ssiR = (signed short int)( dR * AUDIO_VALUE_MAX );
		//oFileWav.Write( (void *)&ssiL, sizeof(ssiL) );
		//oFileWav.Write( (void *)&ssiR, sizeof(ssiR) );
		// </mod>
	}
	while( !CSys::GetCharNonBlock_( 27 ) );

	oFile.Close();
#endif // VERSION_RUNTIME_MODE_RENDER_TO_FILE
#else // VERSION_RUNTIME
	static TWnd oWnd;
	static CCtrl oCtrl( &oWnd, &oOrgOp_, &l_oASOut_ );

	oWnd.m_poHandler	= &oCtrl;
	oWnd.m_iPosX		= 150;
	oWnd.m_iPosY		= 150;
	oWnd.m_iWindowWidth	= 700;
	oWnd.m_iWindowHeight	= 500;
	oWnd.m_bIsFullScreen	= false;
	oWnd.m_pcTitle		= APP_NAME;

	if( !oWnd.Run() )
		return;

#endif // VERSION_RUNTIME
#endif // VERSION_RUNTIME_MODE_CODE
}
