// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OrgOp.cpp

#include "OrgOp.h"
#include <stdio.h>
#include <stdlib.h>

// Alphabetisch sortierte Liste aller Op-Headers.
#include "../Op/OpAccum.h"
#include "../Op/OpAdd.h"
#include "../Op/OpAMU.h"
#include "../Op/OpBit.h"
#include "../Op/OpBitSplit.h"
#include "../Op/OpBuffer.h"
#include "../Op/OpChorus.h"
#include "../Op/OpClip.h"
#include "../Op/OpCmp.h"
#include "../Op/OpCombiner.h"
#include "../Op/OpCompress.h"
#include "../Op/OpCompressEx.h"
#include "../Op/OpConverter.h"
#include "../Op/OpDelay.h"
#include "../Op/OpDemux.h"
#include "../Op/OpDetect.h"
#include "../Op/OpDiff.h"
#include "../Op/OpDistort.h"
#include "../Op/OpDiv.h"
#include "../Op/OpEnv1Bzr.h"
#include "../Op/OpEnv1Exp.h"
#include "../Op/OpEnv1Lin.h"
#include "../Op/OpEnv1Sin.h"
#include "../Op/OpEnv2Bzr.h"
#include "../Op/OpEnv2Exp.h"
#include "../Op/OpEnv2Lin.h"
#include "../Op/OpEnv2Sin.h"
#include "../Op/OpEnvAdsr.h"
#include "../Op/OpEnvGlider.h"
#include "../Op/OpEventTable.h"
#include "../Op/OpFader.h"
#include "../Op/OpFileWavIn.h"
#include "../Op/OpFileWavOut.h"
#include "../Op/OpFlanger.h"
#include "../Op/OpFltAllpass.h"
#include "../Op/OpFltAllpass1.h"
#include "../Op/OpFltAllpass2.h"
#include "../Op/OpFltBiquad.h"
#include "../Op/OpFltComb.h"
#include "../Op/OpFltEq3.h"
#include "../Op/OpFltFBack.h"
#include "../Op/OpFltFForw.h"
#include "../Op/OpFltFFT.h"
#include "../Op/OpFltHighLow.h"
#include "../Op/OpFltLow0.h"
#include "../Op/OpFltMoog.h"
#include "../Op/OpFltRes0.h"
#include "../Op/OpFltTB303.h"
#include "../Op/OpGate.h"
#include "../Op/OpIf.h"
#include "../Op/OpInter.h"
#include "../Op/OpLadder.h"
#include "../Op/OpLogic.h"
#include "../Op/OpLoop.h"
#include "../Op/OpMAD.h"
#include "../Op/OpMathArcus.h"
#include "../Op/OpMathExp.h"
#include "../Op/OpMathFmod.h"
#include "../Op/OpMathLog.h"
#include "../Op/OpMathModf.h"
#include "../Op/OpMathNum.h"
#include "../Op/OpMathPow.h"
#include "../Op/OpMathSqrt.h"
#include "../Op/OpMathTrigo.h"
#include "../Op/OpMidiDevIn.h"
#include "../Op/OpMidiDevOut.h"
#include "../Op/OpMidiFileIn.h"
#include "../Op/OpMidiIn.h"
#include "../Op/OpMidiInKey.h"
#include "../Op/OpMidiOut.h"
#include "../Op/OpMix.h"
#include "../Op/OpMul.h"
#include "../Op/OpNop.h"
#include "../Op/OpOscApprox.h"
#include "../Op/OpOscCircle.h"
#include "../Op/OpOscLUT.h"
#include "../Op/OpOscPls.h"
#include "../Op/OpOscSaw.h"
#include "../Op/OpOscSin.h"
#include "../Op/OpOscTri.h"
#include "../Op/OpOscVar.h"
#include "../Op/OpPercDrum.h"
#include "../Op/OpPhaser.h"
#include "../Op/OpReducer.h"
#include "../Op/OpRetrigger.h"
#include "../Op/OpReverb.h"
#include "../Op/OpReverbAL.h"
#include "../Op/OpRouter.h"
#include "../Op/OpSampler.h"
#include "../Op/OpScale.h"
#include "../Op/OpSelector.h"
#include "../Op/OpSeqAAS.h"
#include "../Op/OpSeqCell1D.h"
#include "../Op/OpSeqColl1D.h"
#include "../Op/OpSeqFibo.h"
#include "../Op/OpSeqLog.h"
#include "../Op/OpSeqMandel.h"
#include "../Op/OpSeqPrime.h"
#include "../Op/OpSeqRand.h"
#include "../Op/OpSeqReca.h"
#include "../Op/OpSH.h"
#include "../Op/OpShaper.h"
#include "../Op/OpStep.h"
#include "../Op/OpSto.h"
#include "../Op/OpSub.h"
#include "../Op/OpSwitch.h"
#include "../Op/OpSys.h"
#include "../Op/OpTime.h"
#include "../Op/OpTransFFT.h"
#include "../Op/OpTrigger.h"
#include "../Op/OpValue.h"

