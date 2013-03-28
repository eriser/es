// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OpEnvGlider.h

#ifndef _OP_ENV_GLIDER_H_
#define _OP_ENV_GLIDER_H_

#include "OpDEF.h"
#include "Op.h"

#ifdef OP_INC_OP_ENV_GLIDER

#include "../OpTk/OpTkEnv.h"

// Glider Envelope
////////////////////////////////////////////////////////////////////////////////
class COpEnvGlider : public COp
{
public:
	COpEnvGlider();
	virtual ~COpEnvGlider();
protected:
	void Init();
	inline void UpdateEnvLin();
	inline void UpdateEnvSin();
	inline void UpdateEnvSinPow();
public:
	virtual void Proc();
	virtual void Update();
	virtual void Reset();
#ifdef OP_USE_ROUTINES
	virtual void Routine( unsigned int uiIndex );
#endif // OP_USE_ROUTINES
	
	OP_GENERIC_COPY_CTOR_DEC( COpEnvGlider )
	OP_GENERIC_METHODS_DEC( COpEnvGlider )

protected:
	COpTkEnv1Lin<double> m_oEnvLin;
	COpTkEnv1Sin<double> m_oEnvSin;
	COpTkEnv1SinPow<double> m_oEnvSinPow;
	unsigned int m_uiFuncNum;
	double m_dDur, m_dV0, m_dV1, m_dSlope;
	COpTkEnv<double> *m_poEnv;
};

#endif // OP_INC_OP_ENV_GLIDER
#endif // _OP_ENV_GLIDER_H_
