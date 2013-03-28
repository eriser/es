// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpTkFlt.inline.h


////////////////////////////////////////////////////////////////////////////////
// COpTkFltAllpass1
////////////////////////////////////////////////////////////////////////////////

template<class T>
COpTkFltAllpass1<T>::COpTkFltAllpass1()
: m_tOut( T(0.0) )
, m_tCoeff( T(0.0) )
{}

template<class T>
COpTkFltAllpass1<T>::COpTkFltAllpass1( const COpTkFltAllpass1<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkFltAllpass1<T> & COpTkFltAllpass1<T>::operator =( const COpTkFltAllpass1 &roO )
{
	m_tCoeff = roO.m_tCoeff;
	m_tOut = roO.m_tOut;
	return *this;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkFltAllpass2
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltAllpass2<T, SAMPLE_FREQ>::COpTkFltAllpass2()
: m_tIn0( T(0.0) )
, m_tIn1( T(0.0) )
, m_tOut0( T(0.0) )
, m_tOut1( T(0.0) )
, m_tRadius( T(0.0) )
, m_tA0( T(0.0) )
, m_tA1( T(0.0) )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltAllpass2<T, SAMPLE_FREQ>::COpTkFltAllpass2( const COpTkFltAllpass2<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltAllpass2<T, SAMPLE_FREQ> & COpTkFltAllpass2<T, SAMPLE_FREQ>::operator =( const COpTkFltAllpass2 &roO )
{
	m_tRadius = roO.m_tRadius;
	m_tA0 = roO.m_tA0;
	m_tA1 = roO.m_tA1;
	m_tIn0 = roO.m_tIn0;
	m_tIn1 = roO.m_tIn1;
	m_tOut0 = roO.m_tOut0;
	m_tOut1 = roO.m_tOut1;
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkFltAllpass2<T, SAMPLE_FREQ>::Init( T tFreq, T tRadius )
{
	InitRadius( tRadius );
	InitFreq( tFreq );
}


////////////////////////////////////////////////////////////////////////////////
// COpTkFltLow0
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltLow0<T, SAMPLE_FREQ>::COpTkFltLow0()
: m_tVibraPos( T(0.0) )
, m_tVibraSpeed( T(0.0) )
, m_tC( T(0.0) )
, m_tR( T(0.0) )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltLow0<T, SAMPLE_FREQ>::COpTkFltLow0( const COpTkFltLow0<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltLow0<T, SAMPLE_FREQ> & COpTkFltLow0<T, SAMPLE_FREQ>::operator =( const COpTkFltLow0<T, SAMPLE_FREQ> &roO )
{
	m_tC = roO.m_tC;
	m_tR = roO.m_tR;
	m_tVibraPos = roO.m_tVibraPos;
	m_tVibraSpeed = roO.m_tVibraSpeed;
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkFltLow0<T, SAMPLE_FREQ>::Init( T tFreq )
{
	const T tTheta = fabs( tFreq ) * ( OP_TK_PI2 / SAMPLE_FREQ );
	if( tTheta == 0.0 )
		return;

	const T tQ = T(1.0) - tTheta /
		( ( T(3.0) / ( T(1.0) + tTheta ) ) + tTheta - T(2.0) );
	m_tR = tQ * tQ;
	m_tC = m_tR + T(1.0) - T(2.0) * cos( tTheta ) * tQ;
}


////////////////////////////////////////////////////////////////////////////////
// COpTkFltHighLow0
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltHighLow0<T, SAMPLE_FREQ>::COpTkFltHighLow0()
: m_tIn0( T(0.0) )
, m_tIn1( T(0.0) )
, m_tOut0( T(0.0) )
, m_tOut1( T(0.0) )
, m_tA0( T(0.0) )
, m_tA1( T(0.0) )
, m_tB0( T(0.0) )
, m_tB1( T(0.0) )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltHighLow0<T, SAMPLE_FREQ>::COpTkFltHighLow0( const COpTkFltHighLow0<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltHighLow0<T, SAMPLE_FREQ> & COpTkFltHighLow0<T, SAMPLE_FREQ>::operator =( const COpTkFltHighLow0<T, SAMPLE_FREQ> &roO )
{
	m_tIn0 = roO.m_tIn0;
	m_tIn1 = roO.m_tIn1;
	m_tOut0 = roO.m_tOut0;
	m_tOut1 = roO.m_tOut1;
	m_tA0 = roO.m_tA0;
	m_tA1 = roO.m_tA1;
	m_tB0 = roO.m_tB0;
	m_tB1 = roO.m_tB1;
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkFltHighLow0<T, SAMPLE_FREQ>::Init( bool bHigh, T tFreq, T tRes )
{
	if( tFreq < T(0.0) )
		tFreq = -tFreq;
	if( tRes < T(0.0) )
		tRes = -tRes;

	tRes = T(1.0) - tRes;

	if( bHigh )
	{
		const T c = tan( OP_TK_PI * tFreq * T( 1.0 / SAMPLE_FREQ ) );
		const T csq = c * c;

		m_tA0 = T(1.0) / ( T(1.0) + tRes * c + csq );
		m_tA1 = T(-2.0) * m_tA0;
		m_tB0 = T(2.0) * ( csq - T(1.0) ) * m_tA0;
		m_tB1 = ( T(1.0) - tRes * c + csq ) * m_tA0;
	}
	else
	{
		if( tFreq == T(0.0) )
			tFreq = T(1e-12);
		const T c = T(1.0) / tan( OP_TK_PI * tFreq * T( 1.0 / SAMPLE_FREQ ) );
		const T csq = c * c;

		m_tA0 = T(1.0) / ( T(1.0) + tRes * c + csq );
		m_tA1 = T(2.0) * m_tA0;
		m_tB0 = T(2.0) * ( T(1.0) - csq ) * m_tA0;
		m_tB1 = ( T(1.0) - tRes * c + csq ) * m_tA0;
	}
}



////////////////////////////////////////////////////////////////////////////////
// COpTkFltBiquad
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltBiquad<T, SAMPLE_FREQ>::COpTkFltBiquad()
: m_tIn0( T(0.0) )
, m_tIn1( T(0.0) )
, m_tOut0( T(0.0) )
, m_tOut1( T(0.0) )
, m_tA0( T(0.0) )
, m_tA1( T(0.0) )
, m_tA2( T(0.0) )
, m_tB0( T(0.0) )
, m_tB1( T(0.0) )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltBiquad<T, SAMPLE_FREQ>::COpTkFltBiquad( const COpTkFltBiquad<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltBiquad<T, SAMPLE_FREQ> & COpTkFltBiquad<T, SAMPLE_FREQ>::operator =( const COpTkFltBiquad<T, SAMPLE_FREQ> &roO )
{
	m_tIn0 = roO.m_tIn0;
	m_tIn1 = roO.m_tIn1;
	m_tOut0 = roO.m_tOut0;
	m_tOut1 = roO.m_tOut1;
	m_tA0 = roO.m_tA0;
	m_tA1 = roO.m_tA1;
	m_tA2 = roO.m_tA2;
	m_tB0 = roO.m_tB0;
	m_tB1 = roO.m_tB1;
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkFltBiquad<T, SAMPLE_FREQ>::Init( unsigned int uiType, T tFreq, T tGain, T tBandwidth )
{
	if( tFreq < T(0.0) )
		tFreq = -tFreq;
	if( tGain < T(0.0) )
		tGain = -tGain;
	if( tBandwidth < T(0.0) )
		tBandwidth = -tBandwidth;

	const T A = pow( T(10.0), tGain / T(40.0) );
	const T omega = OP_TK_PI2 * tFreq * T( 1.0 / SAMPLE_FREQ );
	const T sn = sin( omega );
	const T cs = cos( omega );
	const T alpha = sn * sinh( T( OP_TK_LN_2 * 0.5 ) * tBandwidth * omega /sn );
	const T beta = sqrt( A + A );

	T a0=T(0.0), a1=T(0.0), a2=T(0.0), b0=T(0.0), b1=T(0.0), b2=T(0.0);

	switch( uiType )
	{
	case TYPE_LPF:
		b0 = (T(1.0) - cs) * T(0.5);
		b1 = T(1.0) - cs;
		b2 = (T(1.0) - cs) * T(0.5);
		a0 = T(1.0) + alpha;
		a1 = -T(2.0) * cs;
		a2 = T(1.0) - alpha;
	break;
	case TYPE_HPF:
		b0 = ( T(1.0) + cs ) * T(0.5);
		b1 = -( T(1.0) + cs );
		b2 = ( T(1.0) + cs) * T(0.5);
		a0 = T(1.0) + alpha;
		a1 = -T(2.0) * cs;
		a2 = T(1.0) - alpha;
	break;
	case TYPE_BPF:
		b0 = alpha;
		b1 = T(0.0);
		b2 = -alpha;
		a0 = T(1.0) + alpha;
		a1 = -T(2.0) * cs;
		a2 = T(1.0) - alpha;
	break;
	case TYPE_NOTCH:
		b0 = T(1.0);
		b1 = -T(2.0) * cs;
		b2 = T(1.0);
		a0 = T(1.0) + alpha;
		a1 = -T(2.0) * cs;
		a2 = T(1.0) - alpha;
	break;
	case TYPE_PEQ:
		b0 = T(1.0) + (alpha * A);
		b1 = -T(2.0) * cs;
		b2 = T(1.0) - (alpha * A);
		a0 = T(1.0) + (alpha /A);
		a1 = -T(2.0) * cs;
		a2 = T(1.0) - (alpha /A);
	break;
	case TYPE_LSH:
		b0 = A * ((A + T(1.0)) - (A - T(1.0)) * cs + beta * sn);
		b1 = T(2.0) * A * ((A - T(1.0)) - (A + T(1.0)) * cs);
		b2 = A * ((A + T(1.0)) - (A - T(1.0)) * cs - beta * sn);
		a0 = (A + T(1.0)) + (A - T(1.0)) * cs + beta * sn;
		a1 = -T(2.0) * ((A - T(1.0)) + (A + T(1.0)) * cs);
		a2 = (A + T(1.0)) + (A - T(1.0)) * cs - beta * sn;
	break;
	case TYPE_HSH:
		b0 = A * ((A + T(1.0)) + (A - T(1.0)) * cs + beta * sn);
		b1 = -T(2.0) * A * ((A - T(1.0)) + (A + T(1.0)) * cs);
		b2 = A * ((A + T(1.0)) + (A - T(1.0)) * cs - beta * sn);
		a0 = (A + T(1.0)) - (A - T(1.0)) * cs + beta * sn;
		a1 = T(2.0) * ((A - T(1.0)) - (A + T(1.0)) * cs);
		a2 = (A + T(1.0)) - (A - T(1.0)) * cs - beta * sn;
	break;
	}

	m_tA0 = b0 / a0;
	m_tA1 = b1 / a0;
	m_tA2 = b2 / a0;
	m_tB0 = a1 / a0;
	m_tB1 = a2 / a0;

	//Reset();
}


////////////////////////////////////////////////////////////////////////////////
// COpTkFltRes0
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltRes0<T, SAMPLE_FREQ>::COpTkFltRes0()
: m_tOut0( T(0.0) )
, m_tOut1( T(0.0) )
, m_tF( T(0.0) )
, m_tFB( T(0.0) )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltRes0<T, SAMPLE_FREQ>::COpTkFltRes0( const COpTkFltRes0<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltRes0<T, SAMPLE_FREQ> & COpTkFltRes0<T, SAMPLE_FREQ>::operator =( const COpTkFltRes0<T, SAMPLE_FREQ> &roO )
{
	m_tOut0 = roO.m_tOut0;
	m_tOut1 = roO.m_tOut1;
	m_tF = roO.m_tF;
	m_tFB = roO.m_tFB;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
// COpTkFltTB303
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltTB303<T, SAMPLE_FREQ>::COpTkFltTB303()
: m_tA0( T(0.0) )
, m_tA1( T(0.0) )
, m_tB( T(0.0) )
, m_tOut0( T(0.0) )
, m_tOut1( T(0.0) )
, m_tResCoeff( T(0.0) )
, m_tE0( T(0.0) )
, m_tE1( T(0.0) )
, m_uiPos( 0 )
{}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltTB303<T, SAMPLE_FREQ>::COpTkFltTB303( const COpTkFltTB303<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltTB303<T, SAMPLE_FREQ> & COpTkFltTB303<T, SAMPLE_FREQ>::operator =( const COpTkFltTB303<T, SAMPLE_FREQ> &roO )
{
	m_tA0 = roO.m_tA0;
	m_tA1 = roO.m_tA1;
	m_tB = roO.m_tB;
	m_tOut0 = roO.m_tOut0;
	m_tOut1 = roO.m_tOut1;
	m_tResCoeff = roO.m_tResCoeff;
	m_tE0 = roO.m_tE0;
	m_tE1 = roO.m_tE1;
	m_uiPos = roO.m_uiPos;
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkFltTB303<T, SAMPLE_FREQ>::Init( T tFreq, T tRes )
{
	tFreq *= T( OP_TK_PI / SAMPLE_FREQ );
	m_tResCoeff = T( exp( -1.20 + 3.455 * tRes ) );
	m_tE1 = T( exp( 6.109 /*+ 1.5876 * vcf_envmod*/ + 2.1553 * tFreq - 1.2 * ( 1.0 - tRes ) ) );
	m_tE0 = T( exp( 5.613 /*- 0.8 * vcf_envmod*/ + 2.1553 * tFreq - 0.7696 * ( 1.0 - tRes ) ) );
	m_tE0 *= T( OP_TK_PI / SAMPLE_FREQ );
	m_tE1 *= T( OP_TK_PI / SAMPLE_FREQ );
	m_tE1 -= m_tE0;
}

////////////////////////////////////////////////////////////////////////////////
// COpTkFltMoog0
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltMoog0<T, SAMPLE_FREQ>::COpTkFltMoog0()
: m_tIn0( T(0.0) )
, m_tIn1( T(0.0) )
, m_tIn2( T(0.0) )
, m_tIn3( T(0.0) )
, m_tOut0( T(0.0) )
, m_tOut1( T(0.0) )
, m_tOut2( T(0.0) )
, m_tOut3( T(0.0) )
, m_tF( T(0.0) )
, m_tFB( T(0.0) )
, m_tOneMinusF( T(0.0) )
, m_tFX( T(0.0) )
{
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltMoog0<T, SAMPLE_FREQ>::COpTkFltMoog0( const COpTkFltMoog0<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltMoog0<T, SAMPLE_FREQ> & COpTkFltMoog0<T, SAMPLE_FREQ>::operator =( const COpTkFltMoog0<T, SAMPLE_FREQ> &roO )
{
	m_tIn0 = roO.m_tIn0;
	m_tIn1 = roO.m_tIn1;
	m_tIn2 = roO.m_tIn2;
	m_tIn3 = roO.m_tIn3;
	m_tOut0 = roO.m_tOut0;
	m_tOut1 = roO.m_tOut1;
	m_tOut2 = roO.m_tOut2;
	m_tOut3 = roO.m_tOut3;
	m_tF = roO.m_tF;
	m_tFB = roO.m_tFB;
	m_tOneMinusF = roO.m_tOneMinusF;
	m_tFX = roO.m_tFX;
	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkFltMoog0<T, SAMPLE_FREQ>::Init( T tFreq, T tRes )
{
	//m_tF = tFreq * T(1.16);
	m_tF = T(4.0) * tFreq * T( 1.0 / SAMPLE_FREQ ); // mod
	m_tFX = m_tF * m_tF;
	m_tFB = tRes * T(4.0) * ( T(1.0) - T(0.15) * m_tFX ); // mod: * T(4.0)
	m_tFX *= m_tFX;
	m_tFX *= T(0.35013);
	m_tOneMinusF = T(1.0) - m_tF;
}

////////////////////////////////////////////////////////////////////////////////
// COpTkFltEq3
////////////////////////////////////////////////////////////////////////////////

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltEq3<T, SAMPLE_FREQ>::COpTkFltEq3()
{
	Reset();
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltEq3<T, SAMPLE_FREQ>::COpTkFltEq3( const COpTkFltEq3<T, SAMPLE_FREQ> &roO )
{
	*this = roO;
}

template<class T, const unsigned int SAMPLE_FREQ>
COpTkFltEq3<T, SAMPLE_FREQ> & COpTkFltEq3<T, SAMPLE_FREQ>::operator =( const COpTkFltEq3<T, SAMPLE_FREQ> &roO )
{
	m_tLf = roO.m_tLf;
	m_tF1p0 = roO.m_tF1p0;
	m_tF1p1 = roO.m_tF1p1;
	m_tF1p2 = roO.m_tF1p2;
	m_tF1p3 = roO.m_tF1p3;

	m_tHf = roO.m_tHf;
	m_tF2p0 = roO.m_tF2p0;
	m_tF2p1 = roO.m_tF2p1;
	m_tF2p2 = roO.m_tF2p2;
	m_tF2p3 = roO.m_tF2p3;

	m_tSample0 = roO.m_tSample0;
	m_tSample1 = roO.m_tSample1;
	m_tSample2 = roO.m_tSample2;

	m_tLg = roO.m_tLg;
	m_tMg = roO.m_tMg;
	m_tHg = roO.m_tHg;

	return *this;
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkFltEq3<T, SAMPLE_FREQ>::InitFreq( T tFreqLow, T tFreqHigh )
{
	// Calculate filter cutoff frequencies
	m_tLf = T(2.0) * sin( OP_TK_PI * ( tFreqLow / T(SAMPLE_FREQ) ) );
	m_tHg = T(2.0) * sin( OP_TK_PI * ( tFreqHigh / T(SAMPLE_FREQ) ) );
}

template<class T, const unsigned int SAMPLE_FREQ>
T COpTkFltEq3<T, SAMPLE_FREQ>::Proc( T tIn )
{
	static const T vsa_ = (1.0 / 4294967295.0); // Very small amount (Denormal Fix)
	T l, m, h; // Low / Mid / High - Sample Values

	// Filter #1 (lowpass)
	m_tF1p0 += m_tLf * ( tIn     - m_tF1p0 ) + vsa_;
	m_tF1p1 += m_tLf * ( m_tF1p0 - m_tF1p1 );
	m_tF1p2 += m_tLf * ( m_tF1p1 - m_tF1p2 );
	m_tF1p3 += m_tLf * ( m_tF1p2 - m_tF1p3 );

	l = m_tF1p3;

	// Filter #2 (highpass)
	m_tF2p0 += m_tHf * ( tIn     - m_tF2p0 ) + vsa_;
	m_tF2p1 += m_tHf * ( m_tF2p0 - m_tF2p1 );
	m_tF2p2 += m_tHf * ( m_tF2p1 - m_tF2p2 );
	m_tF2p3 += m_tHf * ( m_tF2p2 - m_tF2p3 );

	h = m_tSample2 - m_tF2p3;

	// Calculate midrange (signal - (low + high))
	m = m_tSample2 - (h + l);

	// Scale, Combine and store
	l *= m_tLg;
	m *= m_tMg;
	h *= m_tHg;

	// Shuffle history buffer
	m_tSample2 = m_tSample1;
	m_tSample1 = m_tSample0;
	m_tSample0 = tIn;

	// Return result
	return ( l + m + h );
}

template<class T, const unsigned int SAMPLE_FREQ>
void COpTkFltEq3<T, SAMPLE_FREQ>::Reset()
{
	// Filter #1 (Low band)
	m_tLf = T(0.0);       // Frequency
	m_tF1p0 = T(0.0);     // Poles ...
	m_tF1p1 = T(0.0);
	m_tF1p2 = T(0.0);
	m_tF1p3 = T(0.0);

	// Filter #2 (High band)
	m_tHf = T(0.0);       // Frequency
	m_tF2p0 = T(0.0);     // Poles ...
	m_tF2p1 = T(0.0);
	m_tF2p2 = T(0.0);
	m_tF2p3 = T(0.0);

	// Sample history buffer
	m_tSample0 = T(0.0);     // Sample data minus 1
	m_tSample1 = T(0.0);     //                   2
	m_tSample2 = T(0.0);     //                   3

	// Gain Controls
	m_tLg = T(0.0);       // low  gain
	m_tMg = T(0.0);       // mid  gain
	m_tHg = T(0.0);       // high gain
}


////////////////////////////////////////////////////////////////////////////////
// COpTkFltAA44100
////////////////////////////////////////////////////////////////////////////////

// FIR_Chebyshev_AntiAlias_Filter_0a
//
// FIR Lowpass Filter (Chebyshev Window)
// Order: 255
// Fs = 1411200 = 32 * 44100
// Fc = 11000
// Sidelobe atten.: 300 (wichtig!)
//
// 256 doubles (8 * 256 Bytes = 2 KB)
//
template<class T>
const T COpTkFltAA44100<T>::m_atFltCoeff256_[256] = {
  -4.17182082302e-017,-4.571660324284e-016,-2.419180340661e-015,-9.247196547526e-015,
  -2.912469922041e-014,-8.037646412274e-014,-2.011780293022e-013,-4.666410896006e-013,
  -1.017772648962e-012,-2.109000378358e-012,-4.183986724084e-012,-7.993549657207e-012,
  -1.477493160274e-011,-2.651870090419e-011,-4.635854049926e-011,-7.91303855619e-011,
  -1.321611017584e-010,-2.163633782301e-010,-3.477328745732e-010,-5.493658847328e-010,
  -8.541406176222e-010,-1.308232099101e-009,-1.975658736539e-009,-2.944086327233e-009,
  -4.332140705561e-009,-6.298500565617e-009,-9.053053691741e-009,-1.287039836849e-008,
  -1.810595158551e-008,-2.52148801661e-008,-3.477399236372e-008,-4.750660690292e-008,
  -6.431024395305e-008,-8.628674727027e-008,-1.147741371955e-007,-1.513790984548e-007,
  -1.980085129307e-007,-2.568978444995e-007,-3.306334606315e-007,-4.221651256779e-007,
  -5.348039184177e-007,-6.721997100819e-007,-8.382910920862e-007,-1.037219274303e-006,
  -1.273196001002e-006,-1.550313976584e-006,-1.872286689756e-006,-2.242102915164e-006,
  -2.661579608225e-006,-3.130795454651e-006,-3.647386064023e-006,-4.205680789319e-006,
  -4.79566050297e-006,-5.401715463216e-006,-6.001182766008e-006,-6.562643903238e-006,
  -7.043964743003e-006,-7.39006291239e-006,-7.530391189424e-006,-7.376130176432e-006,
  -6.817089291302e-006,-5.718322011442e-006,-3.916469343918e-006,-1.215854646631e-006,
  2.61563687708e-006,7.850849553394e-006,1.480776310871e-005,2.385394416593e-005,
  3.541096167158e-005,4.995867170453e-005,6.80392645767e-005,9.026095529415e-005,
  0.0001173011877208,0.0001499092136226,0.0001889079006236,0.0002351946184458,
  0.0002897410510762,0.0003535917841445,0.0004278615221811,0.0005137307998928,
  0.000612440065394,0.0007252820316312,0.0008535922150978, 0.000998737608299,
  0.001162103464103, 0.001345078205792, 0.001549036515825, 0.001775320698473,
  0.002025220455803, 0.002299951262141, 0.002600631568133, 0.002928259110767,
  0.003283686649078, 0.003667597485495, 0.004080481168721,  0.00452260980444,
  0.004994015423818, 0.005494468875724, 0.006023460715772, 0.006580184563001,
  0.007163523382508,  0.00777203912941, 0.008403966155793, 0.009057208738087,
  0.009729343027841,  0.01041762366488,  0.01111899521921,  0.01183010854812,
  0.01254734206872,  0.01326682785624,  0.01398448238526,  0.01469604163816,
  0.01539710021331,  0.01608315397771,  0.01674964572685,  0.01739201324041,
  0.01800573905837,  0.01858640124916,  0.01912972440251,  0.01963163005392,
  0.02008828573827,  0.02049615187597,  0.02085202571711,  0.02115308160705,
  0.02139690689052,  0.02158153283861,  0.02170546006417,  0.02176767798342,
  0.02176767798342,  0.02170546006417,  0.02158153283861,  0.02139690689052,
  0.02115308160705,  0.02085202571711,  0.02049615187597,  0.02008828573827,
  0.01963163005392,  0.01912972440251,  0.01858640124916,  0.01800573905837,
  0.01739201324041,  0.01674964572685,  0.01608315397771,  0.01539710021331,
  0.01469604163816,  0.01398448238526,  0.01326682785624,  0.01254734206872,
  0.01183010854812,  0.01111899521921,  0.01041762366488, 0.009729343027841,
  0.009057208738087, 0.008403966155793,  0.00777203912941, 0.007163523382508,
  0.006580184563001, 0.006023460715772, 0.005494468875724, 0.004994015423818,
  0.00452260980444, 0.004080481168721, 0.003667597485495, 0.003283686649078,
  0.002928259110767, 0.002600631568133, 0.002299951262141, 0.002025220455803,
  0.001775320698473, 0.001549036515825, 0.001345078205792, 0.001162103464103,
  0.000998737608299,0.0008535922150978,0.0007252820316312, 0.000612440065394,
  0.0005137307998928,0.0004278615221811,0.0003535917841445,0.0002897410510762,
  0.0002351946184458,0.0001889079006236,0.0001499092136226,0.0001173011877208,
  9.026095529415e-005,6.80392645767e-005,4.995867170453e-005,3.541096167158e-005,
  2.385394416593e-005,1.480776310871e-005,7.850849553394e-006,2.61563687708e-006,
  -1.215854646631e-006,-3.916469343918e-006,-5.718322011442e-006,-6.817089291302e-006,
  -7.376130176432e-006,-7.530391189424e-006,-7.39006291239e-006,-7.043964743003e-006,
  -6.562643903238e-006,-6.001182766008e-006,-5.401715463216e-006,-4.79566050297e-006,
  -4.205680789319e-006,-3.647386064023e-006,-3.130795454651e-006,-2.661579608225e-006,
  -2.242102915164e-006,-1.872286689756e-006,-1.550313976584e-006,-1.273196001002e-006,
  -1.037219274303e-006,-8.382910920862e-007,-6.721997100819e-007,-5.348039184177e-007,
  -4.221651256779e-007,-3.306334606315e-007,-2.568978444995e-007,-1.980085129307e-007,
  -1.513790984548e-007,-1.147741371955e-007,-8.628674727027e-008,-6.431024395305e-008,
  -4.750660690292e-008,-3.477399236372e-008,-2.52148801661e-008,-1.810595158551e-008,
  -1.287039836849e-008,-9.053053691741e-009,-6.298500565617e-009,-4.332140705561e-009,
  -2.944086327233e-009,-1.975658736539e-009,-1.308232099101e-009,-8.541406176222e-010,
  -5.493658847328e-010,-3.477328745732e-010,-2.163633782301e-010,-1.321611017584e-010,
  -7.91303855619e-011,-4.635854049926e-011,-2.651870090419e-011,-1.477493160274e-011,
  -7.993549657207e-012,-4.183986724084e-012,-2.109000378358e-012,-1.017772648962e-012,
  -4.666410896006e-013,-2.011780293022e-013,-8.037646412274e-014,-2.912469922041e-014,
  -9.247196547526e-015,-2.419180340661e-015,-4.571660324284e-016,-4.17182082302e-017
};

template<class T>
const T COpTkFltAA44100<T>::m_atFltCoeff128_[128] = {
  -7.54074496091e-016,-1.808481398342e-014,-1.624862795986e-013,-9.519399217858e-013,
  -4.309645580436e-012,-1.631686222925e-011,-5.401270594125e-011,-1.607506019534e-010,
  -4.383480835611e-010,-1.110078112591e-009,-2.63693738698e-009,-5.920723201007e-009,
  -1.264059430217e-008,-2.578291680742e-008,-5.043307025118e-008,-9.489527839417e-008,
  -1.721814559168e-007,-3.018473787479e-007,-5.120350907673e-007,-8.413824998666e-007,
  -1.340162521545e-006,-2.069596716566e-006,-3.097743563686e-006,-4.48969739194e-006,
  -6.289083032129e-006,-8.487057541322e-006,-1.097433492363e-005,-1.347127196735e-005,
  -1.543096650933e-005,-1.591081929809e-005,-1.340929508313e-005,-5.666859495564e-006,
  1.056662339096e-005,3.979136066111e-005,8.803195363517e-005, 0.000163108555151,
  0.0002748806426086,0.0004354334400554,0.0006591725112228,0.0009627896374945,
  0.001365063648933, 0.001886464133475, 0.002548534410894, 0.003373042968275,
  0.004380909401248, 0.005590930962185, 0.007018357731675, 0.008673386384816,
  0.01055966236014,  0.01267289564546,  0.01499970414764,  0.01751679883953,
  0.02019061531685,  0.02297747662509,  0.02582434279357,  0.02867016504491,
  0.03144781971686,   0.0340865519246,  0.03651481579966,  0.03866336079783,
  0.04046838582858,   0.0418745678832,  0.04283777144008,  0.04332725989921,
  0.04332725989921,  0.04283777144008,   0.0418745678832,  0.04046838582858,
  0.03866336079783,  0.03651481579966,   0.0340865519246,  0.03144781971686,
  0.02867016504491,  0.02582434279357,  0.02297747662509,  0.02019061531685,
  0.01751679883953,  0.01499970414764,  0.01267289564546,  0.01055966236014,
  0.008673386384816, 0.007018357731675, 0.005590930962185, 0.004380909401248,
  0.003373042968275, 0.002548534410894, 0.001886464133475, 0.001365063648933,
  0.0009627896374945,0.0006591725112228,0.0004354334400554,0.0002748806426086,
  0.000163108555151,8.803195363517e-005,3.979136066111e-005,1.056662339096e-005,
  -5.666859495564e-006,-1.340929508313e-005,-1.591081929809e-005,-1.543096650933e-005,
  -1.347127196735e-005,-1.097433492363e-005,-8.487057541322e-006,-6.289083032129e-006,
  -4.48969739194e-006,-3.097743563686e-006,-2.069596716566e-006,-1.340162521545e-006,
  -8.413824998666e-007,-5.120350907673e-007,-3.018473787479e-007,-1.721814559168e-007,
  -9.489527839417e-008,-5.043307025118e-008,-2.578291680742e-008,-1.264059430217e-008,
  -5.920723201007e-009,-2.63693738698e-009,-1.110078112591e-009,-4.383480835611e-010,
  -1.607506019534e-010,-5.401270594125e-011,-1.631686222925e-011,-4.309645580436e-012,
  -9.519399217858e-013,-1.624862795986e-013,-1.808481398342e-014,-7.54074496091e-016 };

template<class T>
const T COpTkFltAA44100<T>::m_atFltCoeff64_[64] = {
  -1.288644643524e-013,-5.84693620865e-012,-8.39863917049e-011,-7.176907930132e-010,
  -4.418001076683e-009,-2.14351945182e-008,-8.623050899666e-008,-2.967974861851e-007,
  -8.91858215673e-007,-2.369477214945e-006,-5.601845082332e-006,-1.178376833915e-005,
  -2.184242839883e-005,-3.464625284558e-005,-4.3070950234e-005,-2.690328802827e-005,
  5.803817933007e-005,0.0002940549729624,0.0008165542984751,   0.0018250263087,
  0.003583105636523, 0.006400272831922,  0.01058991519612,  0.01640387953311,
  0.02395165213146,  0.03312079586401,  0.04352111600991,  0.05447506884457,
  0.06506940936286,   0.0742687664053,  0.08107432070221,  0.08469554327974,
  0.08469554327974,  0.08107432070221,   0.0742687664053,  0.06506940936286,
  0.05447506884457,  0.04352111600991,  0.03312079586401,  0.02395165213146,
  0.01640387953311,  0.01058991519612, 0.006400272831922, 0.003583105636523,
  0.0018250263087,0.0008165542984751,0.0002940549729624,5.803817933007e-005,
  -2.690328802827e-005,-4.3070950234e-005,-3.464625284558e-005,-2.184242839883e-005,
  -1.178376833915e-005,-5.601845082332e-006,-2.369477214945e-006,-8.91858215673e-007,
  -2.967974861851e-007,-8.623050899666e-008,-2.14351945182e-008,-4.418001076683e-009,
  -7.176907930132e-010,-8.39863917049e-011,-5.84693620865e-012,-1.288644643524e-013
};

template<class T>
const T COpTkFltAA44100<T>::m_atFltCoeff32_[32] = {
  -4.173061425742e-010,-2.482548754684e-008,-4.048642734677e-007,-3.500570614929e-006,
  -1.931296318316e-005,-7.232438087597e-005,-0.0001774990615967,-0.0001923836078728,
  0.0005731796240611, 0.003934103420301,  0.01309093148961,  0.03170725467815,
  0.06139614560278,  0.09874606917894,    0.134458961563,   0.1565588051344,
  0.1565588051344,    0.134458961563,  0.09874606917894,  0.06139614560278,
  0.03170725467815,  0.01309093148961, 0.003934103420301,0.0005731796240611,
  -0.0001923836078728,-0.0001774990615967,-7.232438087597e-005,-1.931296318316e-005,
  -3.500570614929e-006,-4.048642734677e-007,-2.482548754684e-008,-4.173061425742e-010
};

template<class T>
COpTkFltAA44100<T>::COpTkFltAA44100()
{
	SetFilter( 0 );
}

template<class T>
COpTkFltAA44100<T>::COpTkFltAA44100( const COpTkFltAA44100<T> &roO )
{
	*this = roO;
}

template<class T>
COpTkFltAA44100<T>::~COpTkFltAA44100()
{}

template<class T>
COpTkFltAA44100<T> & COpTkFltAA44100<T>::operator =( const COpTkFltAA44100 &roO )
{
	m_uiOverSample = roO.m_uiOverSample;
	m_uiFilterSize = roO.m_uiFilterSize;
	m_uiBufferInPos = roO.m_uiBufferInPos;
	OP_TK_MEM_COPY( m_atBufferIn, roO.m_atBufferIn, sizeof(m_atBufferIn) );
	OP_TK_MEM_COPY( m_atFltCoeff, roO.m_atFltCoeff, sizeof(m_atFltCoeff) );
	return *this;
}

template<class T>
double COpTkFltAA44100<T>::Filter()
{
	T tVal = 0.0;
	T *ptB = m_atBufferIn;
	T *ptC = m_atFltCoeff + m_uiFilterSize - m_uiBufferInPos;
	unsigned int i = 0;
	do
	{
		tVal += *ptC++ * *ptB++;
		++i;
	}
	while( i < m_uiFilterSize );

	/*
	// Unoptimiertes FIR-Filter.
	T tVal = 0.0;
	unsigned int j = m_uiBufferInPos;
	unsigned int i = 0;
	do
	{
		dVal += m_atFltCoeff_[i] * m_atBufferIn[j];
		if( j == 0 )
			j = m_uiFilterSize;
		--j;
		++i;
	}
	while( i < m_uiFilterSize );
	*/

	return tVal;
}

template<class T>
void COpTkFltAA44100<T>::ResetFilter()
{
	m_uiBufferInPos = 0; // Wichtig um den Puffer synchron zu den Resample-Chunks zu füllen!
	OP_MEM_ZERO( m_atBufferIn, sizeof(T) * m_uiFilterSize );
}

template<class T>
unsigned int COpTkFltAA44100<T>::SetFilter( unsigned int uiFilter )
{
	static const unsigned int auiOverSample_[] = { 1, 4, 8, 16, 32 };
	static const unsigned int auiFilterSize_[] = { 1, 32, 64, 128, 256 };
	static const T * aatFltCoeff_[] =
		{ 0, m_atFltCoeff32_, m_atFltCoeff64_, m_atFltCoeff128_, m_atFltCoeff256_ };

	const unsigned int uiFlt = uiFilter % 5;
	const T * atFltCoeff = aatFltCoeff_[uiFlt];
	m_uiFilterSize = auiFilterSize_[uiFlt];
	m_uiOverSample = auiOverSample_[uiFlt];

	// Koeffiziente-Array verdoppeln, so dass im Filter-Loop
	// keine Abfrage Index-Prüfung gemacht werden muss
	OP_MEM_ZERO( m_atFltCoeff, m_uiFilterSize * sizeof(T) );
	if( m_uiFilterSize == 1 )
	{
		// Funktioniert in Filter() so m_uiFilterSize=1/m_uiOverSample=1,
		// mit diesen Koeffizienten. - Möglich ist, bei diesen Bedingungen
		// gar nicht erst die Filter()-Methode aufzurufen!
		m_atFltCoeff[0] = m_atFltCoeff[1] = T(1.0);
	}
	else
	{
		for( unsigned int i=0; i<m_uiFilterSize; ++i )
			m_atFltCoeff[i] = m_atFltCoeff[i+m_uiFilterSize] = atFltCoeff[i];
	}
	ResetFilter();
	return uiFlt;
}