////////////////////////////////////////////////////////////////////////////////
// COrgOp
////////////////////////////////////////////////////////////////////////////////

COp * COrgOp::m_poOpRootL_ = 0;
COp * COrgOp::m_poOpRootR_ = 0;

#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )
bool COrgOp::m_bEnableCache_ = false;
unsigned int COrgOp::m_uiCachePos_ = 0;
double COrgOp::m_adCache_[ORG_OP_CACHE];
#endif // VERSION_RUNTIME // VERSION_LIB

COrgOp::COrgOp()
{
	// Hier ist der Zentrale Ort, wo alle verfügbaren Op-Klassen registriert sind.
	////////////////////////////////////////////////////////////////////////////////

#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LITE )

	ORG_OP_REGISTER_CLASS( Accum );
	ORG_OP_REGISTER_CLASS( Add );
	ORG_OP_REGISTER_CLASS( AMU );
	ORG_OP_REGISTER_CLASS( Bit );
	ORG_OP_REGISTER_CLASS( BitSplit );
	ORG_OP_REGISTER_CLASS( Buffer );
	ORG_OP_REGISTER_CLASS( Chorus );
	ORG_OP_REGISTER_CLASS( Clip );
	ORG_OP_REGISTER_CLASS( Cmp );
	ORG_OP_REGISTER_CLASS( Combiner );
	ORG_OP_REGISTER_CLASS( Compress );
	ORG_OP_REGISTER_CLASS( CompressEx );
	ORG_OP_REGISTER_CLASS( Converter );
	ORG_OP_REGISTER_CLASS( Delay );
	ORG_OP_REGISTER_CLASS( Demux );
	ORG_OP_REGISTER_CLASS( Detect );
	ORG_OP_REGISTER_CLASS( Diff );
	ORG_OP_REGISTER_CLASS( Distort );
	ORG_OP_REGISTER_CLASS( Div );
	ORG_OP_REGISTER_CLASS( Env1Bzr );
	ORG_OP_REGISTER_CLASS( Env1Exp );
	ORG_OP_REGISTER_CLASS( Env1Lin );
	ORG_OP_REGISTER_CLASS( Env1Sin );
	ORG_OP_REGISTER_CLASS( Env2Bzr );
	ORG_OP_REGISTER_CLASS( Env2Exp );
	ORG_OP_REGISTER_CLASS( Env2Lin );
	ORG_OP_REGISTER_CLASS( Env2Sin );
	ORG_OP_REGISTER_CLASS( EnvAdsr );
	ORG_OP_REGISTER_CLASS( EnvGlider );
	ORG_OP_REGISTER_CLASS( EventTable );
	ORG_OP_REGISTER_CLASS( Fader );
	ORG_OP_REGISTER_CLASS( FileWavIn );
	ORG_OP_REGISTER_CLASS( FileWavOut );
	ORG_OP_REGISTER_CLASS( Flanger );
	ORG_OP_REGISTER_CLASS( FltAllpass );
	ORG_OP_REGISTER_CLASS( FltAllpass1 );
	ORG_OP_REGISTER_CLASS( FltAllpass2 );
	ORG_OP_REGISTER_CLASS( FltBiquad );
	ORG_OP_REGISTER_CLASS( FltComb );
	ORG_OP_REGISTER_CLASS( FltEq3 );
	ORG_OP_REGISTER_CLASS( FltFBack );
	ORG_OP_REGISTER_CLASS( FltFForw );
	ORG_OP_REGISTER_CLASS( FltFFT );
	ORG_OP_REGISTER_CLASS( FltHighLow );
	ORG_OP_REGISTER_CLASS( FltLow0 );
	ORG_OP_REGISTER_CLASS( FltMoog );
	ORG_OP_REGISTER_CLASS( FltRes0 );
	ORG_OP_REGISTER_CLASS( FltTB303 );
	ORG_OP_REGISTER_CLASS( Gate );
	ORG_OP_REGISTER_CLASS( If );
	ORG_OP_REGISTER_CLASS( Inter );
	ORG_OP_REGISTER_CLASS( Ladder );
	ORG_OP_REGISTER_CLASS( Logic );
	ORG_OP_REGISTER_CLASS( Loop );
	ORG_OP_REGISTER_CLASS( MAD );
	ORG_OP_REGISTER_CLASS( MathArcus );
	ORG_OP_REGISTER_CLASS( MathExp );
	ORG_OP_REGISTER_CLASS( MathFmod );
	ORG_OP_REGISTER_CLASS( MathLog );
	ORG_OP_REGISTER_CLASS( MathModf );
	ORG_OP_REGISTER_CLASS( MathNum );
	ORG_OP_REGISTER_CLASS( MathPow );
	ORG_OP_REGISTER_CLASS( MathSqrt );
	ORG_OP_REGISTER_CLASS( MathTrigo );
#ifdef OP_USE_MIDI
	ORG_OP_REGISTER_CLASS( MidiDevIn );
	ORG_OP_REGISTER_CLASS( MidiDevOut );
#endif // OP_USE_MIDI
	ORG_OP_REGISTER_CLASS( MidiFileIn );
	ORG_OP_REGISTER_CLASS( MidiIn );
	ORG_OP_REGISTER_CLASS( MidiInKey );
#ifdef OP_USE_MIDI
	ORG_OP_REGISTER_CLASS( MidiOut );
#endif // OP_USE_MIDI
	ORG_OP_REGISTER_CLASS( Mix );
	ORG_OP_REGISTER_CLASS( Mul );
	ORG_OP_REGISTER_CLASS( Nop );
	ORG_OP_REGISTER_CLASS( OscApprox );
	ORG_OP_REGISTER_CLASS( OscCircle );
	ORG_OP_REGISTER_CLASS( OscLUT );
	ORG_OP_REGISTER_CLASS( OscPls );
	ORG_OP_REGISTER_CLASS( OscSaw );
	ORG_OP_REGISTER_CLASS( OscSin );
	ORG_OP_REGISTER_CLASS( OscTri );
	ORG_OP_REGISTER_CLASS( OscVar );
	ORG_OP_REGISTER_CLASS( PercDrum );
	ORG_OP_REGISTER_CLASS( Phaser );
	ORG_OP_REGISTER_CLASS( Reducer );
	ORG_OP_REGISTER_CLASS( Retrigger );
	ORG_OP_REGISTER_CLASS( Reverb );
	ORG_OP_REGISTER_CLASS( ReverbAL );
	ORG_OP_REGISTER_CLASS( Router );
	ORG_OP_REGISTER_CLASS( Sampler );
	ORG_OP_REGISTER_CLASS( Scale );
	ORG_OP_REGISTER_CLASS( Selector );
	ORG_OP_REGISTER_CLASS( SeqAAS );
	ORG_OP_REGISTER_CLASS( SeqCell1D );
	ORG_OP_REGISTER_CLASS( SeqColl1D );
	ORG_OP_REGISTER_CLASS( SeqFibo );
	ORG_OP_REGISTER_CLASS( SeqLog );
	ORG_OP_REGISTER_CLASS( SeqMandel );
	ORG_OP_REGISTER_CLASS( SeqPrime );
	ORG_OP_REGISTER_CLASS( SeqRand );
	ORG_OP_REGISTER_CLASS( SeqReca );
	ORG_OP_REGISTER_CLASS( SH );
	ORG_OP_REGISTER_CLASS( Shaper );
	ORG_OP_REGISTER_CLASS( Step );
	ORG_OP_REGISTER_CLASS( Sto );
	ORG_OP_REGISTER_CLASS( Sub );
	ORG_OP_REGISTER_CLASS( Switch );
	ORG_OP_REGISTER_CLASS( Sys );
	ORG_OP_REGISTER_CLASS( Time );
	ORG_OP_REGISTER_CLASS( TransFFT );
	ORG_OP_REGISTER_CLASS( Trigger );
	ORG_OP_REGISTER_CLASS( Value );
	////////////////////////////////////////////////////////////////////////////////

#if !defined( VERSION_LIB )
	OP_MEM_ZERO( m_adCache_, sizeof(m_adCache_) );
	m_uiCachePos_ = 0;
#endif // VERSION_LIB

#else // VERSION_RUNTIME // VERSION_LITE

#include RUNTIME_FILE_NAME_REG // ... ptch.exe

#endif // VERSION_RUNTIME // VERSION_LITE

#undef ORG_OP_REGISTER_CLASS
}

