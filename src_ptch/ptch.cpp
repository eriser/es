// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// ptch.cpp

#define APP_NAME			"ptch"
#define APP_VERSION			"0.05"
#define APP_AUTHOR			"alucch"
#define APP_DATE			__DATE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../src/List/List.h"
#include "../src/Str/Str.h"

#define BUFFER_SIZE			256
#define FILE_NAME_OUT_RUN_PATCH		"run.patch.txt"
#define FILE_NAME_OUT_RUN_DEF		"run.def.txt"
#define FILE_NAME_OUT_RUN_REG		"run.reg.txt"
#define FILE_NAME_OUT_EDIT_PATCH	"edit.patch.txt"
#define SYS_DIR_DELIM			'\\'
#define VIS_OP_DELIM			'>'
#define OP_DELIM			':'
#define OP_DELIM_ZERO_DELIM		":0:"
#define OP_DELIM_DELIM			"::"
#define OP_DEF_PREFIX			"#define OP_INC"
#define OP_DEF_POSTFIX			"\n"
#define OP_REG_PREFIX			"ORG_OP_REGISTER_CLASS( "
#define OP_REG_POSTFIX			" );\n"
#ifdef _MSC_VER
#define FOPEN( file, ... )		fopen_s( &file, __VA_ARGS__ )
#else
#define FOPEN( file, ... )		file = fopen( __VA_ARGS__ )
#endif // _MSVC

int main( int iArg, char *ppcArg[] );
static int ReportError_( const char * pcMsg );
static void Quit_();

static FILE *l_pFileIn_ = 0;
static FILE *l_pFileOutRunPatch_ = 0;
static FILE *l_pFileOutRunDef_ = 0;
static FILE *l_pFileOutRunReg_ = 0;
static FILE *l_pFileOutEditPatch_ = 0;

static CList<CStr> l_oLstOpName_;

