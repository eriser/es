// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpCompress.h

#ifndef _OP_COMPRESS_PRO_H_
#define _OP_COMPRESS_PRO_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_COMPRESS_EX

//#include "SimpleSource.h" // tmp

// Extended Compressor
////////////////////////////////////////////////////////////////////////////////
class COpCompressEx : public COp
{
private:
	// ref: Original Sources:
	//
	// Simple Source 
	// 2006, ChunkWare Music Software, OPEN-SOURCE
	//

	static const double DC_OFFSET;
	static const double LOG_2_DB;
	static const double DB_2_LOG;

	static inline double lin2dB( double lin ); // linear -> dB conversion
	static inline double dB2lin( double dB ); // dB -> linear conversion
	
	// Envelope Detector
	////////////////////////////////////////////////////////////////////////////////
	class CEnvelopeDetector
	{
	public:
		CEnvelopeDetector(
			double ms = 1.0
			//, double sampleRate = 44100.0
			);
		~CEnvelopeDetector() {}

		// time constant
		void   setTc( double ms );
		inline double getTc( void ) const { return ms_; }

		// sample rate
		//void   setSampleRate( double sampleRate );
		//inline double getSampleRate( void ) const { return sampleRate_; }

		// runtime function
		inline void run( double in, double &state ) {
			state = in + coef_ * ( state - in );
		}

	protected:
		//double sampleRate_; // sample rate
		double ms_; // time constant in ms
		double coef_; // runtime coefficient
		void setCoef( void ); // coef calculation

	};
	
	// Attack/Release Envelope
	////////////////////////////////////////////////////////////////////////////////
	class CAttRelEnvelope
	{
	public:
		CAttRelEnvelope(
			double att_ms = 10.0
			, double rel_ms = 100.0
			//, double sampleRate = 44100.0
			);
		~CAttRelEnvelope() {}

		// attack time constant
		void   setAttack( double ms );
		inline double getAttack( void ) const { return att_.getTc(); }

		// release time constant
		void   setRelease( double ms );
		inline double getRelease( void ) const { return rel_.getTc(); }

		// sample rate dependencies
		//void   setSampleRate( double sampleRate );
		//inline double getSampleRate( void ) const { return att_.getSampleRate(); }

		// runtime function
		inline void run( double in, double &state ) {

			/* assumes that:
			* positive delta = attack
			* negative delta = release
			* good for linear & log values
			*/

			if ( in > state )
				att_.run( in, state );	// attack
			else
				rel_.run( in, state );	// release
		}
	private:
		CEnvelopeDetector att_;
		CEnvelopeDetector rel_;
	};
	
	// Simple Compressor
	////////////////////////////////////////////////////////////////////////////////
	class CSimpleComp : public CAttRelEnvelope
	{
	public:
		CSimpleComp();
		~CSimpleComp() {}

		// parameters
		void setThresh( double dB );
		void setThreshLinear( double dThreshold );
		void setRatio( double dB );

		double getThresh( void ) const { return threshdB_; }
		double getRatio( void )  const { return ratio_; }

		// runtime
		void initRuntime( void );			// call before runtime (in resume())
		void process( double &in1, double &in2 );	// compressor runtime process
		void process( double &in1, double &in2, double keyLinked );	// with stereo-linked key in
		
		inline double calcGainReduction( double keyLinked ); // mod

		// mod
		double getEnv() const
		{
			return 1.0 - dB2lin( -envdB_ - DC_OFFSET ); // ???
		}
		
	public:
		// transfer function
		double threshdB_;		// threshold (dB)
	protected:
		double ratio_;			// ratio (compression: < 1 ; expansion: > 1)
	public:
		// runtime variables
		double envdB_;			// over-threshold envelope (dB)

	};
	
	// Simple Compressor with RMS Detection
	////////////////////////////////////////////////////////////////////////////////
	class CSimpleCompRms : public CSimpleComp
	{
	public:
		CSimpleCompRms();
		~CSimpleCompRms() {}

		// sample rate
		//void setSampleRate( double sampleRate );

		// RMS window
		void setWindow( double ms );
		double getWindow( void ) const { return ave_.getTc(); }

		// runtime process
		void initRuntime( void );			// call before runtime (in resume())
		void process( double &in1, double &in2 );	// compressor runtime process
		
		inline double calcGainReductionRMS( double keyLinked ); // mod

		// mod
		double getEnergy() const
		{
			return ( m_bEnableRMS ? aveOfSqrs_ : 0.0 );
		}
	public:
		double aveOfSqrs_; // average of squares
	private:
		CEnvelopeDetector ave_; // averager
		bool m_bEnableRMS; // mod

	};

public:
	COpCompressEx();
	virtual ~COpCompressEx();
protected:
	void Init();
public:	
	virtual void Proc();
	virtual void Reset();
	
	OP_GENERIC_COPY_CTOR_DEC( COpCompressEx )
	OP_GENERIC_METHODS_DEC( COpCompressEx )

protected:
	CSimpleCompRms m_oCompressor;
	double m_dThreshold, m_dRatio;
	double m_dAtt, m_dRel;
	double m_dRMSWindow;
};

#include "OpCompressEx.inline.h"

#endif // OP_INC_OP_COMPRESS_EX
#endif // _OP_COMPRESS_PRO_H_