COrgOp::~COrgOp()
{
	Clear();
}

COp * COrgOp::CreateShort( const char *pcClassNameShort )
{
	for( unsigned int i=0; i<m_oArrClassName.GetSize(); ++i )
	{
		if( !strcmp( m_oArrClassName[i], pcClassNameShort ) )
		{
			COp *poOp = ( m_oArrClassCreate[i] )();
			return poOp;
		}
	}
	return 0;
}

COp * COrgOp::Create( const char *pcClassName )
{
	CStr oTmp = CStr(pcClassName).GetSub( 3, (unsigned int)strlen(pcClassName)-3 );
	return CreateShort( oTmp );
}

bool COrgOp::ValidateAll()
{
	COp *poOp;
	bool bRet = true;
	m_oLstOp.MoveToFront();
	while( m_oLstOp.GetNext( &poOp ) )
	{
		if( poOp )
		{
			poOp->Validate();
			if( !poOp->IsValid() )
				bRet = false;
		}
		else
			bRet = false;
	}
	return bRet;
}

void COrgOp::UpdateAll()
{
	COp *poOp;
	m_oLstOp.MoveToFront();
	while( m_oLstOp.GetNext( &poOp ) )
	{
		if( poOp )
		{
			poOp->Update();
		}
	}
}

void COrgOp::ResetAll()
{
	COp *poOp;
	m_oLstOp.MoveToFront();
	while( m_oLstOp.GetNext( &poOp ) )
	{
		if( poOp )
		{
			poOp->Reset();
		}
	}
	COp::m_uiSignal_ = 0;
}

