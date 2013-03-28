// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// OrgOp.inline.h

////////////////////////////////////////////////////////////////////////////////
// COrgOp
////////////////////////////////////////////////////////////////////////////////

inline void COrgOp::Proc_( double *pdLeft, double *pdRight )
{
	m_poOpRootL_->Proc();
	m_poOpRootR_->Proc();
	*pdLeft = m_poOpRootL_->Out( 0 );
	*pdRight = m_poOpRootR_->Out( 0 );
#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )
	if( m_bEnableCache_ )
	{
		m_adCache_[m_uiCachePos_++] = *pdLeft;
		m_adCache_[m_uiCachePos_++] = *pdRight;
		if( m_uiCachePos_ >= ORG_OP_CACHE )
			m_uiCachePos_ = 0;
	}
#endif // VERSION_RUNTIME // VERSION_LIB
}

#if !defined( VERSION_RUNTIME ) && !defined( VERSION_LIB )

inline COp * COrgOp::GetOpRootLeft_() {
	return m_poOpRootL_;
}

inline COp * COrgOp::GetOpRootRight_() {
	return m_poOpRootR_;
}

inline void COrgOp::SetOpRootLeft_( COp *poOp ) {
	m_poOpRootL_ = poOp;
}

inline void COrgOp::SetOpRootRight_( COp *poOp ) {
	m_poOpRootR_ = poOp;
}

inline void COrgOp::Rewind() {
	m_oLstOp.MoveToFront();
}

inline bool COrgOp::GetNext( COp ** ppoOp ) {
	return m_oLstOp.GetNext( ppoOp );
}
#endif // VERSION_RUNTIME // VERSION_LIB
