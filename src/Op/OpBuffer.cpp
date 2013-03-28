// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpBuffer.cpp

#include "OpBuffer.h"
#ifdef OP_INC_OP_BUFFER

////////////////////////////////////////////////////////////////////////////////
// COpBuffer
////////////////////////////////////////////////////////////////////////////////

#define OP_BFF_IN_POSR			0
#define OP_BFF_IN_SIZE			1
#define OP_BFF_IN_CLEAR			2
#define OP_BFF_IN_SRC			3
#define OP_BFF_IN_POSW			4
#define OP_BFF_OUT_DST			0
#define OP_BFF_ROUTINE_IREAD		0
#define OP_BFF_ROUTINE_EXPAND		1
#define OP_BFF_ROUTINE_SHRINK		2
#define OP_BFF_ROUTINE_MOD		3
#define OP_BFF_FLAG_SET			0
#define OP_BFF_FLAG_ADD			1
#define OP_BFF_FLAG_MULT		2
#define OP_BFF_FLAG_MAX			3

#if defined( OP_LANGUAGE_ENGLISH )

#define OP_BFF_IN_POSR_INFO		"read position"
#define OP_BFF_IN_SIZE_INFO		"size"
#define OP_BFF_IN_CLEAR_INFO		"reset"
#define OP_BFF_IN_SRC_INFO		"input"
#define OP_BFF_IN_POSW_INFO		"write position"
#define OP_BFF_OUT_DST_INFO		"output"
#define OP_BFF_ROUTINE_IREAD_INFO	"read interpolation mode"
#define OP_BFF_ROUTINE_EXPAND_INFO	"add write head"
#define OP_BFF_ROUTINE_SHRINK_INFO	"remove write head"
#define OP_BFF_ROUTINE_MOD_INFO		"write mode"
#define OP_BFF_CLASS_INFO \
\
"Buffer\n" \
"- First, the value at each read position will be written to the corresponding output,\n" \
"  then the input values will be written to the buffer at each write position.\n" \
"- Invalid write and read positions are always mapped onto the size of the buffer.\n" \
"- From a negative size, its absolute value is used. Minimal size: 1.\n" \
"- When the reset input value is greater than 0.0, then the buffer will be reset\n" \
"  to 0.0 according to the specified size.\n" \
"- There are maximally 16 write heads.\n" \
"- Each write position is rounded.\n" \
"- For negative positions, the write process will not be executed. Then, the\n" \
"  corresponding output will also be inactive.\n" \
"- When one or both of the input or write position inlets are not connected, then\n" \
"  the write process is also not executed.\n" \
"- Changes on the size inlet only lead to an effective resizing, when the new size\n" \
"  is less than the current internal buffer. Therefore, dynamic size modification\n" \
"  is efficient enough when restricted to a certain range.\n" \
"- With activated read interpolation, operations on the buffer are interpolated for\n" \
"  non-integral read positions.\n" \
"- The write mode defines the mix function for each write head:\n" \
"  = (overwrite), + (add), * (multiply)."

#elif defined( OP_LANGUAGE_GERMAN )

