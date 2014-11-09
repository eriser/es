// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Main.cpp

#include "Main.h"
#include "../src/Sys/Sys.h"
#include "../src/Array/Array.h"
#include "../src/Str/Str.h"
#include "../src_lib/es.h"
#include "../src/File/FileStreamWav.h"

// rem: Sowohl Editor-Patches als auch run-Patches konvertiert durch 'ptch'
//      im C-String-Format können als gültige Input-Files verwendet werden!
#define PATCH_POSTFIX		".txt"
#define KEY_SKIP			'\x1B' // escape
#define BUFFER_VALUE_COUNT	( AUDIO_BUFFER_SIZE * 2 )

static bool ReadFile_( const char *pcName, CStr &roData );
static int ReportError_( const char * pcMsg );

int main( int iArg, char **ppcArg )
{
	LOG( "%s_v%s", APP_NAME, APP_VERSION );
	LOG( " (%s, %s)\n\n", APP_AUTHOR, APP_DATE );
	
	CSys::Init_();
	CSys::SetThreadPriority_( CSys::THREAD_PRIO_HIGH );
	
	// ACHTUNG: Das Arbeitsverzeichnis verändert sich nicht, somit
	// beziehen sich alle realtiven Pfade auf das Verzeichnis wo sich
	// eswav-Tool befindet. Referenzierte Midi-Files im Patch werden
	// z.B. nur genau in diesem Ordner gefunden!
	CStr oDir( "./" );
	if( iArg > 1 )
	{
		oDir = ppcArg[1];
		if( oDir.GetSize() )
		{
			char cSep = '/';
			if( oDir.Find( 0, '\\' ) >= 0 )
				cSep = '\\';
			if( oDir[oDir.GetSize()-1] != cSep )
				oDir += cSep;
		}
	}
	
	CArray<CStr> oArrFileName;
	
	LOG( "patches in %s\n", oDir.GetData() );
	if( CSys::GetFilesInDir_( oDir.GetData(), "*" PATCH_POSTFIX, &oArrFileName ) )
	{
		for( unsigned int i=0; i<oArrFileName.GetSize(); ++i )
		{
			LOG( "file: %s\n", oArrFileName[i].GetData() );
		}
	}
	
	if( !oArrFileName.GetSize() )
		return ReportError_( "no patches in directory" );
	
	for( unsigned int i=0; i<oArrFileName.GetSize(); ++i )
	{
		const char *pcFileName = oArrFileName[i].GetData();
		CStr oPatchData;
		
		if( !ReadFile_( pcFileName, oPatchData ) )
		{
			ERR( "can't read %s\n", pcFileName );
			continue;
		}
		if( !esLoad( oPatchData.GetData() ) )
		{
			ERR( "can't load %s\n", pcFileName );
			continue;
		}
		
		//double *pdIn = 0, *pdOut = 0;
		//unsigned int uiCountIn, uiCountOut;
		//esInter( &pdIn, &uiCountIn, &pdOut, &uiCountOut );
		
		esReset();
		
		LOG( "\nprocessing %s...\n", pcFileName );
		
		CStr oWavFileName( pcFileName );
		oWavFileName.Replace( PATCH_POSTFIX, ".wav" );
		CFileStreamWav oFileWav;
		oFileWav.m_uiSampleFreq = AUDIO_SAMPLE_FREQ;
		oFileWav.m_uiChannels = 2;
		oFileWav.Open( oWavFileName.GetData(), CFile::FLAG_WRITE | CFile::FLAG_TRUNC );
		
		static signed short int assiBuffer_[BUFFER_VALUE_COUNT];
		double dR, dL;
		bool bRun = true;
		unsigned long int uliSamples = 0;
		do
		{
			unsigned int s = 0;
			do
			{
				const bool bSig = esProc( &dL, &dR );
				assiBuffer_[s] = (signed short int)( dL * AUDIO_VALUE_MAX ); ++s;
				assiBuffer_[s] = (signed short int)( dR * AUDIO_VALUE_MAX ); ++s;
				++uliSamples;
				bRun = !bSig && !CSys::GetCharNonBlock_( KEY_SKIP );
			}
			while( bRun && s < BUFFER_VALUE_COUNT );
			
			// <mod date="2010-12-10">
			oFileWav.WriteValues( assiBuffer_, s );
			//oFileWav.Write( (void *)&assiBuffer_[0], s * sizeof(assiBuffer_[0]) );
			// </mod>
			LOG( "%s (%g KB)\n", pcFileName, ( uliSamples << 2 ) / 1024.0f );
		}
		while( bRun );
		while( CSys::GetCharNonBlock_( KEY_SKIP ) ) {}
		
		oFileWav.Close();
	}
	
	return 0;
}

static bool ReadFile_( const char *pcName, CStr &roData )
{
	roData.Clear();
	CFile oFile;
	if( !oFile.Open( pcName, CFile::FLAG_READ ) )
		return false;
	roData = CStr( oFile.GetSize() );
	oFile.Read( (void *)roData.GetData(), oFile.GetSize() );
	oFile.Close();
	return true;
}

static int ReportError_( const char * pcMsg )
{
	ERR( "%s\n", pcMsg );
	return -1;
}
