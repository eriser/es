// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpSampler.cpp

#include "OpSampler.h"
#ifdef OP_INC_OP_SAMPLER

////////////////////////////////////////////////////////////////////////////////
// COpSampler
////////////////////////////////////////////////////////////////////////////////

#define OP_SMP_IN_TRIGGERA		0
#define OP_SMP_IN_TRIGGERR		1
#define OP_SMP_IN_NOTE			2
#define OP_SMP_IN_ROOTKEY		3
#define OP_SMP_IN_LOOPTYPE		4
#define OP_SMP_IN_LOOP0			5
#define OP_SMP_IN_LOOP1			6
#define OP_SMP_IN_LOOPCOUNT		7
#define OP_SMP_OUT_FILE			0
#define OP_SMP_OUT_TRIGGERS		1
#define OP_SMP_OUT_TRIGGERL		2
#define OP_SMP_OUT_DURA			3
#define OP_SMP_OUT_DURT			4
#define OP_SMP_OUT_DURFACTOR		5
#define OP_SMP_OUT_ROOTKEY		6
#define OP_SMP_ROUTINE_FIN		0
#define OP_SMP_ROUTINE_MODE		1

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_SMP_IN_TRIGGERA_INFO		"A-trigger"
#define OP_SMP_IN_TRIGGERR_INFO		"R-trigger"
#define OP_SMP_IN_NOTE_INFO		"note"
#define OP_SMP_IN_ROOTKEY_INFO		"root key"
#define OP_SMP_IN_LOOPTYPE_INFO		"loop type"
#define OP_SMP_IN_LOOP0_INFO		"loop start"
#define OP_SMP_IN_LOOP1_INFO		"loop end"
#define OP_SMP_IN_LOOPCOUNT_INFO	"number of loops"
#define OP_SMP_OUT_FILE_INFO		"output"
#define OP_SMP_OUT_TRIGGERS_INFO	"loop start trigger"
#define OP_SMP_OUT_TRIGGERL_INFO	"loop repeat trigger"
#define OP_SMP_OUT_DURA_INFO		"current A-duration"
#define OP_SMP_OUT_DURT_INFO		"current duration total"
#define OP_SMP_OUT_DURFACTOR_INFO	"current duration factor"
#define OP_SMP_OUT_ROOTKEY_INFO		"root key"
#define OP_SMP_ROUTINE_FIN_INFO		"load file"
#define OP_SMP_ROUTINE_MODE_INFO	"interpolation mode"
#define OP_SMP_CLASS_INFO \
\
"Sampler\n" \
"- The format is RIFF WAVE (16-Bit, Mono, PCM). Supported \"smpl\" chunks:\n" \
"  loop start, loop end, loop type, loop count.\n" \
"- When a file has more than one channel, then only the first one will be used.\n" \
"- The routine reloads the file.\n" \
"- The instance name is interpreted as file name.\n" \
"- All input values have higher priority than values stored in the file.\n" \
"- Available interpolation modes: NEAREST_NEIGHBOUR, LINEAR.\n" \
"- Available loop types: 0 = None, 1 = forward, 2 = ping-pong, 3 = backwards.\n" \
"- Loop start and end values are relative to 1.0, which refers to the\n" \
"  overall duration."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_SMP_IN_TRIGGERA_INFO		"A-Auslöser"
#define OP_SMP_IN_TRIGGERR_INFO		"R-Auslöser"
#define OP_SMP_IN_NOTE_INFO		"Note"
#define OP_SMP_IN_ROOTKEY_INFO		"Basis-Note"
#define OP_SMP_IN_LOOPTYPE_INFO		"Loop-Typ"
#define OP_SMP_IN_LOOP0_INFO		"Loop-Start"
#define OP_SMP_IN_LOOP1_INFO		"Loop-Ende"
#define OP_SMP_IN_LOOPCOUNT_INFO	"Anzahl Loops"
#define OP_SMP_OUT_FILE_INFO		"Ausgang"
#define OP_SMP_OUT_TRIGGERS_INFO	"Loop-Start-Auslöser"
#define OP_SMP_OUT_TRIGGERL_INFO	"Loop-Wiederhol-Auslöser"
#define OP_SMP_OUT_DURA_INFO		"Aktuelle A-Dauer"
#define OP_SMP_OUT_DURT_INFO		"Aktuelle Gesamtdauer"
#define OP_SMP_OUT_DURFACTOR_INFO	"Aktueller Dauer-Faktor"
#define OP_SMP_OUT_ROOTKEY_INFO		"Basis-Note"
#define OP_SMP_ROUTINE_FIN_INFO		"Datei einlesen"
#define OP_SMP_ROUTINE_MODE_INFO	"Interpolationsmodus"
#define OP_SMP_CLASS_INFO \
\
"Sampler\n" \
"- Das Format ist RIFF WAVE (16-Bit, Mono, PCM). \"smpl\"-Chunk berücksichtigt:\n" \
"  Loop-Start, Loop-End, Loop-Type, Loop-Count.\n" \
"- Bei Dateien mit mehr als einem Kanal wird immer nur der erste verwendet.\n" \
"- Über die Routine kann die Datei neu geladen werden.\n" \
"- Der Instanznamen wird als Dateiname interpretiert.\n" \
"- Werte an den Eingängen haben höhre Priorität als die Werte aus der Datei.\n" \
"- Es kann \"nearest neighbour\" oder lineare Interpolation gewählt werden.\n" \
"- Loop-Typen: 0 = Kein, 1 = Vorwärts, 2 = Ping-Pong, 3 = Rückwärts.\n" \
"- Loop-Start/-Ende-Werte beziehen sich auf 1.0, was der Gesamtlänge entspricht.\n"

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpSampler::COpSampler()
: COp( 8, 7 )
, m_uiFileNameLen( 0 )
, m_uiFuncNum( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "smplr" );
	m_poIn[OP_SMP_IN_TRIGGERR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SMP_IN_NOTE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SMP_IN_ROOTKEY].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SMP_IN_LOOPTYPE].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SMP_IN_LOOP0].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SMP_IN_LOOP1].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_SMP_IN_LOOPCOUNT].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_SMP_IN_TRIGGERA, "ta", OP_SMP_IN_TRIGGERA_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SMP_IN_TRIGGERR, "tr", OP_SMP_IN_TRIGGERR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SMP_IN_NOTE, "nt", OP_SMP_IN_NOTE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SMP_IN_ROOTKEY, "nr", OP_SMP_IN_ROOTKEY_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SMP_IN_LOOPTYPE, "lt", OP_SMP_IN_LOOPTYPE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SMP_IN_LOOP0, "l0", OP_SMP_IN_LOOP0_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SMP_IN_LOOP1, "l1", OP_SMP_IN_LOOP1_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_SMP_IN_LOOPCOUNT, "lc", OP_SMP_IN_LOOPCOUNT_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SMP_OUT_FILE, "o", OP_SMP_OUT_FILE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SMP_OUT_TRIGGERS, "ts", OP_SMP_OUT_TRIGGERS_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SMP_OUT_TRIGGERL, "tl", OP_SMP_OUT_TRIGGERL_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SMP_OUT_DURA, "da", OP_SMP_OUT_DURA_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SMP_OUT_DURT, "dt", OP_SMP_OUT_DURT_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SMP_OUT_DURFACTOR, "df", OP_SMP_OUT_DURFACTOR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_SMP_OUT_ROOTKEY, "nr", OP_SMP_OUT_ROOTKEY_INFO );
	OP_SET_COUNT_ROUTINE( 2 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SMP_ROUTINE_FIN, "i" OP_STR_ROUTINE, OP_SMP_ROUTINE_FIN_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_SMP_ROUTINE_MODE, "f" OP_STR_ROUTINE, OP_SMP_ROUTINE_MODE_INFO );
	Update();
	Validate();
}