#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )
const char * COrgOp::GetNameClass( unsigned int uiClassID )
{
	m_oStrTmp = CStr("COp") + CStr( GetNameClassShort( uiClassID ) ); // ACHTUNG!
	return m_oStrTmp;
}

const char * COrgOp::GetNameClassShort( unsigned int uiClassID )
{
	if( uiClassID < m_oArrClassName.GetSize() )
	{
		return m_oArrClassName[uiClassID];
	}
	return 0;
}

bool COrgOp::Insert( COp *poOp )
{
	if( poOp )
	{
		m_oLstOp.Append( poOp );
		return true;
	}
	return false;
}

bool COrgOp::Delete( COp *poOp )
{
	bool bRet = false;
	if( poOp )
	{
		bRet = ( m_oLstOp.RemoveEach( poOp ) != 0 );
		delete poOp;
	}
	return bRet;
}

unsigned int COrgOp::GetCountOp()
{
	return m_oArrClassName.GetSize();
}

COp * COrgOp::GetOp( unsigned int uiID )
{
	COp *poOp;
	m_oLstOp.MoveToFront();
	while( m_oLstOp.GetNext( &poOp ) )
	{
		if( poOp->GetID() == uiID )
			return poOp;
	}
	return 0;
}

bool COrgOp::SaveAll( CStr &roDst )
{
	COp *poOp;

	Encode_Op( roDst, m_poOpRootL_ );
	Encode_Op( roDst, m_poOpRootR_ );

 	m_oLstOp.MoveToFront();
	while( m_oLstOp.GetNext( &poOp ) )
	{
		if( poOp && poOp != m_poOpRootL_ && poOp != m_poOpRootR_ )
		{
			Encode_Op( roDst, poOp );
		}
	}

	return true;
}

bool COrgOp::Save( CStr &roDst, COp *poOp )
{
	return Encode_Op( roDst, poOp );
}

bool COrgOp::Save( CStr &roDst, COp **apoOp, unsigned int uiSize )
{
	CList<COp::CLink *> m_oLstLinkBak;
	CArray<COp *> m_oArrOpBak;

	unsigned int i = uiSize;
	while( i )
	{
		--i;
		COp *poOp = apoOp[i];
		if( !poOp )
			continue;

		const unsigned int uiCountIn = poOp->GetCountIn();
		unsigned int j = 0;
		while( j < uiCountIn )
		{
			COp::CLink *poLink = &poOp->In( j );
			COp *poOpIn = poLink->GetOp();
			if( poOpIn )
			{
				unsigned int k = 0;
				while( k < uiSize )
				{
					if( k != i )
					{
						if( poOpIn == apoOp[k] )
							break;
					}
					++k;
				}
				// Ist Input-Operator nicht in der Save-Liste enthalten,
				// wird dieser temporär deaktiviert.
				if( k == uiSize )
				{
					m_oLstLinkBak.Append( poLink );
					m_oArrOpBak.Append( poOpIn );
					poLink->SetOp( 0 );
				}
			}
			++j;
		}
		if( !Save( roDst, poOp ) )
			return false;
	}

	// Alle modifizierten Links zurücksetzen.
	i = 0;
	COp::CLink *poLink;
	m_oLstLinkBak.MoveToFront();
	while( m_oLstLinkBak.GetNext( &poLink ) )
	{
		poLink->SetOp( m_oArrOpBak[i] );
		++i;
	}
	return true;
}

