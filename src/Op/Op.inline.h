// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// Op.inline.h

////////////////////////////////////////////////////////////////////////////////
// COp::CLink
////////////////////////////////////////////////////////////////////////////////

inline double & COp::CLink::Proc()
{
	m_poOp->Proc();
	return m_poOp->Out( m_uiIndex );
}

inline bool COp::CLink::IsValid() const {
	return ( m_poOp != 0 ); // && m_uiIndex < m_poOp->GetCountOut() );
}

inline COp * COp::CLink::GetOp() const {
	return m_poOp;
}

inline void COp::CLink::SetOp( COp * poOp ) {
	m_poOp = poOp;
}

inline unsigned int COp::CLink::GetIndex() const {
	return m_uiIndex;
}

inline void COp::CLink::SetIndex( unsigned int uiIndex ) {
	m_uiIndex = uiIndex;
}

inline unsigned int COp::CLink::GetFlags() const {
	return m_uiFlags;
}

inline void COp::CLink::SetFlags( unsigned int uiFlags ) {
	m_uiFlags = uiFlags;
}


////////////////////////////////////////////////////////////////////////////////
// COp
////////////////////////////////////////////////////////////////////////////////

inline unsigned int COp::GetCountIn() const {
	return m_uiCountIn;
}

inline COp::CLink & COp::In( unsigned int uiIndex ) const {
	return m_poIn[uiIndex];	
}

inline unsigned int COp::GetCountOut() const {
	return m_uiCountOut;
}

inline double & COp::Out( unsigned int uiIndex ) const {
	return m_pdOut[uiIndex];
}

inline bool COp::IsValid() {
	return !m_bIsNotValid;
}

inline unsigned int COp::GetID() const {
	return m_uiID;
}

inline void COp::SetID( unsigned int uiID ) {
	// Die höchste ID bleibt die grösste!
	if( uiID >= m_uiIDMax_ )
		m_uiIDMax_ = uiID + 1;
	m_uiID = uiID;
}

inline unsigned int COp::GetFlags() const {
	return m_uiFlags;
}

inline void COp::SetFlags( unsigned int uiFlags ) {
	m_uiFlags = uiFlags;
}

#ifdef OP_USE_INTERNALS
inline unsigned int COp::GetCountInternalInt() const {
	return m_uiCountInternalInt;
}

inline unsigned int COp::GetCountInternalUInt() const {
	return m_uiCountInternalUInt;
}

inline unsigned int COp::GetCountInternalFlt() const {
	return m_uiCountInternalFlt;
}

inline int COp::GetValueInternalInt( unsigned int uiIndex ) const {
	return *m_pptPtrInternalInt[uiIndex];
}

inline unsigned int COp::GetValueInternalUInt( unsigned int uiIndex ) const {
	return *m_pptPtrInternalUInt[uiIndex];
}

inline double COp::GetValueInternalFlt( unsigned int uiIndex ) const {
	return *m_pptPtrInternalFlt[uiIndex];
}

inline void COp::SetValueInternalInt( unsigned int uiIndex, int iVal ) {
	*( m_pptPtrInternalInt[uiIndex] ) = iVal;
}

inline void COp::SetValueInternalUInt( unsigned int uiIndex, unsigned int uiVal ) {
	*( m_pptPtrInternalUInt[uiIndex] ) = uiVal;
}

inline void COp::SetValueInternalFlt( unsigned int uiIndex, double dVal ) {
	*( m_pptPtrInternalFlt[uiIndex] ) = dVal;
}

inline void COp::SetPtrInternalInt( unsigned int uiIndex, int *piVal ) {
	m_pptPtrInternalInt[uiIndex] = piVal;
}

inline void COp::SetPtrInternalUInt( unsigned int uiIndex, unsigned int *puiVal ) {
	m_pptPtrInternalUInt[uiIndex] = puiVal;
}

inline void COp::SetPtrInternalFlt( unsigned int uiIndex, double *pdVal ) {
	m_pptPtrInternalFlt[uiIndex] = pdVal;
}
#endif // OP_USE_INTERNALS
#ifdef OP_USE_RUNTIME_INFO
inline const char * COp::GetNameInstance() const {
	return m_oStrNameInstance;
}

inline void COp::SetNameInstance( const char * acName ) {
	m_oStrNameInstance = acName;
}

inline const char * COp::GetNameInput( unsigned int uiIndex ) {
	return m_oArrStrNameIn[uiIndex];
}

inline void COp::SetNameInput( unsigned int uiIndex, const char *pcName ) {
	CStr oStrName = pcName;
	if( m_poIn[uiIndex].m_uiFlags & OP_LINK_FLAG_OPTIONAL )
		oStrName.Append( OP_STR_OPTIONAL_INPUT );
	m_oArrStrNameIn[uiIndex] = oStrName;
}

inline const char * COp::GetNameOutput( unsigned int uiIndex ) {
	return m_oArrStrNameOut[uiIndex];
}

inline void COp::SetNameOutput( unsigned int uiIndex, const char *pcName ) {
	m_oArrStrNameOut[uiIndex] = pcName;
}

inline const char * COp::GetInfoOutput( unsigned int uiIndex ) {
	return m_oArrStrInfoOut[uiIndex];
}

inline void COp::SetInfoOutput( unsigned int uiIndex, const char *pcInfo ) {
	m_oArrStrInfoOut[uiIndex] = pcInfo;
}

inline const char * COp::GetInfoInput( unsigned int uiIndex ) {
	return m_oArrStrInfoIn[uiIndex];
}

inline void COp::SetInfoInput( unsigned int uiIndex, const char *pcInfo ) {
	m_oArrStrInfoIn[uiIndex] = pcInfo;
}

#ifdef OP_USE_ROUTINES
inline const char * COp::GetInfoRoutine( unsigned int uiIndex ) {
	return m_oArrStrInfoRoutine[uiIndex];
}

inline void COp::SetInfoRoutine( unsigned int uiIndex, const char *pcInfo ) {
	m_oArrStrInfoRoutine[uiIndex] = pcInfo;
}

inline unsigned int COp::GetCountRoutine() const {
	return m_oArrStrNameRoutine.GetSize();
}

inline void COp::SetCountRoutine( unsigned int uiCount ) {
	m_oArrStrNameRoutine.Resize( uiCount );
	m_oArrStrInfoRoutine.Resize( uiCount );
}

inline const char* COp::GetNameRoutine( unsigned int uiIndex ) {
	return m_oArrStrNameRoutine[uiIndex];
}

inline void COp::SetNameRoutine( unsigned int uiIndex, const char *pcName ) {
	m_oArrStrNameRoutine[uiIndex] = pcName;
}

#endif // OP_USE_ROUTINES
#endif // OP_USE_RUNTIME_INFO