COpSampler::~COpSampler() 
{}

COpSampler & COpSampler::operator = ( const COpSampler &roOp )
{
	COp::operator = ( roOp );
	Update();
	m_oBuffer = roOp.m_oBuffer;
	//m_uiFileNameLen
	//m_auiFileName
	//m_uiFuncNum
	m_dBufferPos = roOp.m_dBufferPos;
	
	m_dMIDIRootKey = roOp.m_dMIDIRootKey;
	m_uiLoopType = roOp.m_uiLoopType;
	m_uiLoopStart = roOp.m_uiLoopStart;
	m_uiLoopEnd = roOp.m_uiLoopEnd;
	m_uiLoopPlayCount = roOp.m_uiLoopPlayCount;
	m_uiLoopCounter = roOp.m_uiLoopCounter;
	m_bIsSustained = roOp.m_bIsSustained;
	m_bIsFresh = roOp.m_bIsFresh;
	m_dStpSample = roOp.m_dStpSample;
	m_dSampleRateFactor = roOp.m_dSampleRateFactor;

	return *this;
}

void COpSampler::Init()
{
	const unsigned int uiCharIntMax = sizeof(m_auiFileName) / sizeof(m_auiFileName[0]);
	OP_SET_COUNT_INTERNAL_UINT( 2 + uiCharIntMax );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNum );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_uiFileNameLen );
	for( unsigned int i=0; i<uiCharIntMax; ++i )
		OP_SET_PTR_INTERNAL_UINT( 2 + i, &m_auiFileName[i] );
	COp::Init();
}