#endif // RUNTIME_VERSION // VERSION_LIB

bool COrgOp::LoadAll( CStr &roSrc )
{
	Clear();
	Load( roSrc, 0 );

	if( m_oLstOp.GetSize() < 2 )
	{
		Clear();
		ORG_OP_LOG( "Fehler: Weniger als 2 Operatoren eingelesen!\n" );
		return false;
	}

	m_poOpRootL_ = m_oLstOp[0];
	m_poOpRootR_ = m_oLstOp[1];
	if( m_poOpRootL_ == 0 || m_poOpRootR_ == 0 )
		return false;

	UpdateAll();
	ValidateAll();
	return true;
}


bool COrgOp::Load( CStr &roSrc, unsigned int uiIDOffset )
{
	CStr oStrLine;
	CStr oStrToken;
	CList<COp *> oLstPatch;
	unsigned int uiIDIn = 0;
	unsigned int uiIdxIn = 0;
	unsigned int uiCountIn = 0;
	unsigned int uiPortIn = 0;
	unsigned int uiIndexIn = 0;
	unsigned int uiCountOut = 0;
	unsigned int uiPortOut = 0;
	unsigned int uiInteralInt = 0;
	unsigned int uiInteralUInt = 0;
	unsigned int uiInteralFlt = 0;
	unsigned int i = 0;
	unsigned int uiMaxIPortIn = 0;
	unsigned int uiMaxIPortOut = 0;
	unsigned int uiMaxIInternalInt = 0;
	unsigned int uiMaxIInternalUInt = 0;
	unsigned int uiMaxIInternalFlt = 0;
	COp *poOp = 0;
	
	// <mod date="2010-12-07">
	CList<CArray<unsigned int> *> oLstArrID;
	CArray<unsigned int> * poArrID = 0;
	// </mod>
	
	// Alle Operatoren einlesen, erstellen und pseudo-patchen.
	while( 1 )
	{
		if( !Decode_GetNextLine( oStrLine, roSrc ) )
			break;
		i = 0;
		while( 1 )
		{
			if( !Decode_GetNextToken( oStrToken, oStrLine ) )
				break;

			if( !oStrToken.GetSize() )
			{
				if( i > 1 ) // Komprimierung :: -> :0:
					oStrToken = '0';
				else if( i == 0 )
					ORG_OP_LOG( "Fehler: Klassen-Name nicht vorhanden." );
			}

			// class
			if( i == 0 )
			{
				poOp = Create( oStrToken );
				if( poOp == 0 )
				{
					ORG_OP_LOG( "Fehler: Unbekannte Klasse: %s\n", oStrToken.GetData() );
					return false;
				}
				oLstPatch.Append( poOp );
			}
			// instance
			else if( i == 1 )
			{
#ifdef OP_USE_RUNTIME_INFO
				poOp->SetNameInstance( oStrToken );
#endif // OP_USE_RUNTIME_INFO
			}
			// id
			else if( i == 2 )
			{
				unsigned int uiID;
				ORG_OP_SSCANF( oStrToken, "%x", &uiID );
				poOp->SetID( uiID + uiIDOffset ); // Um keinen Konflikt zu erzeugen.
			}
			// flags
			else if( i == 3 )
			{
				unsigned int uiFlags;
				ORG_OP_SSCANF( oStrToken, "%x", &uiFlags );
				poOp->SetFlags( uiFlags );
			}
			// count_input
			else if ( i == 4 )
			{
				ORG_OP_SSCANF( oStrToken, "%x", &uiCountIn );
				if( poOp->GetFlags() & OP_FLAG_DYNAMIC_INPUTS )
				{
					poOp->SetCountIn( uiCountIn );
				}
				else if( poOp->GetCountIn() != uiCountIn )
				{
#ifdef OP_USE_RUNTIME_INFO
					ORG_OP_LOG( "Fehler: Von Klasse %s werden nicht %d Inputs unterstuetzt! "
						     "Keine dynamischen Inputs!\n", poOp->GetNameClass(), uiCountIn );
#endif // OP_USE_RUNTIME_INFO
					return false;
				}
				uiPortIn = 0;
				uiIndexIn = 0;
				uiMaxIPortIn = 3 * uiCountIn + 4 + 1;
				
				// <mod date="2010-12-07">
				poArrID = new CArray<unsigned int>( uiCountIn );
				oLstArrID.Append( poArrID );
				// </mod>
			}
			// inputs
			else if( i < uiMaxIPortIn )
			{
				unsigned int uiVal;
				ORG_OP_SSCANF( oStrToken.GetData(), "%x", &uiVal );
				switch( uiIndexIn )
				{
				case 0: // id
					uiIDIn = uiVal;
				break;
				case 1: // port
					uiIdxIn = uiVal;
				break;
				case 2: // flags
					if( uiIDIn )
						uiIDIn += uiIDOffset; // Um keinen Konflikt zu erzeugen.
					
					// <mod date="2010-12-07">
					poArrID->At( uiPortIn ) = uiIDIn;
					poOp->In( uiPortIn ) = COp::CLink( 0, uiIdxIn, uiVal );
					//poOp->In( uiPortIn ) = COp::CLink( reinterpret_cast<COp *>( uiIDIn ), uiIdxIn, uiVal );
					// </mod>
					
					++uiPortIn;
				break;
				}
				++uiIndexIn;
				if( uiIndexIn == 3 )
					uiIndexIn = 0;
			}

			// count_output
			else if( i == uiMaxIPortIn )
			{
				ORG_OP_SSCANF( oStrToken, "%x", &uiCountOut );
				if( poOp->GetFlags() & OP_FLAG_DYNAMIC_OUTPUTS )
				{
					poOp->SetCountOut( uiCountOut );
				}
				else if( poOp->GetCountOut() != uiCountOut )
				{
#ifdef OP_USE_RUNTIME_INFO
					ORG_OP_LOG( "Fehler: Von Klasse %s werden nicht %d Outputs unterstuetzt! "
						     "Keine dynamischen Outputs!\n", poOp->GetNameClass(), uiCountOut );
#endif // OP_USE_RUNTIME_INFO
					return false;
				}
				uiPortOut = 0;
				uiMaxIPortOut = uiCountOut + uiMaxIPortIn + 1;
			}
			// outputs
			else if( i < uiMaxIPortOut )
			{
				double dVal;
				ORG_OP_SSCANF( oStrToken.GetData(), "%lf", &dVal );
				poOp->Out( uiPortOut ) = dVal;
				++uiPortOut;
			}

			// int internal count
			else if( i == uiMaxIPortOut )
			{
				unsigned int uiCountInternalInt;
				ORG_OP_SSCANF( oStrToken, "%x", &uiCountInternalInt );
				if( poOp->GetCountInternalInt() != uiCountInternalInt )
				{
#ifdef OP_USE_RUNTIME_INFO
					ORG_OP_LOG( "Fehler: Von Klasse %s werden nicht %d Int-Interals unterstuetzt!\n", poOp->GetNameClass(), uiCountIn );
#endif // OP_USE_RUNTIME_INFO
					return false;
				}
				uiInteralInt = 0;
				uiMaxIInternalInt = uiMaxIPortOut + uiCountInternalInt + 1;
			}
			// int internals
			else if( i < uiMaxIInternalInt )
			{
				int iVal;
				ORG_OP_SSCANF( oStrToken.GetData(), "%d", &iVal );
				poOp->SetValueInternalInt( uiInteralInt, iVal );
				++uiInteralInt;
			}
			// uint internal count
			else if( i == uiMaxIInternalInt )
			{
				unsigned int uiCountInternalUInt;
				ORG_OP_SSCANF( oStrToken, "%x", &uiCountInternalUInt );
				if( poOp->GetCountInternalUInt() != uiCountInternalUInt )
				{
#ifdef OP_USE_RUNTIME_INFO
					ORG_OP_LOG( "Fehler: Von Klasse %s werden nicht %d UInt-Interals unterstuetzt!\n", poOp->GetNameClass(), uiCountIn );
#endif // OP_USE_RUNTIME_INFO
					return false;
				}
				uiInteralUInt = 0;
				uiMaxIInternalUInt = uiMaxIInternalInt + uiCountInternalUInt + 1;
			}
			// uint internals
			else if( i < uiMaxIInternalUInt )
			{
				unsigned int uiVal;
				ORG_OP_SSCANF( oStrToken.GetData(), "%u", &uiVal );
				poOp->SetValueInternalUInt( uiInteralUInt, uiVal );
				++uiInteralUInt;
			}
			// flt internal count
			else if( i == uiMaxIInternalUInt )
			{
				unsigned int uiCountInternalFlt;
				ORG_OP_SSCANF( oStrToken, "%x", &uiCountInternalFlt );
				if( poOp->GetCountInternalFlt() != uiCountInternalFlt )
				{
#ifdef OP_USE_RUNTIME_INFO
					ORG_OP_LOG( "Fehler: Von Klasse %s werden nicht %d Float-Interals unterstuetzt!\n", poOp->GetNameClass(), uiCountIn );
#endif // OP_USE_RUNTIME_INFO
					return false;
				}
				uiInteralFlt = 0;
				uiMaxIInternalFlt = uiMaxIInternalUInt + uiCountInternalFlt + 1;
			}
			// flt internals
			else if( i < uiMaxIInternalFlt )
			{
				double dVal;
				ORG_OP_SSCANF( oStrToken.GetData(), "%lf", &dVal );
				poOp->SetValueInternalFlt( uiInteralFlt, dVal );
				++uiInteralFlt;
			}
			else
				break;
			++i;
		}
	}

	// Pseudo-Patch nun mit echten Verbindungen versehen.
	i = oLstPatch.GetSize();
	/*
	if( i < 2 )
	{
		ORG_OP_LOG( "Fehler: Weniger als 2 Operatoren eingelesen!\n" );
		return false;
	}
	*/

	// Altes Patch löschen.
	//Clear();
	//m_poOpRootL_ = 0;
	//m_poOpRootR_ = 0;
	
	
	// <mod date="2010-12-07">
	oLstArrID.MoveToBack();
	// </mod>
	
	while( i )
	{
		--i;
		poOp = oLstPatch[i];
		
		// <mod date="2010-12-07">
		oLstArrID.GetPrev( &poArrID );
		// </mod>
		
		if( poOp )
		{
			unsigned int uiIn = poOp->GetCountIn();
			while( uiIn )
			{
				--uiIn;
				
				// <mod date="2010-12-07">
				const unsigned int uiTmpIDNext = poArrID->At( uiIn );
				//unsigned int uiTmpIDNext =
				//	reinterpret_cast<unsigned int>( poOp->In( uiIn ).GetOp() );
				// </mod>
				
				if( !uiTmpIDNext ) // ID 0 ist ungültig.
					continue;

				// Ganze Liste durchiterieren, und nach der ID aus dem aktuellen
				// Input-Array Ausschau halten!
				COp *poOpNext;
				oLstPatch.MoveToFront();
				while( oLstPatch.GetNext( &poOpNext ) )
				{
					if( poOpNext->GetID() == uiTmpIDNext )
					{
						poOp->In( uiIn ).SetOp( poOpNext );
					}
				}
			}
		}

		//if( i == 0 )
		//	m_poOpRootL_ = poOp;
		//else if ( i == 1 )
		//	m_poOpRootR_ = poOp;
	}
	
	// <mod date="2010-12-07">
	LIST_DELETE( oLstArrID, CArray<unsigned int> );
	// </mod>
	
	// Alle aufglösten Operatoren in die echte Liste einfügen...
	oLstPatch.MoveToFront();
	while( oLstPatch.GetNext( &poOp ) )
	{
#ifdef OP_USE_USER_DATA
		poOp->m_pvData = 0;
#endif // OP_USE_USER_DATA
		m_oLstOp.Append( poOp );
	}

	//UpdateAll();
	ValidateAll();
	return true;
}