#define OP_BFF_IN_POSR_INFO		"Lese-Position"
#define OP_BFF_IN_SIZE_INFO		"Grösse"
#define OP_BFF_IN_CLEAR_INFO		"Zurücksetzen"
#define OP_BFF_IN_SRC_INFO		"Eingang"
#define OP_BFF_IN_POSW_INFO		"Schreib-Position"
#define OP_BFF_OUT_DST_INFO		"Ausgang"
#define OP_BFF_ROUTINE_IREAD_INFO	"Lese-Interpolationsmodus"
#define OP_BFF_ROUTINE_EXPAND_INFO	"Schreib-Kopf hinzufügen"
#define OP_BFF_ROUTINE_SHRINK_INFO	"Schreib-Kopf entfernen"
#define OP_BFF_ROUTINE_MOD_INFO		"Schreib-Kopf Modus"
#define OP_BFF_CLASS_INFO \
\
"Puffer\n" \
"- Zuerst wird der gespeicherte Wert an der Lese-Position ausgegeben, danach\n" \
"  der/die Eingabe-Wert(e) an der/den Schreib-Position(en) in den Puffer ge-\n" \
"  schrieben.\n" \
"- Ungültige Schreib-/Lese-Positionen werden so behandelt, als wäre der Puffer\n" \
"  kreisförmig.\n" \
"- Von negativen Grössenangaben wird der Betrag genommen. Mindest-Grösse: 1.0.\n" \
"- Ist der Zurücksetzen-Eingang grösser als 0.0, so wird die Puffer-Grösse gemäss\n" \
"  dem Grösse-Eingang fixiert und komplett auf 0.0 zurückgesetzt.\n" \
"- Es gibt max. 16 Schreibköpfe.\n" \
"- Jede Schreib-Position wird auf ganze Zahlen gerundet.\n" \
"- Der Schreib-Vorgang wird für negativen Positionen nicht ausgeführt. Dann ist\n" \
"  auch die zugehörige Eingabe inaktiv.\n" \
"- Ist einer oder beide der Eingang- oder Schrei-Positions-Eingänge nicht ver-\n" \
"  bunden, so wird die Schreib-Operation ebenfalls nicht ausgeführt!\n" \
"- Änderungen am Grösse-Eingang führen nur zu einer wirklichen Grössenänderung,\n" \
"  wenn die erwünschte Grösse grösser ist als der interne Puffer. Dynamische\n" \
"  Grössenänderungen sind somit in einem bestimmten Bereich sehr effizent.\n" \
"- Bei aktivierter Lese-Interpolation werden Zugriff auf den Puffer bei nicht-\n" \
"  ganzzahligen Lese-Positionen linear interpoliert.\n" \
"- Der Schreib-Kopf Modus bestimmt das Mix-Verhalten jedes einzelnen Schreib-\n" \
"  Kopfes. Es stehen = (speichern), + (addieren) und * (multiplizieren) zur Ver-\n" \
"  fügung."

#else
#error language not specified
#endif // OP_LANGUAGE_*

COpBuffer::COpBuffer()
: COp( 5, 1 )
, m_uiFuncNumIRead( 0 )
, m_uiModeWrite( 0 )
{
	Init();
	OP_SET_NAME_INSTANCE( "buffer" );
	m_poIn[OP_BFF_IN_CLEAR].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_BFF_IN_SRC].SetFlags( OP_LINK_FLAG_OPTIONAL );
	m_poIn[OP_BFF_IN_POSW].SetFlags( OP_LINK_FLAG_OPTIONAL );
	OP_SET_NAME_AND_INFO_INPUT( OP_BFF_IN_POSR, "r", OP_BFF_IN_POSR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_BFF_IN_SIZE, "s", OP_BFF_IN_SIZE_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_BFF_IN_CLEAR, "cl", OP_BFF_IN_CLEAR_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_BFF_IN_SRC, "i", OP_BFF_IN_SRC_INFO );
	OP_SET_NAME_AND_INFO_INPUT( OP_BFF_IN_POSW, "w", OP_BFF_IN_POSW_INFO );
	OP_SET_NAME_AND_INFO_OUTPUT( OP_BFF_OUT_DST, "o", OP_BFF_OUT_DST_INFO );
	OP_SET_COUNT_ROUTINE( 4 );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BFF_ROUTINE_IREAD, "f" OP_STR_ROUTINE, OP_BFF_ROUTINE_IREAD_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BFF_ROUTINE_EXPAND, "+" OP_STR_ROUTINE, OP_BFF_ROUTINE_EXPAND_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BFF_ROUTINE_SHRINK, "-" OP_STR_ROUTINE, OP_BFF_ROUTINE_SHRINK_INFO );
	OP_SET_NAME_AND_INFO_ROUTINE( OP_BFF_ROUTINE_MOD, "=" OP_STR_ROUTINE, OP_BFF_ROUTINE_MOD_INFO );
	SetFlags( OP_FLAG_DYNAMIC_INPUTS );
	m_oBuffer.SetSize( 1 );
	Reset();
	Update();
	Validate();
}