void COpSampler::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	
	const unsigned int uiSize( m_oBuffer.GetSize() );
	const unsigned int uiSampleIndexMax( uiSize - 1 );
	
	unsigned int uiLoopType( m_uiLoopType );
	if( m_poIn[OP_SMP_IN_LOOPTYPE].IsValid() )
	{
		int iLoopType( int( m_poIn[OP_SMP_IN_LOOPTYPE].Proc() ) );
		OP_RANGE_MINMAX( iLoopType, 0, 3 );
		uiLoopType = iLoopType;
	}
	
	double dLoopStart( m_uiLoopStart );
	double dLoopEnd( m_uiLoopEnd );
	if( m_poIn[OP_SMP_IN_LOOP0].IsValid() )
	{
		dLoopStart = m_poIn[OP_SMP_IN_LOOP0].Proc() * uiSampleIndexMax;
		OP_RANGE_MINMAX( dLoopStart, 0.0, uiSampleIndexMax );
	}
	if( m_poIn[OP_SMP_IN_LOOP1].IsValid() )
	{
		dLoopEnd = m_poIn[OP_SMP_IN_LOOP1].Proc() * uiSampleIndexMax;
		OP_RANGE_MINMAX( dLoopEnd, 0.0, uiSampleIndexMax );
	}
	if( dLoopStart > dLoopEnd )
	{
		OP_SWAP( dLoopStart, dLoopEnd, double );
	}
	
	if( m_poIn[OP_SMP_IN_TRIGGERA].Proc() > 0.0 )
	{
		m_dBufferPos = 0.0;
		m_bIsSustained = ( uiLoopType != CFileStreamWav::LOOP_NONE );
		m_bIsFresh = true;
		m_uiLoopCounter = 0;
		if( m_dStpSample < 0.0 )
			m_dStpSample = -m_dStpSample;
	}
	if( m_poIn[OP_SMP_IN_TRIGGERR].IsValid()
	 && m_poIn[OP_SMP_IN_TRIGGERR].Proc() > 0.0 )
	{
		m_bIsSustained = false;
		if( m_dStpSample < 0.0 )
			m_dStpSample = -m_dStpSample;
	}
	
	const double dRootKey(
		m_poIn[OP_SMP_IN_ROOTKEY].IsValid() ?
		m_poIn[OP_SMP_IN_ROOTKEY].Proc() :
		m_dMIDIRootKey );
	m_pdOut[OP_SMP_OUT_ROOTKEY] = dRootKey;
	
	const double dNoteScaleFactor(
		m_poIn[OP_SMP_IN_NOTE].IsValid() ? 
		pow( OP_SQRT_2_12, m_poIn[OP_SMP_IN_NOTE].Proc() - dRootKey ) :
		1.0 );
	
	m_dBufferPos += m_dStpSample * dNoteScaleFactor;
	
	const double dDurFac( 1.0 / dNoteScaleFactor );
	const double dDurFacS( m_dSampleRateFactor * dDurFac );
	m_pdOut[OP_SMP_OUT_DURA] = ( dLoopStart * dDurFacS );
	m_pdOut[OP_SMP_OUT_DURT] = ( double( uiSize ) * dDurFacS );
	m_pdOut[OP_SMP_OUT_DURFACTOR] = dDurFac;
	
	unsigned int uiLoopPlayCount( m_uiLoopPlayCount );
	if( m_poIn[OP_SMP_IN_LOOPCOUNT].IsValid() )
	{
		const double dLoopPlayCount( m_poIn[OP_SMP_IN_LOOPCOUNT].Proc() );
		if( dLoopPlayCount < 0.0 )
			uiLoopPlayCount = 0;
		else
			uiLoopPlayCount = (unsigned int)( dLoopPlayCount );
	}
	
	double dTriggerLoop( 0.0 );
	if( uiSize )
	{
		if( m_bIsSustained )
		{
			switch( uiLoopType )
			{
			//case CFileStreamWav::LOOP_NONE:
			//break;
			case CFileStreamWav::LOOP_FORWARD:
				if( m_dBufferPos >= dLoopEnd )
				{
					++m_uiLoopCounter;
					if( uiLoopPlayCount && m_uiLoopCounter >= uiLoopPlayCount )
					{
						m_bIsSustained = false;
					}
					else
					{
						m_dBufferPos = dLoopStart;
						dTriggerLoop = 1.0;
					}
				}
			break;
			case CFileStreamWav::LOOP_PINGPONG:
				if( m_dStpSample > 0.0 )
				{
					if( m_dBufferPos >= dLoopEnd )
					{
						++m_uiLoopCounter;
						if( uiLoopPlayCount && m_uiLoopCounter >= uiLoopPlayCount )
						{
							m_bIsSustained = false;
						}
						else
						{
							if( m_dBufferPos >= double( uiSize ) ) // Um nicht vorzeitig abzubrechen!
								m_dBufferPos = dLoopEnd;
							m_dStpSample = -m_dStpSample;
							dTriggerLoop = 1.0;
						}
					}
				}
				else
				{
					if( m_dBufferPos <= dLoopStart )
					{
						++m_uiLoopCounter;
						if( uiLoopPlayCount && m_uiLoopCounter >= uiLoopPlayCount )
						{
							m_bIsSustained = false;
							m_dStpSample = -m_dStpSample;
						}
						else
						{
							if( m_dBufferPos < 0.0 ) // Kein Wrapping erlauben.
								m_dBufferPos = dLoopStart;
							m_dStpSample = -m_dStpSample;
							dTriggerLoop = 1.0;
						}
					}
				}
			break;
			case CFileStreamWav::LOOP_BACKWARD:
				if( m_dStpSample > 0.0 )
				{
					if( m_dBufferPos >= dLoopEnd )
					{
						++m_uiLoopCounter;
						if( uiLoopPlayCount && m_uiLoopCounter >= uiLoopPlayCount )
						{
							m_bIsSustained = false;
						}
						else
						{
							if( m_dBufferPos >= double( uiSize ) ) // Um nicht vorzeitig abzubrechen!
								m_dBufferPos = dLoopEnd;
							m_dStpSample = -m_dStpSample; // nur einmal!
							dTriggerLoop = 1.0;
						}
					}
				}
				else
				{
					if( m_dBufferPos <= dLoopStart )
					{
						++m_uiLoopCounter;
						if( uiLoopPlayCount && m_uiLoopCounter >= uiLoopPlayCount )
						{
							m_bIsSustained = false;
							//m_dBufferPos = dLoopStart; // dLoopEnd // ???
							if( m_dBufferPos < 0.0 ) // Kein Wrapping erlauben.
								m_dBufferPos = dLoopStart;
							m_dStpSample = -m_dStpSample;
						}
						else
						{
							m_dBufferPos = dLoopEnd;
							dTriggerLoop = 1.0;
						}
					} 
				}
			break;
			}
		}
		if( m_dBufferPos < double( uiSize ) )
		{
			if( m_uiFuncNum )
			{
				m_pdOut[OP_SMP_OUT_FILE] = m_oBuffer.ReadWrap( m_dBufferPos ); // interpolated
			}
			else
			{
				m_pdOut[OP_SMP_OUT_FILE] = m_oBuffer.ReadWrap( (unsigned int)( floor( m_dBufferPos ) ) );
			}
		}
		else
		{
			m_pdOut[OP_SMP_OUT_FILE] = 0.0;
		}
	}
	else
	{
		m_pdOut[OP_SMP_OUT_FILE] = 0.0;
	}
	
	if( m_bIsFresh && ( m_dBufferPos > dLoopStart ) )
	{
		m_bIsFresh = false;
		m_pdOut[OP_SMP_OUT_TRIGGERS] = 1.0;
		dTriggerLoop = 1.0;
	}
	else
	{
		m_pdOut[OP_SMP_OUT_TRIGGERS] = 0.0;
	}
	m_pdOut[OP_SMP_OUT_TRIGGERL] = dTriggerLoop;
}

