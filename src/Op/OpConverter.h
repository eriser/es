// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpConverter.h

#ifndef _OP_CONVERTER_H_
#define _OP_CONVERTER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_CONVERTER

// Converter
////////////////////////////////////////////////////////////////////////////////
class COpConverter : public COp
{
public:
	COpConverter();
	virtual ~COpConverter();
protected:
	void ConvertBPM( double dBMP );
	void ConvertMIDI( double dMIDI, double dBase );
public:	
	virtual void Proc();
	virtual void Reset();

	OP_GENERIC_COPY_CTOR_DEC( COpConverter )
	OP_GENERIC_METHODS_DEC( COpConverter )

protected:
	double m_dBPM, m_dMIDI, m_dBase;
};

#endif // OP_INC_OP_CONVERTER
#endif // _OP_CONVERTER_H_