int main( int iArg, char *ppcArg[] )
{
	static unsigned char acBuffer_[BUFFER_SIZE];
	unsigned int uiDelimCount = 0;
	unsigned int uiCharCount = 0;
	bool bNL = true;
	CStr oStrOpName, oStrDef, oStrReg;
	CStr oStrRunPatchTmp;
	
	printf( "%s_v%s", APP_NAME, APP_VERSION );
	printf( " (%s, %s)\n", APP_AUTHOR, APP_DATE );
	
	if( iArg < 2 )
		return ReportError_( "syntax: " APP_NAME " <patch>" );
	
	atexit( Quit_ );
	
	printf( "input:\t%s\n", ppcArg[1] );
	FOPEN( l_pFileIn_, ppcArg[1], "r" );
	if( !l_pFileIn_ )
		return ReportError_( "open patch file" );
	
	/*if( iArg > 2 )
		FOPEN( l_pFileOutRun_, ppcArg[2], "w+" );
	else*/
	{
		CStr oStrRunPatch = CStr( FILE_NAME_OUT_RUN_PATCH );
		CStr oStrRunDef = CStr( FILE_NAME_OUT_RUN_DEF );
		CStr oStrRunReg = CStr( FILE_NAME_OUT_RUN_REG );
		CStr oStrEditPatch = CStr( FILE_NAME_OUT_EDIT_PATCH );
		const char *pcLst = strrchr( ppcArg[1], SYS_DIR_DELIM );
		if( pcLst )
		{
			CStr oStrDir = CStr( ppcArg[1] ).GetSub( 0, (unsigned int)( pcLst - ppcArg[1] + 1 ) );
			oStrRunPatch = oStrDir + oStrRunPatch;
			oStrRunDef = oStrDir + oStrRunDef;
			oStrRunReg = oStrDir + oStrRunReg;
			oStrEditPatch = oStrDir + oStrEditPatch;
		}
		printf( "output_run_patch:\t%s\n", oStrRunPatch.GetData() );
		printf( "output_run_inc:\t\t%s\n", oStrRunDef.GetData() );
		printf( "output_run_reg:\t\t%s\n", oStrRunReg.GetData() );
		printf( "output_edit_patch:\t%s\n", oStrEditPatch.GetData() );
		FOPEN( l_pFileOutRunPatch_, oStrRunPatch.GetData(), "w+" );
		FOPEN( l_pFileOutRunDef_, oStrRunDef.GetData(), "w+" );
		FOPEN( l_pFileOutRunReg_, oStrRunReg.GetData(), "w+" );
		FOPEN( l_pFileOutEditPatch_, oStrEditPatch.GetData(), "w+" );
	}
	//l_pFileOutRun_ = fopen( strcat( ppcArg[1], ".ptch" ), "w+" );
	
	if( !l_pFileOutRunPatch_ || !l_pFileOutRunDef_ || !l_pFileOutRunReg_ || !l_pFileOutEditPatch_ )
		return ReportError_( "open output file" );
	
	int c1 = ' '; // ACHTUNG: Neu!
	bool bIsNotVisOp = true;
	while( !feof( l_pFileIn_ ) )
	{
		size_t tRead = fread( acBuffer_, sizeof(unsigned char), BUFFER_SIZE, l_pFileIn_ );
		if( ferror( l_pFileIn_ ) )
			return ReportError_( "reading file" );
		
		size_t i = 0;
		
		while( bIsNotVisOp && i < tRead )
		{
			// rem: Nur so werden bleiben c1/cp auch über die Buffer-Grenzen
			//      hinaus aktuell!
			int c = c1;
			c1 = acBuffer_[i]; // Einkalkuliert. Achtung: Neu!
			
			if( bNL )
			{
				if( c == VIS_OP_DELIM )
				{
					bIsNotVisOp = false;
					break;
				}
				bNL = false;
				uiDelimCount = 0;
				//fputc( '"', l_pFileOutRunPatch_ ); // Zeile abschliessen.
				oStrRunPatchTmp += '"';
				fputc( '"', l_pFileOutEditPatch_ ); // Zeile abschliessen.
			}
			
			if( c == '\n' )
			{
				//rem: // Unix: \n, DOS: \n, MAC: 
				//fputs( "\\n\"\n", l_pFileOutRunPatch_ );
				oStrRunPatchTmp += "\\n\"\n";
				fputs( "\\n\"\n", l_pFileOutEditPatch_ );
				bNL = true;
			}
			else			
			{
				
				if( uiDelimCount != 2 ) // Instanz-Name auslassen.
				{
					//fputc( c, l_pFileOutRunPatch_ );
					oStrRunPatchTmp += c;
				}
				fputc( c, l_pFileOutEditPatch_ );
				
				if( uiDelimCount == 1 ) // Klassen-Name.
				{
					static bool bFirst_ = false; // hack
					static int cp = ' ';
					if( uiCharCount == 0 )
					{
						bFirst_ = true;
						oStrDef += OP_DEF_PREFIX;
						oStrReg += OP_REG_PREFIX;
					}
					else if( c == OP_DELIM )
					{
						bool bOk = true;
						l_oLstOpName_.MoveToFront();
						while( l_oLstOpName_.Next() )
						{
							if( l_oLstOpName_.Get() == oStrOpName )
							{
								bOk = false;
								break;
							}
						}
						if( bOk )
						{
							printf( "%s\n", oStrDef.GetData() );
							l_oLstOpName_.Append( oStrOpName );
							
							fputs( oStrDef.GetData(), l_pFileOutRunDef_ );
							fputs( OP_DEF_POSTFIX, l_pFileOutRunDef_ );
							fputs( oStrReg.GetData(), l_pFileOutRunReg_ );
							fputs( OP_REG_POSTFIX, l_pFileOutRunReg_ );
						}
						
						oStrOpName.Clear();
						oStrDef.Clear();
						oStrReg.Clear();
					}						
					else
					{
						//if( oStrDef == "#define OP_INC_OP_ENV_2" )
						//{
						//	printf( "stop" );
						//}
						
						bool bCUp = ( isupper( c ) || isdigit( c ) );
						if( bFirst_ ||
						    ( bCUp && ( islower( c1 ) && !isdigit( c1 ) ) ) ||
						    ( bCUp && ( islower( cp ) && !isdigit( cp ) ) ) )
						{
							bFirst_ = false;
							oStrDef += '_';
						}
						oStrDef += toupper( c );
						
						if( uiCharCount > 2 ) // skip "Op"
							oStrReg += char(c);
						
						oStrOpName += char(c);
					}
					cp = c;
				}
				
				if( c == OP_DELIM )
				{
					if( uiDelimCount == 2 )
					{
						// DELIM nachtragen.
						//fputc( c, l_pFileOutRunPatch_ );
						oStrRunPatchTmp += c;
					}
					
					++uiDelimCount;
					uiCharCount = 0;
				}
				else
					++uiCharCount;
			}
			++i;
		}
		while( !bIsNotVisOp && i<tRead )
		{
			int c = acBuffer_[i];
			if( bNL )
			{
				bNL = false;
				uiDelimCount = 0;	
				fputc( '"', l_pFileOutEditPatch_ ); // Zeile abschliessen.
			}
			if( c == '\n' )
			{
				fputs( "\\n\"\n", l_pFileOutEditPatch_ );
				bNL = true;
			}
			else
				fputc( c, l_pFileOutEditPatch_ );
			
			++i;
		}
	}
	
	while( oStrRunPatchTmp.Find( 0, OP_DELIM_ZERO_DELIM ) >= 0 )
	{
		oStrRunPatchTmp.Replace( OP_DELIM_ZERO_DELIM, OP_DELIM_DELIM );
	}
	fwrite( oStrRunPatchTmp.GetData(), 1, oStrRunPatchTmp.GetSize(), l_pFileOutRunPatch_ );
	
	return 0;
}

static void Quit_()
{
	if( l_pFileIn_ )
		fclose( l_pFileIn_ );
	if( l_pFileOutRunPatch_ )
		fclose( l_pFileOutRunPatch_ );
	if( l_pFileOutRunDef_ )
		fclose( l_pFileOutRunDef_ );
	if( l_pFileOutRunReg_ )
		fclose( l_pFileOutRunReg_ );
}

static int ReportError_( const char * pcMsg )
{
	printf( "error: %s\n", pcMsg );
	return -1;
}