COpBuffer::~COpBuffer()
{}

COpBuffer & COpBuffer::operator = ( const COpBuffer &roOp )
{
	COp::operator = ( roOp ); // m_uiFuncNumIRead, m_uiModeWrite
	Update();
	m_oBuffer = roOp.m_oBuffer;
	return *this;
}

void COpBuffer::Init()
{
	OP_SET_COUNT_INTERNAL_UINT( 2 );
	OP_SET_PTR_INTERNAL_UINT( 0, &m_uiFuncNumIRead );
	OP_SET_PTR_INTERNAL_UINT( 1, &m_uiModeWrite );
	COp::Init();
}

void COpBuffer::Proc()
{	
	OP_GENERIC_CODE_PROC_PROLOG();
	
	if( m_poIn[OP_BFF_IN_CLEAR].IsValid() )
		if( m_poIn[OP_BFF_IN_CLEAR].Proc() > 0.0 )
			Reset();
	
	unsigned int uiSize = (unsigned int)abs( (int)rint( m_poIn[OP_BFF_IN_SIZE].Proc() ) );
	if( uiSize > m_oBuffer.GetSize() )
		m_oBuffer.SetSize( uiSize );
	if( uiSize != m_oBuffer.GetMax() )
		m_oBuffer.SetMax( uiSize );
	
	const double dPosR( m_poIn[OP_BFF_IN_POSR].Proc() );
	
	if( uiSize == 0 ) // new (2010-08-26)
		m_pdOut[OP_BFF_OUT_DST] = 0.0;
	else if( m_uiFuncNumIRead )
		m_pdOut[OP_BFF_OUT_DST] = m_oBuffer.ReadWrap( dPosR );
	else
		m_pdOut[OP_BFF_OUT_DST] = m_oBuffer.ReadWrap( (int)rint( dPosR ) );
	
	unsigned int uiMask = 3;
	for( unsigned int p = OP_BFF_IN_SRC; p < m_uiCountIn; p += 2, uiMask <<= 2 )
	{
		if( m_poIn[p+1].IsValid() && m_poIn[p].IsValid() )
		{
			int iPosW = 0;
			if( uiSize )
			{
				const double dPosW = m_poIn[p+1].Proc();
				iPosW = (int)rint( dPosW );
				iPosW %= (int)uiSize;
				if( iPosW < 0 )
				{
					continue; // ACHTUNG: Neu!
					//iPosW += uiSize;
				}
			}
			double dVal = m_poIn[p].Proc();
			const unsigned int uiFlags = ( m_uiModeWrite & uiMask ) >> ( p - OP_BFF_IN_SRC );
			if( uiFlags )
			{
				const double dW = m_oBuffer.Read( iPosW );
				if( uiFlags == OP_BFF_FLAG_MULT )
					dVal *= dW;
				else if( uiFlags == OP_BFF_FLAG_ADD )
					dVal += dW;
			}
			m_oBuffer.Write( iPosW, dVal );
		}
	}
}

