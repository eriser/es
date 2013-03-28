// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFileWavOut.cpp

#include "OpFileWavOut.h"
#ifdef OP_INC_OP_FILE_WAV_OUT

////////////////////////////////////////////////////////////////////////////////
// COpFileWavOut
////////////////////////////////////////////////////////////////////////////////

#define OP_FWO_IN_SRCL			0
#define OP_FWO_IN_SRCR			1
#define OP_FWO_IN_TRIGGER		2
#define OP_FWO_OUT_DSTL			0
#define OP_FWO_OUT_DSTR			1
#define OP_FWO_ROUTINE_FOUT		0

#define OP_FWO_VALUE_MAX		( 32767.0 )

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FWO_IN_SRCL_INFO		"left input"
#define OP_FWO_IN_SRCR_INFO		"right input" 
#define OP_FWO_IN_TRIGGER_INFO		"trigger" 
#define OP_FWO_OUT_DSTL_INFO		"left output"
#define OP_FWO_OUT_DSTR_INFO		"right output"
#define OP_FWO_ROUTINE_FOUT_INFO	"open/close file"
#define OP_FWO_CLASS_INFO \
\
"Wave Output File\n" \
"- The format is RIFF WAVE (16-bit, stereo, PCM).\n" \
"- The instance name is interpreted as file name.\n" \
"- Input values should lie within [-1.0,1.0], and will not be clamped.\n" \
"- When the trigger value is greater than 0.0, a new file will be opened\n" \
"  and used immediately.\n" \
"- At the maximum 100 files per second."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FWO_IN_SRCL_INFO		"linker Eingang"
#define OP_FWO_IN_SRCR_INFO		"rechter Eingang" 
#define OP_FWO_IN_TRIGGER_INFO		"Auslöser" 
#define OP_FWO_OUT_DSTL_INFO		"linker Ausgang"
#define OP_FWO_OUT_DSTR_INFO		"rechter Ausgang"
#define OP_FWO_ROUTINE_FOUT_INFO	"Datei öffnen/schliessen"
#define OP_FWO_CLASS_INFO \
\
"Wave-Ausgabe-Datei\n" \
"- Das Format ist RIFF WAVE (16-bit, stereo, PCM).\n" \
"- Der Dateiname ist aus dem Instanznamen und einem genauen Zeitformat\n" \
"  zusammengesetzt.\n" \
"- Die Eingabewerte sollten in [-1.0,1.0] liegen und werden nicht gekappt.\n" \
"- Ist der Auslöser angeschlossen, wird immer dann, wenn der Wert am Eingang\n" \
"  grösser 0.0 ist eine neue Datei geöffnet und sofort beschrieben.\n" \
"- Pro Sekunde können max. 100 Dateien geschrieben werden."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFileWavOut::COpFileWavOut()
: COp( 3, 2 )
, m_uiIsOpen( 0 )
{
	OP_SET_NAME_INSTANCE( "wavOut" );
	m_poIn[OP_FWO_IN_SRCL].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_FWO_IN_SRCR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_FWO_IN_TRIGGER].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_FWO_IN_SRCL, "l", OP_FWO_IN_SRCL_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FWO_IN_SRCR, "r", OP_FWO_IN_SRCR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_FWO_IN_TRIGGER, "tr", OP_FWO_IN_TRIGGER_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FWO_OUT_DSTL, "l", OP_FWO_OUT_DSTL_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FWO_OUT_DSTR, "r", OP_FWO_OUT_DSTR_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_FWO_ROUTINE_FOUT, "o" OP_STR_ROUTINE, OP_FWO_ROUTINE_FOUT_INFO );
	Update();
	Validate();
}

COpFileWavOut::~COpFileWavOut() 
{
	// new
	if( m_uiIsOpen )
	{
		m_oFileOut.Close();
	}
}

void COpFileWavOut::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_FWO_IN_TRIGGER].IsValid() )
		if( m_poIn[OP_FWO_IN_TRIGGER].Proc() > 0.0 )
		{
			if( m_uiIsOpen )
				m_oFileOut.Close();
			m_uiIsOpen = NewFile();
			Update();
		}
	
	double dInL = 0.0;
	if( m_poIn[OP_FWO_IN_SRCL].IsValid() )
		dInL = m_poIn[OP_FWO_IN_SRCL].Proc();
	double dInR = 0.0;
	if( m_poIn[OP_FWO_IN_SRCR].IsValid() )
		dInR = m_poIn[OP_FWO_IN_SRCR].Proc();
	if( m_uiIsOpen )
	{
		// <mod date="2010-12-10">
		const signed short int assiSample[2] = {
			(signed short int)( dInL * OP_FWO_VALUE_MAX ),
			(signed short int)( dInR * OP_FWO_VALUE_MAX ) };
		m_oFileOut.WriteValues( assiSample, 2 );
		//m_oFileOut.Write( (void *)assiSample, 2 * sizeof(signed short int) );
		// </mod>
	}
	m_pdOut[OP_FWO_OUT_DSTL] = dInL;
	m_pdOut[OP_FWO_OUT_DSTR] = dInR;
}

void COpFileWavOut::Update()
{
	COp::Update();
#ifdef OP_USE_RUNTIME_INFO
	unsigned char aucName[] = "l ";
	if( m_uiIsOpen )
		aucName[1] = '>';
	SetNameOutput( OP_FWO_OUT_DSTL, (const char *)aucName );
	aucName[0] = 'r';
	SetNameOutput( OP_FWO_OUT_DSTR, (const char *)aucName );
#endif // OP_USE_RUNTIME_INF
}

#ifdef OP_USE_ROUTINES
void COpFileWavOut::Routine( unsigned int uiIndex )
{
	COp::Routine( uiIndex );
	
	switch( uiIndex )
	{
	case OP_FWO_ROUTINE_FOUT:
		if( m_uiIsOpen )
		{
			m_oFileOut.Close();
			m_uiIsOpen = false;
		}
		else
			m_uiIsOpen = NewFile();
	break;
	}
	Update();
	Validate();
}
#endif

bool COpFileWavOut::NewFile()
{
#ifdef OP_USE_RUNTIME_INFO
	m_oTime.Update();
	m_oFileOut.m_uiChannels = 2;
	m_oFileOut.m_uiSampleFreq = OP_SAMPLE_FREQ;
	CStr oStrFileName = m_oStrNameInstance;
	oStrFileName += "_";
	oStrFileName += m_oTime.GetString();
	oStrFileName += "_00.wav";
	char *pZ = (char *)oStrFileName + oStrFileName.GetSize() - 6;
	char *pE = pZ + 1;
	unsigned int uiNum = 0;
	while( CFile::Exist_( oStrFileName ) )
	{
		if( ++uiNum >= 100 )
			return false;
		if( *pE >= '9' )
		{
			*pE = '0';
			*pZ += 1;
		}
		else
			*pE += 1;
	}
	return m_oFileOut.Open( oStrFileName, CFile::FLAG_WRITE | CFile::FLAG_TRUNC );
#else
	return false;
#endif // OP_USE_RUNTIME_INFO
}

OP_SET_NAME_AND_INFO_CLASS( COpFileWavOut, OP_FWO_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFileWavOut )
OP_GENERIC_METHODS_DEF( COpFileWavOut )

#endif // OP_INC_OP_FILE_WAV_OUT
