// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// test_libes.cpp

#include <stdio.h>
#include "../../src_lib/es.h"

int main()
{
	esLoad( 
#include "run.patch.txt"
	);
	
	double *pdIn = 0, *pdOut = 0;
	unsigned int uiCountIn, uiCountOut;
	esInter( &pdIn, &uiCountIn, &pdOut, &uiCountOut );
	
	printf( "\navailable interface ports (COpInter):\n" );
	printf( "  inputs:  %d (%x)\n", uiCountIn, (unsigned int)pdIn );
	printf( "  outputs: %d (%x)\n", uiCountOut, (unsigned int)pdOut );
	printf( "\npress any key to start...\n" );
	getchar();
	
	esReset();
	unsigned int i = 0;
	while( true )
	{
		double dL, dR;
		if( esProc( &dL, &dR ) )
			break;
		printf( "%g, %g\n", dL, dR );
		++i;
	}
	printf( "%d samples\n", i );
	
	return 0;
}
