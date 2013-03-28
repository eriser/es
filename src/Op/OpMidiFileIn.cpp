// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpMidiFileIn.cpp

#include "OpMidiFileIn.h"
#ifdef OP_INC_OP_MIDI_FILE_IN

////////////////////////////////////////////////////////////////////////////////
// COpMidiFileIn
////////////////////////////////////////////////////////////////////////////////

#define OP_MFI_IN_BPM			0
#define OP_MFI_IN_CLEAR			1
#define OP_MFI_OUT_FILE			0
#define OP_MFI_OUT_TRACKS		1
#define OP_MFI_OUT_DIVISION		2
#define OP_MFI_OUT_CLOCKMAX		3
#define OP_MFI_OUT_BPM			4
#define OP_MFI_OUT_CLOCK		5
#define OP_MFI_OUT_TRIGG		6
#define OP_MFI_ROUTINE_DEV		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_MFI_IN_BPM_INFO		"BPM"
#define OP_MFI_IN_CLEAR_INFO		"restart"
#define OP_MFI_OUT_FILE_INFO		"output"
#define OP_MFI_OUT_TRACKS_INFO		"number of tracks"
#define OP_MFI_OUT_DIVISION_INFO	"division"
#define OP_MFI_OUT_CLOCKMAX_INFO	"number of clocks of the longest track"
#define OP_MFI_OUT_BPM_INFO		"BPM"
#define OP_MFI_OUT_CLOCK_INFO		"current clock"
#define OP_MFI_OUT_TRIGG_INFO		"trigger"
#define OP_MFI_ROUTINE_DEV_INFO		"load file"
#define OP_MFI_CLASS_INFO \
\
"MIDI Input File\n" \
"- The instance name is interpreted as file name.\n" \
"- The routine reloads the file.\n" \
"- The division output value tells the PPQN (pulses per quarter) number\n" \
"  of clocks per quarter/beat.\n" \
"- The current clock value will always be reset to 0.0 when the longest\n" \
"  track ends or if the restart input value is greater than 0.0.\n" \
"  Then the trigger output value is 1.0, otherwise 0.0, also at the\n" \
"  beginning.\n" \
"- The BPM output tells the tempo of the loaded file. When not available,\n" \
"  it is 0.0.\n" \
"- If the BPM inlet is connected, it has first priority. Otherwise, if the\n" \
"  tempo is correctly stored in the file, that will be used. For corrupt\n" \
"  values, 120.0 BPM will be assumed by default."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_MFI_IN_BPM_INFO		"BPM"
#define OP_MFI_IN_CLEAR_INFO		"Neustart"
#define OP_MFI_OUT_FILE_INFO		"Ausgang"
#define OP_MFI_OUT_TRACKS_INFO		"Anzahl Tracks"
#define OP_MFI_OUT_DIVISION_INFO	"Division"
#define OP_MFI_OUT_CLOCKMAX_INFO	"Anzahl Clocks des längsten Tracks"
#define OP_MFI_OUT_BPM_INFO		"BPM"
#define OP_MFI_OUT_CLOCK_INFO		"Aktuelle Anzahl Clocks"
#define OP_MFI_OUT_TRIGG_INFO		"Auslöser"
#define OP_MFI_ROUTINE_DEV_INFO		"Datei laden"
#define OP_MFI_CLASS_INFO \
\
"MIDI-Eingabe-Datei\n" \
"- Der Titel fungiert als Dateiname.\n" \
"- Über die Routine kann die Datei neu geladen werden.\n" \
"- Der Division-Wert gibt in PPQN (Pulses Per Quarter) Anzahl Clocks pro\n" \
"  Viertelnote/Beat an.\n" \
"- Die aktuelle Anzahl Clocks wird immer dann auf 0.0 zurückgesetzt,\n" \
"  wenn der längste Track endet oder wenn der Neustart-Eingangswert\n" \
"  grösser als 0.0 ist. Dann steht der Auslöser-Ausgang auf 1.0, sonst\n" \
"  immer auf 0.0, auch ganz zu Beginn.\n" \
"- Der BPM-Ausgang gibt das Tempo der geladenen Datei an. Falls nicht vorhanden,\n" \
"  steht der Ausgang auf 0.0.\n" \
"- Ist das Tempo in der Datei gespeichert, so wird diese automatisch in dieser\n" \
"  Geschwindigkeit abgespielt. Ist jedoch der optionale BPM-Eingang verbunden,\n" \
"  so wird dieser bevorzugt. Bei ungültiger Konfiguration wird 120.0 BPM\n" \
"  angenommen."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpMidiFileIn::COpMidiFileIn()
: COp( 2, 7 )
{
	Init();
	OP_SET_NAME_INSTANCE( "mFin" );
	m_poIn[OP_MFI_IN_BPM].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_MFI_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_MFI_IN_BPM, "bm", OP_MFI_IN_BPM_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_MFI_IN_CLEAR, "cl", OP_MFI_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MFI_OUT_FILE, "f", OP_MFI_OUT_FILE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MFI_OUT_TRACKS, "t", OP_MFI_OUT_TRACKS_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MFI_OUT_DIVISION, "d", OP_MFI_OUT_DIVISION_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MFI_OUT_CLOCKMAX, "cm", OP_MFI_OUT_CLOCKMAX_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MFI_OUT_BPM, "bpm", OP_MFI_OUT_BPM_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MFI_OUT_CLOCK, "c", OP_MFI_OUT_CLOCK_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_MFI_OUT_TRIGG, "tr", OP_MFI_OUT_TRIGG_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_MFI_ROUTINE_DEV, "d" OP_STR_ROUTINE, OP_MFI_ROUTINE_DEV_INFO );
	//Update();
	Validate();
}