void COrgOp::Clear()
{
	m_oLstOp.MoveToFront();
	while( m_oLstOp.Next() )
	{
		COp *poOp = m_oLstOp.Get();
		if( poOp )
			delete poOp;
	}
	m_oLstOp.Clear();
}


#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )

bool COrgOp::Encode_Op( CStr &roStrDst, COp *poOp ) const
{
	char acTmp[50];

	// class
	roStrDst += ORG_OP_DELIM;
	roStrDst += poOp->GetNameClass();

	// instance
	roStrDst += ORG_OP_DELIM;
	roStrDst += poOp->GetNameInstance();

	// id
	roStrDst += ORG_OP_DELIM;
	ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", poOp->GetID() );
	roStrDst += acTmp;

	// falgs
	roStrDst += ORG_OP_DELIM;
	ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", poOp->GetFlags() );
	roStrDst += acTmp;

	// count_input
	roStrDst += ORG_OP_DELIM;
	ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", poOp->GetCountIn() );
	roStrDst += acTmp;

	// inputs
	for( unsigned int i=0; i<poOp->GetCountIn(); ++i )
	{
		COp::CLink &roLink = poOp->In( i );
		COp *poOpIn = roLink.GetOp();

		// id
		unsigned int uiID = 0;
		if( poOpIn )
			uiID = poOpIn->GetID();
		roStrDst += ORG_OP_DELIM;
		ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", uiID );
		roStrDst += acTmp;

		// port
		unsigned int uiPort = roLink.GetIndex();
		roStrDst += ORG_OP_DELIM;
		ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", uiPort );
		roStrDst += acTmp;

		// flags
		unsigned int uiFlags = roLink.GetFlags();
		roStrDst += ORG_OP_DELIM;
		ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", uiFlags );
		roStrDst += acTmp;
	}

	// count_output
	roStrDst += ORG_OP_DELIM;
	//printf( "out: %s: %d", poOp->GetNameInstance(), poOp->GetCountOut() );
	ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", poOp->GetCountOut() );
	roStrDst += acTmp;

	// outputs
	for( unsigned int i=0; i<poOp->GetCountOut(); ++i )
	{
		roStrDst += ORG_OP_DELIM;
		ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%.16g", poOp->Out( i ) );
		roStrDst += acTmp;
	}

	// int internal count
	roStrDst += ORG_OP_DELIM;
	ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", poOp->GetCountInternalInt() );
	roStrDst += acTmp;

	// int internals
	for( unsigned int i=0; i<poOp->GetCountInternalInt(); ++i )
	{
		roStrDst += ORG_OP_DELIM;
		ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%d", poOp->GetValueInternalInt( i ) );
		roStrDst += acTmp;
	}

	// uint internal count
	roStrDst += ORG_OP_DELIM;
	ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", poOp->GetCountInternalUInt() );
	roStrDst += acTmp;

	// uint internals
	for( unsigned int i=0; i<poOp->GetCountInternalUInt(); ++i )
	{
		roStrDst += ORG_OP_DELIM;
		ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%u", poOp->GetValueInternalUInt( i ) );
		roStrDst += acTmp;
	}

	// flt internal count
	roStrDst += ORG_OP_DELIM;
	ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%x", poOp->GetCountInternalFlt() );
	roStrDst += acTmp;

	// flt internals
	for( unsigned int i=0; i<poOp->GetCountInternalFlt(); ++i )
	{
		roStrDst += ORG_OP_DELIM;
		ORG_OP_SPRINTF( acTmp, sizeof(acTmp), "%.16g", poOp->GetValueInternalFlt( i ) );
		roStrDst += acTmp;
	}

	roStrDst += ORG_OP_DELIM;
	roStrDst += ORG_OP_NEWLINE;
	return true;
}
#endif // VERSION_RUNTIME // VERSION_LIB

