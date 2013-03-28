// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCompressEx.inline.h

// ref: Original Sources:
//
// Simple Source 
// 2006, ChunkWare Music Software, OPEN-SOURCE
//

////////////////////////////////////////////////////////////////////////////////
// COpCompressEx
////////////////////////////////////////////////////////////////////////////////

inline double COpCompressEx::lin2dB( double lin )
{
	return log( lin ) * LOG_2_DB;
}

// dB -> linear conversion
inline double COpCompressEx::dB2lin( double dB )
{
	return exp( dB * DB_2_LOG );
}


////////////////////////////////////////////////////////////////////////////////
// COpCompressEx::CSimpleComp
////////////////////////////////////////////////////////////////////////////////

inline void COpCompressEx::CSimpleComp::process( double &in1, double &in2 )
{
	// create sidechain

	double rect1 = fabs( in1 );	// rectify input
	double rect2 = fabs( in2 );

	/* if desired, one could use another CEnvelopeDetector to smooth
		* the rectified signal.
		*/

	double link = OP_MAX( rect1, rect2 );	// link channels with greater of 2 // mod

	process( in1, in2, link );	// rest of process
}

inline double COpCompressEx::CSimpleComp::calcGainReduction( double keyLinked )
{
	keyLinked = fabs( keyLinked );		// rectify (just in case)

	// convert key to dB
	keyLinked += DC_OFFSET;				// add DC offset to avoid log( 0 )
	double keydB = lin2dB( keyLinked );	// convert linear -> dB

	// threshold
	double overdB = keydB - threshdB_;	// delta over threshold
	if ( overdB < 0.0 )
		overdB = 0.0;

	// attack/release

	overdB += DC_OFFSET;					// add DC offset to avoid denormal
	CAttRelEnvelope::run( overdB, envdB_ );	// run attack/release envelope
	overdB = envdB_ - DC_OFFSET;			// subtract DC offset

	/* REGARDING THE DC OFFSET: In this case, since the offset is added before 
		* the attack/release processes, the envelope will never fall below the offset,
		* thereby avoiding denormals. However, to prevent the offset from causing
		* constant gain reduction, we must subtract it from the envelope, yielding
		* a minimum value of 0dB.
		*/

	// transfer function
	double gr = overdB * ( ratio_ - 1.0 );	// gain reduction (dB)
	gr = dB2lin( gr );						// convert dB -> linear

	// output gain
	//in1 *= gr;	// apply gain reduction to input
	//in2 *= gr;

	return gr;
}

inline void COpCompressEx::CSimpleComp::process( double &in1, double &in2, double keyLinked )
{
	const double gr = calcGainReduction( keyLinked );

	// output gain
	in1 *= gr;	// apply gain reduction to input
	in2 *= gr;
}


////////////////////////////////////////////////////////////////////////////////
// COpCompressEx::CSimpleCompRms
////////////////////////////////////////////////////////////////////////////////

inline double COpCompressEx::CSimpleCompRms::calcGainReductionRMS( double keyLinked )
{
	// mod
	if( m_bEnableRMS )
	{
		// see blow...
		double in = keyLinked * keyLinked;
		in += DC_OFFSET;
		ave_.run( in, aveOfSqrs_ );
		double rms = sqrt( aveOfSqrs_ );	// rms (sort of ...)
		return calcGainReduction( rms );
	}
	else
	{
		return CSimpleComp::calcGainReduction( keyLinked );
	}
}

inline void COpCompressEx::CSimpleCompRms::process( double &in1, double &in2 )
{
	// mod
	if( m_bEnableRMS )
	{
		// create sidechain
		double inSq1 = in1 * in1;		// square input
		double inSq2 = in2 * in2;

		double sum = inSq1 + inSq2;		// power summing
		sum += DC_OFFSET;			// DC offset, to prevent denormal
		ave_.run( sum, aveOfSqrs_ );		// average of squares
		double rms = sqrt( aveOfSqrs_ );	// rms (sort of ...)

		/* REGARDING THE RMS AVERAGER: Ok, so this isn't a REAL RMS
			* calculation. A true RMS is an FIR moving average. This
			* approximation is a 1-pole IIR. Nonetheless, in practice,
			* and in the interest of simplicity, this method will suffice,
			* giving comparable results.
		*/

		CSimpleComp::process( in1, in2, rms );	// rest of process
	}
	else
	{
		CSimpleComp::process( in1, in2 );
	}
}
