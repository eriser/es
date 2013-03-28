// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpFileWavIn.cpp

#include "OpFileWavIn.h"
#ifdef OP_INC_OP_FILE_WAV_IN

////////////////////////////////////////////////////////////////////////////////
// COpFileWavIn
////////////////////////////////////////////////////////////////////////////////

#define OP_FWI_IN_POSR			0
#define OP_FWI_OUT_FILE			0
#define OP_FWI_OUT_SIZE			1
#define OP_FWI_ROUTINE_FIN		0

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_FWI_IN_POSR_INFO		"read position"
#define OP_FWI_OUT_FILE_INFO		"output"
#define OP_FWI_OUT_SIZE_INFO		"size"
#define OP_FWI_ROUTINE_FIN_INFO		"load file"
#define OP_FWI_CLASS_INFO \
\
"Wave Input File\n" \
"- The format is RIFF WAVE (16-Bit, Mono, PCM).\n" \
"- When a file has more than one channel, the only the first one will be used.\n" \
"- The routine reloads the file.\n" \
"- The instance name is interpreted as file name.\n" \
"- The size output tells the file size in samples.\n" \
"- If the read position is not an integer, interpolation happens.\n" \
"- Invalid read positions are always mapped to the actual size of the\n" \
"  file, as if it were a cyclic buffer."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_FWI_IN_POSR_INFO		"Lese-Position"
#define OP_FWI_OUT_FILE_INFO		"Ausgang"
#define OP_FWI_OUT_SIZE_INFO		"Grösse"
#define OP_FWI_ROUTINE_FIN_INFO		"Datei einlesen"
#define OP_FWI_CLASS_INFO \
\
"Wave-Eingabe-Datei\n" \
"- Das Format ist RIFF WAVE (16-Bit, Mono, PCM).\n" \
"- Bei Dateien mit mehr als einem Kanal wird immer nur der erste verwendet.\n" \
"- Über die Routine kann die Datei neu geladen werden.\n" \
"- Der Instanznamen wird als Dateiname interpretiert.\n" \
"- Der Grösse-Ausgang gibt die Dateigrösse in Samples an.\n" \
"- Ist die Lese-Position nicht ganzzahlig, wird ein linear interpolierter\n" \
"  Wert ausgegeben.\n" \
"- Ungültige Lese-Positionen werden so behandelt, als wäre der Puffer\n" \
"  kreisförmig."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpFileWavIn::COpFileWavIn()
: COp( 1, 2 )
, m_uiFileNameLen( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "wavIn" );
	OP_SET_NAME_AND_INFO_INPUT( OP_FWI_IN_POSR, "r", OP_FWI_IN_POSR_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FWI_OUT_FILE, "o", OP_FWI_OUT_FILE_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_FWI_OUT_SIZE, "s", OP_FWI_OUT_SIZE_INFO );
	OP_SET_COUNT_ROUTINE( 1 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_FWI_ROUTINE_FIN, "i" OP_STR_ROUTINE, OP_FWI_ROUTINE_FIN_INFO );
	Update();
	Validate();
}

COpFileWavIn::~COpFileWavIn() 
{}

COpFileWavIn & COpFileWavIn::operator = ( const COpFileWavIn &roOp )
{
	COp::operator = ( roOp );
	Update();
	m_oBuffer = roOp.m_oBuffer;
	return *this;
}

void COpFileWavIn::Init()
{
	const unsigned int uiCharIntMax = sizeof(m_auiFileName) / sizeof(m_auiFileName[0]);
	OP_SET_COUNT_INTERNAL_UINT( 1 + uiCharIntMax );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFileNameLen );
	for( unsigned int i=0; i<uiCharIntMax; ++i )
		OP_SET_PTR_INTERNAL_UINT( 1 + i, &m_auiFileName[i] );
	COp::Init();
}

void COpFileWavIn::Proc() 
{
	OP_GENERIC_CODE_PROC_PROLOG();
	const int iBufferPos = int( m_poIn[OP_FWI_IN_POSR].Proc() );
	const unsigned int uiSize = m_oBuffer.GetSize();
	if( uiSize )
		m_pdOut[OP_FWI_OUT_FILE] = m_oBuffer.ReadWrap( iBufferPos );
	//m_pdOut[OP_FWI_OUT_SIZE] = double( uiSize );
}

void COpFileWavIn::Update()
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
		// <mod>
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
	}
	
	m_pdOut[OP_FWI_OUT_SIZE] = double( m_oBuffer.GetSize() );
	
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
	SetNameOutput( OP_FWI_OUT_FILE, bSuccess ? "f" : "?" );
#endif // OP_USE_RUNTIME_INFO
	
}

void COpFileWavIn::Reset()
{
	m_oBuffer.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpFileWavIn::Routine( unsigned int uiIndex )
{
	Update();
	Validate();
}
#endif

OP_SET_NAME_AND_INFO_CLASS( COpFileWavIn, OP_FWI_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpFileWavIn )
OP_GENERIC_METHODS_DEF( COpFileWavIn )

#endif // OP_INC_OP_FILE_WAV_IN