void COpSampler::Update()
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
	
	bool bSuccess = false;
	
	CFileStreamWav oWav;
	if( oWav.Open( pcFileName, CFile::FLAG_READ )
	 && oWav.m_uiChannels > 0
	// && ( oWav.m_uiChannels == 1 /* || oWav.m_uiChannels == 2*/  )
	)
	{
		const unsigned int uiValueCount( oWav.m_uiDataBytes / sizeof(signed short int) );
		const unsigned int uiSampleCount( uiValueCount / oWav.m_uiChannels );
		
		m_oBuffer.SetSize( uiSampleCount );
		m_oBuffer.SetMax( uiSampleCount );
		
		signed short int * pssiBuffer( new signed short int[ oWav.m_uiDataBytes ] );
		// <mod date="2010-12-10">
		if( oWav.ReadValues( pssiBuffer, uiValueCount ) )
		//if( oWav.Read( (unsigned char *)pssiBuffer, oWav.m_uiDataBytes ) )
		// </mod>
		{
			const unsigned int c( oWav.m_uiChannels );
			unsigned int s( 0 );
			for( unsigned int i = 0; i<uiSampleCount; ++i )
			{
				m_oBuffer.Write( i, pssiBuffer[s]  * ( 1.0 / AUDIO_VALUE_MAX ) );
				s += c;
			}
			bSuccess = true;
		}
		OP_DELETE_ARRAY( pssiBuffer );
		
		m_dMIDIRootKey = oWav.m_uiMIDIRootKey;
		m_uiLoopType = oWav.m_uiLoopType;
		m_uiLoopStart = oWav.m_uiLoopStart;
		m_uiLoopEnd = oWav.m_uiLoopEnd;
		m_uiLoopPlayCount = oWav.m_uiLoopPlayCount;
		m_dStpSample = double( oWav.m_uiSampleFreq ) / double( AUDIO_SAMPLE_FREQ );
		
		const unsigned int uiSampleIndexMax( uiSampleCount - 1 );
		if( m_uiLoopStart > uiSampleIndexMax )
			m_uiLoopStart = uiSampleIndexMax;
		if( m_uiLoopEnd > uiSampleIndexMax )
			m_uiLoopEnd = uiSampleIndexMax;
		if( m_uiLoopStart  > m_uiLoopEnd )
		{
			OP_SWAP( m_uiLoopStart, m_uiLoopEnd, unsigned int );
		}
		
		m_dSampleRateFactor = double( AUDIO_SAMPLE_FREQ ) / double( oWav.m_uiSampleFreq );
		//m_dSampleDurAttack = ( double( m_uiLoopStart ) / double( oWav.m_uiSampleFreq ) * double( AUDIO_SAMPLE_FREQ ) );
		//m_dSampleDur = ( double( uiSampleCount ) / double( oWav.m_uiSampleFreq ) * double( AUDIO_SAMPLE_FREQ ) );
	}
	else
	{
		m_oBuffer.Reset();
		m_dMIDIRootKey = 0.0;
		m_uiLoopType = 0;
		m_uiLoopStart = 0;
		m_uiLoopEnd = 0;
		m_uiLoopPlayCount = 0;
		m_dStpSample = 0.0;
		m_dSampleRateFactor = 0.0;
	}
	m_uiLoopCounter = 0;
	m_bIsSustained = false;
	m_bIsFresh = false;
	
	m_dBufferPos = m_oBuffer.GetSize(); // Nicht sofort abspilen.
	
	//m_pdOut[OP_SMP_OUT_ROOTKEY] = m_dMIDIRootKey;
	
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
	SetNameOutput( OP_SMP_OUT_FILE, bSuccess ? ( m_uiFuncNum == 1 ? "f(o)" : "o" ) : "?" );
#endif // OP_USE_RUNTIME_INFO
	
}

void COpSampler::Reset()
{
	m_oBuffer.Reset();
	m_bIsSustained = false;
	m_dBufferPos = m_oBuffer.GetSize(); // Nicht sofort abspielen.
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpSampler::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	//case OP_SMP_ROUTINE_FIN:
	//break;
	case OP_SMP_ROUTINE_MODE:
		m_uiFuncNum = !m_uiFuncNum;
	break;
	}
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpSampler, OP_SMP_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpSampler )
OP_GENERIC_METHODS_DEF( COpSampler )

#endif // OP_INC_OP_SAMPLER