bool COrgOp::Decode_GetNextLine( CStr &roDst, CStr &roSrc ) const
{
	int j;

	j = roSrc.Find( 0, ORG_OP_DELIM );
	if( j < 0 )
		return false;
	roSrc.Del( 0, ++j );

	j = roSrc.Find( 0, ORG_OP_NEWLINE );
	if( j < 0 )
		return false;
	roDst = roSrc.GetSub( 0, j );
	roSrc.Del( 0, j+sizeof( ORG_OP_NEWLINE ) );
	ORG_OP_LOG( "line: %s\n", roDst.GetData() );
	return true;
}

bool COrgOp::Decode_GetNextToken( CStr &roDst, CStr &roSrc ) const
{
	int j = roSrc.Find( 0, ORG_OP_DELIM );
	if( j < 0 )
		return false;
	roDst = roSrc.GetSub( 0, j );
	roSrc.Del( 0, j+1 ); // ORG_OP_DELIM auch löschen
	ORG_OP_LOG( "token: %s\n", roDst.GetData() );
	return true;
}

void COrgOp::RegisterOpClass( const char *pcClassName, COrgOp::TFuncCreate tFuncCreate )
{
	if( pcClassName && tFuncCreate )
	{
		CStr oName = pcClassName;
		m_oArrClassName.Append( oName );
		m_oArrClassCreate.Append( tFuncCreate );
	}
}

