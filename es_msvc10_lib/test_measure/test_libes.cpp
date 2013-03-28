// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// test_libes.cpp

#include <stdio.h>
#include "../../src_lib/es.h"
#include "../../src/Time/Time.h"

#ifdef _MSC_VER
#define FOPEN( file, ... )		fopen_s( &file, __VA_ARGS__ )
#else
#define FOPEN( file, ... )		file = fopen( __VA_ARGS__ )
#endif // _MSVC

int main( int iArg, char **ppcArg )
{
	// HACK!
	if( iArg <= 1 )
		return 1;
	
	FILE *pFileIn = 0;
	FOPEN( pFileIn, ppcArg[1], "r" );
	if( !pFileIn )
		return 1;
	fseek ( pFileIn, 0 , SEEK_END );
	const unsigned long int uliSize = ftell( pFileIn );
	rewind( pFileIn );
	
	char * pcBuffer = new char[ uliSize ];
	if( fread ( pcBuffer, 1, uliSize, pFileIn ) != uliSize )
		return -1;
	
	esLoad( pcBuffer );
	
	if( pcBuffer )
	{
		delete [] pcBuffer;
		pcBuffer = 0;
	}
	if( pFileIn )
		fclose( pFileIn );
	
	printf( "patch: %s\n\n", ppcArg[1] );
	
	double *pdIn = 0, *pdOut = 0;
	unsigned int uiCountIn, uiCountOut;
	esInter( &pdIn, &uiCountIn, &pdOut, &uiCountOut );
	
	printf( "  inputs:  %d (%x)\n", uiCountIn, (unsigned int)pdIn );
	printf( "  outputs: %d (%x)\n", uiCountOut, (unsigned int)pdOut );
	printf( "\ntest starts now...\n" );
	//printf( "\npress any key to start...\n" );
	//getchar();
	
	CTime oTime;
	oTime.Reset();
	const unsigned long int uliTimeStart = oTime.GetMilliseconds();
	
	esReset();
	unsigned int i = 0;
	while( true )
	{
		double dL, dR;
		if( esProc( &dL, &dR ) )
			break;
		//printf( "%g, %g\n", dL, dR );
		++i;
	}
	printf( "%d samples\n", i );
	
	const unsigned long int uliTimeDiff = oTime.GetMilliseconds() - uliTimeStart;
	printf( "%ld ms\n", uliTimeDiff );
	
	printf( "\npress any key to quit...\n" );
	getchar();
	
	return 0;
}
