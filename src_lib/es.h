// Copyright (c) 2008 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// es.h

#ifndef _ES_H_
#define _ES_H_

#define _ES_VERSION_	"0.03" // 2010-08-27

extern const char * esVersion();
extern bool esLoad( const char *pcData );
extern void esReset();
extern bool esProc( double *pdL, double *pdR );
extern void esInter( double **ppdIn, unsigned int *puiInCount, double **ppdOut, unsigned int *puiOutCount );

#endif // _ES_H_