COpMidiFileIn::~COpMidiFileIn() 
{}

COpMidiFileIn & COpMidiFileIn::operator = ( const COpMidiFileIn &roO )
{
	COp::operator = ( roO );
	m_oFile = roO.m_oFile;
	//Update();
	return *this;
}

void COpMidiFileIn::Init()
{
	const unsigned int uiCharIntMax = sizeof(m_auiFileName) / sizeof(m_auiFileName[0]);
	OP_SET_COUNT_INTERNAL_UINT( 1 + uiCharIntMax );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFileNameLen );
	for( unsigned int i=0; i<uiCharIntMax; ++i )
		OP_SET_PTR_INTERNAL_UINT( 1 + i, &m_auiFileName[i] );
	COp::Init();
}

void COpMidiFileIn::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	bool bTriggRestart = false;
	if( m_poIn[OP_MFI_IN_CLEAR].IsValid()
	 && m_poIn[OP_MFI_IN_CLEAR].Proc() > 0.0 )
	{
		bTriggRestart = true;
		m_oFile.Reset();
	}

	if( m_poIn[OP_MFI_IN_BPM].IsValid() )
	{
		const double dBMP = fabs( m_poIn[OP_MFI_IN_BPM].Proc() );
		if( dBMP != m_oFile.m_dBPM )
		{
			m_oFile.m_dBPM = dBMP;
			m_oFile.UpdateMetrics();
		}
	}
	
	const double dClock = double( m_oFile.m_uliClock );
	if( bTriggRestart || dClock >= m_pdOut[OP_MFI_OUT_CLOCKMAX] )
	{
		m_oFile.m_uliClock = 0;
		m_pdOut[OP_MFI_OUT_TRIGG] = 1.0;
		m_pdOut[OP_MFI_OUT_CLOCK] = 0.0;
	}
	else
	{
		m_pdOut[OP_MFI_OUT_TRIGG] = 0.0;
		m_pdOut[OP_MFI_OUT_CLOCK] = dClock;
	}
	m_oFile.Proc();
}

void COpMidiFileIn::Update()
{
	COp::Update();

	const char *pcFileName = 0;

#ifdef OP_USE_RUNTIME_INFO
	pcFileName = m_oStrNameInstance.GetData();
#else
	// Den Dateinamen über int-Internals laden.
	static char acTmp_[( sizeof(m_auiFileName) / sizeof(m_auiFileName[0]) )+1];
	
	unsigned int *puiFileName = &m_auiFileName[0];
	unsigned int c = 0;
	while( c < m_uiFileNameLen )
	{
		const unsigned int uiChars = *puiFileName++;
		for( unsigned int j=0; ( j < sizeof(unsigned int) ) && ( c < m_uiFileNameLen ); ++j )
		{
			const unsigned int uiShift = j << 3;
			acTmp_[c] = char( ( uiChars & ( 0xFF << uiShift ) ) >> uiShift );
			++c;
		}
	}
	acTmp_[c] = 0;
	pcFileName = &acTmp_[0];
#endif // OP_USE_RUNTIME_INFO

	m_oFile.m_uiSampleFreq = OP_SAMPLE_FREQ;
	const bool bSuccess = m_oFile.Init( pcFileName );
	
	m_pdOut[OP_MFI_OUT_TRACKS] = double( m_oFile.GetTrackCount() );
	m_pdOut[OP_MFI_OUT_DIVISION] = double( m_oFile.GetDivision() );
	m_pdOut[OP_MFI_OUT_CLOCKMAX] = double( m_oFile.GetClockMax() );
	m_pdOut[OP_MFI_OUT_BPM] = double( m_oFile.GetFileBPM() );
	
#ifdef OP_USE_RUNTIME_INFO
	// Den Dateinamen über int-Internals speichern.
	const unsigned int uiCharCountMax = sizeof(m_auiFileName) / sizeof(m_auiFileName[0]);
	m_uiFileNameLen = MIN( m_oStrNameInstance.GetSize(), uiCharCountMax );
	unsigned int *puiFileName = &m_auiFileName[0];
	unsigned int c = 0;
	while( c < m_uiFileNameLen )
	{
		unsigned int uiChars = 0;
		for( unsigned int j=0; ( j < sizeof(unsigned int) ) && ( c < m_uiFileNameLen ); ++j )
		{
			uiChars |= (unsigned int)( m_oStrNameInstance[c] ) << ( j * 8 );
			++c;
		}
		*puiFileName++ = uiChars;
	}
	SetNameOutput( OP_MFI_OUT_FILE, bSuccess ? "f" : "?" );
#endif // OP_USE_RUNTIME_INFO
}

void COpMidiFileIn::Reset()
{
	m_oFile.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpMidiFileIn::Routine( unsigned int uiIndex )
{
	Update();
	Validate();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpMidiFileIn, OP_MFI_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpMidiFileIn )
OP_GENERIC_METHODS_DEF( COpMidiFileIn )

#endif // OP_INC_OP_MIDI_FILE_IN