void COpBuffer::Update()
{
#ifdef OP_USE_RUNTIME_INFO
	SetNameOutput( OP_BFF_OUT_DST, ( m_uiFuncNumIRead ? "f(o)" : "o" ) );
	
	unsigned int i = OP_BFF_IN_POSW + 1;
	while( i < m_uiCountIn )
	{
		SetNameInput( i, "i" );
		SetInfoInput( i++, OP_BFF_IN_SRC_INFO );
		SetNameInput( i, "w" );
		SetInfoInput( i++, OP_BFF_IN_POSW_INFO );
	}
	unsigned int uiMask = 3;
	unsigned int j = OP_BFF_ROUTINE_MOD;
	unsigned int uiRoutineCount = GetCountRoutine();
	const unsigned int uiRoutineCountNew = 4 + ( m_uiCountIn - 5 );
	if( uiRoutineCount != uiRoutineCountNew )
	{
		SetCountRoutine( uiRoutineCountNew );
		while( uiRoutineCount < uiRoutineCountNew )
		{
			SetInfoRoutine( uiRoutineCount++, OP_STR_ROUTINE_DUMMY );
			SetInfoRoutine( uiRoutineCount++, OP_BFF_ROUTINE_MOD_INFO );
		}
	}
	while( j < uiRoutineCount )
	{
		const unsigned int uiPos = ( j - OP_BFF_ROUTINE_MOD );
		const unsigned int uiFalgs = ( m_uiModeWrite & uiMask ) >> uiPos;
		switch( uiFalgs )
		{
		case OP_BFF_FLAG_SET:
			SetNameRoutine( j, "=" OP_STR_ROUTINE );
		break;
		case OP_BFF_FLAG_ADD:
			SetNameRoutine( j, "+" OP_STR_ROUTINE );
		break;
		case OP_BFF_FLAG_MULT:
			SetNameRoutine( j, "*" OP_STR_ROUTINE );
		break;
		}
		j += 2;
		uiMask <<= 2;
	}
#endif // OP_USE_RUNTIME_INFO
}

void COpBuffer::Reset()
{
	if( m_poIn[OP_BFF_IN_SIZE].IsValid() )
	{
		double dSize = m_poIn[OP_BFF_IN_SIZE].Proc();
		unsigned int uiSize = (unsigned int)abs( (int)rint( dSize ) );
		if( uiSize != m_oBuffer.GetSize() )
		{
			if( !uiSize )
				++uiSize;
			m_oBuffer.SetSize( uiSize );
		}
	}
	m_oBuffer.Reset();
	COp::Reset();
}

#ifdef OP_USE_ROUTINES
void COpBuffer::Routine( unsigned int uiIndex )
{
	switch( uiIndex )
	{
	case OP_BFF_ROUTINE_IREAD:
		m_uiFuncNumIRead = !m_uiFuncNumIRead;
	break;
	case OP_BFF_ROUTINE_SHRINK:
		if( m_uiCountIn > 5 )
		{
			SetCountIn( m_uiCountIn - 2 );
			SetCountRoutine( GetCountRoutine() - 2 );
		}
	break;
	case OP_BFF_ROUTINE_EXPAND:
		if( m_uiCountIn < 3 + 16 * 2 ) // Max. 16 Schreibköpfe.
		{
			SetCountIn( m_uiCountIn + 2 );
			m_poIn[m_uiCountIn - 1].SetFlags( OP_LINK_FLAG_OPTIONAL );
			m_poIn[m_uiCountIn - 2].SetFlags( OP_LINK_FLAG_OPTIONAL );
		}
	break;
	default:
		if( uiIndex >= OP_BFF_ROUTINE_MOD )
		{
			const unsigned int uiDiff = uiIndex - ( OP_BFF_ROUTINE_MOD - 1 );
			if(  uiDiff % 2 == 0 ) // Dummy.
				break;
			const unsigned int uiPos = uiDiff - 1;
			const unsigned int uiMask = ( 3 << uiPos );
			unsigned int uiState = ( m_uiModeWrite & uiMask ) >> uiPos;
			++uiState;
			if( uiState >= OP_BFF_FLAG_MAX )
				uiState = 0;
			m_uiModeWrite &= ~uiMask;
			m_uiModeWrite |= ( uiState << uiPos );
		}
	}
	Update();
}
#endif // OP_USE_ROUTINES

OP_SET_NAME_AND_INFO_CLASS( COpBuffer, OP_BFF_CLASS_INFO )
OP_GENERIC_COPY_CTOR_DEF( COpBuffer )
OP_GENERIC_METHODS_DEF( COpBuffer )

#endif // OP_INC_OP_BUFFER
