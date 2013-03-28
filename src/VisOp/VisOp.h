// Copyright (c) 2009 Aurelio Lucchesi
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file "LICENSE.txt" in this distribution.
//
// VisOp.h

#ifndef _VIS_OP_H_
#define _VIS_OP_H_

#include "VisOpDEF.h"


// Visal Patch Editor
////////////////////////////////////////////////////////////////////////////////
class CVisOp
{
protected:
	typedef  void (* TFuncDialog)(void *, unsigned int);
	
protected:
	// Operator-Item
	////////////////////////////////////////////////////////////////////////////////
	class CItem
	{
	public:
		CItem( CItem *poItemOutFirst, unsigned int uiPortOut, COp *poOp, unsigned uiType=VIS_OP_ITEM_TYPE_STD, 
			GLfloat fX=0.0f, GLfloat fY=0.0f  );
		CItem( CItem  & roItem ); // const
		~CItem();
		
	public:
		CItem & operator = ( CItem & roItem ); // const
		
	public:
		void Render() const;
		void RenderLinkInOut( unsigned int uiPortIn, CItem *poItem, unsigned int uiPortOut ) const;
		void RenderLinkIn( unsigned int uiPortIn, GLfloat fX, GLfloat fY ) const;
		void RenderLinkOut( unsigned int uiPortOut, GLfloat fX, GLfloat fY ) const;
		
	public:	
		int  CallRoutine( GLfloat fX, GLfloat fY ); // Spezial-Funktionen auf Input-Seite.
		int  GetPortIn( GLfloat fX, GLfloat fY ) const; // Rückgabe von < 0 entspricht keinem Port!
		int  GetPortOut( GLfloat fX, GLfloat ) const; // s.o.
		bool CheckPos( GLfloat fX, GLfloat fY ) const; // Wahr, wenn (fX, fY) im Item.
		bool CheckArea( GLfloat fX0, GLfloat fY0, GLfloat fX1, GLfloat fY1 );
		int  CheckText( GLfloat fX, GLfloat fY ) const;
		
		CStr           GetText( int iID ) const;
		void           SetText( int iID, const CStr & roText );
		void           SetPos( GLfloat fX, GLfloat fY );
		void           TranslatePos( GLfloat fX, GLfloat fY );
		GLfloat        GetPos( unsigned int uiIndex );
		GLfloat        GetLeft() const;
		GLfloat        GetRight() const;
		GLfloat        GetBottom() const;
		GLfloat        GetTop() const;
		GLfloat        GetWidth() const;
		GLfloat        GetHeight() const; // Text-Bar mit eingerechnet!
		void           SetColor( GLubyte tR, GLubyte tG, GLubyte tB, GLubyte tA );
		void           SetColorRGB( GLubyte *ptC );
		void           SetColorRGBA( GLubyte *ptC );
		GLubyte        GetColor( unsigned int uiIndex );
		void           SetType( unsigned int uiType );
		unsigned int   GetType() const;
		COp *          GetOp() const;
		void           SetOp( COp *poOp );
		bool           GetSelect() const;
		void           SetSelect( bool bSelect );
		unsigned int   GetState() const;
		void           SetState( unsigned int uiState );
		void           NextState();
		
		unsigned int   GetCountOut() const; // Anzahl Output-Ports.
		unsigned int   GetCountOutConnections( unsigned uiPortOut ); // Anzahl Output-Items auf Output-Port uiPortOut.
		
		CItem *        GetItemOut( unsigned int uiPort, unsigned int uiIndex ) const;
		void           SetItemOut( unsigned int uiPort, unsigned int uiIndex, CItem *poItem );
		void           InsertItemOut( unsigned int uiPort, CItem *poItem );
		
		bool Save( CStr &roSrc, GLfloat fRefX, GLfloat fRefY ); // const
		bool Load( CStr &roDst, GLfloat fRefX, GLfloat fRefY );
		
		CStr Info();
		
	protected:
		void UpdateMetrics();
		void UpdateOutItemLists();
		void ClearOutputItemLists();
		
	public:
		void CopyConnections( CItem *poItemSrc );
		void DisconnectIn( unsigned int uiPortIn );	// Über Input-Seite verbundenes Item korrekt disconnecten.
		void DisconnectInAll();
		void DisconnectOutAll();
		void ResetIn(); // Input-Operatoren auf 0 setzen, Anzahl bleibt gleich!
		void ResetOut(); // Löscht die Output-Item-List aller Out-Port, die Anzahl bleibt gleich!
		
	public:
		// poItemOut ("links") mit pItemIn ("rechts") auf Port uiPortIn verbinden.
		static void Connect_( CItem *poItemIn, unsigned int uiPortIn, CItem *poItemOut, unsigned int uiPortOut );
	protected:
		static void RemoveInput_( CItem *poDst, const CItem *poInput ); // poInput aus poDst Input-Operator-Array entfernen.
		static void RemoveOutput_( CItem *poDst, unsigned int uiPortOut, CItem *poOutput ); // poOutput aus poDsts Output-Item-Liste eines Ports entfernen.
		static void RemoveOutputAll_( CItem *poDst, CItem *poOutput ); // poOutput aus poDsts allen Output-Item-Listen entfernen.
		static void ClearNestedItemArray_( CArray< CList<CItem *> *> *poArrNest ); // Listen löschen, Array-Grösse belassen.
		
	protected:
		CArray< CList<CItem *> *> m_oArrLstItemOut;
		COp *m_poOp;
		
		GLfloat m_afPos[2];
		GLubyte m_atColor[4];
		unsigned int m_uiType;
		bool m_bIsSelect;
		unsigned int m_uiState;
		
		GLfloat m_fLeft;
		GLfloat m_fRight;
		GLfloat m_fBottom;
		GLfloat m_fTop;
		
	public:
		static GLubyte m_atColorStd_[4];
		static GLubyte m_atColorRoot_[4];
		static GLubyte m_atColorSub_[4];
		static GLubyte m_atColorSup_[4];
		static bool m_bIsRender_;
	};
	
	friend class CItem;
	
public:
	CVisOp( COrgOp *poOrgOp );
	~CVisOp();
	
public:
	void Setup();
	void UpdateFont( CGLTxtFont *poFont );
	void Update();
	void Proc( int iX, int iY );
	
public:
	void Init(); // Danach: Update().
	void Clear();
	
	bool FileNew( unsigned int uiFileTemplate );
	
	bool FileLoad( const char *pcFileName );
	bool FileSave( const char *pcFileName );
	
	bool FileLoadMacro( const char *pcFileName );
	bool FileSaveMacro( const char *pcFileName );
	
	bool LoadPatch( const char *pcPatch ); // Danach: Update().
	
	bool Dialog( const char *pcMsg, TFuncDialog tFunc, void *ptArg0, unsigned int uiArg1 );
	void DialogOk();
	void DialogCancel();
	
	void PickBegin( unsigned int uiPickMode, unsigned int uiPickID, int iX, int iY );
	void PickProc( int iX, int iY );
	void PickEnd( unsigned int uiPickMode );
	
	void CoordTranslate( float fX, float fY );
	void CoordUpdate();

	void KeyFunctionA( bool bState );
	void KeyFunctionB( bool bState );
	void KeyFunctionInfo();
	void KeyChar( char cKey );
	
	void ShowCursor( bool bShow );
	void ToggleLog();
	void ToggleOscilloscope();

protected:
	bool SaveAll( CStr &roDst ); // const
	bool Save( CStr &roDst, CItem *poItem, GLfloat fRefX, GLfloat fRefY );
	bool Load( CStr &roSrc, unsigned int uiIDOffset, GLfloat fRefX, GLfloat fRefY );
	
	CItem * SearchItem( const COp *poOp );
	CItem * SearchItem( unsigned int uiID );
	void InsertItem( CItem *poItem );
	void RemoveItem( CItem *poItem );
	void InsertUnconnectedItems( GLfloat fX, GLfloat fY );
	
	bool SelectItem( CItem *poItem );
	void SelectAllItems();
	bool UnSelectItem( CItem *poItem );
	void UnSelectAllItems();
	
	CItem * GetSelectionSingle() const;
	
	void ClipboardCopyTo( CList<CItem *> *poLstItem );
	void ClipboardPasteFrom();
	void ClipboardDeleteAll();
	
	void ProcOp( COp *poOp );
	
	void SnapToGrid( CList<CItem *> *poLstItem ) const;
	void SnapToGrid( CItem *poItem ) const;
	void SnapToGrid( GLfloat *pfX, GLfloat *pfY ) const;
	
	void RenderBGOverlay() const;

protected:
	COrgOp *m_poOrgOp;
	
	CList<CItem *> m_oLstItem;
	CList<CItem *> m_oLstSelection;
	CList<CItem *> m_oLstClipboard;
	
	CItem *m_poItemRootL;
	CItem *m_poItemRootR;
	CItem *m_poItemPickMove;
	CItem *m_poItemPickMoveLinkIn0;
	CItem *m_poItemPickMoveLinkIn1;
	CItem *m_poItemPickMoveLinkOut0;
	CItem *m_poItemPickMoveLinkOut1;
	CItem *m_poItemPickAction0;
	CItem *m_poItemPickAction1;
	CItem *m_poItemPickTwiddle;
	CItem *m_poItemPickCurr;
	GLfloat m_afPickTwiddleStart[2];
	GLfloat m_afPickContext0Start[2];
	GLfloat m_afPickContext0Insert[2];
	GLfloat m_afPickMovePos[2];
	GLfloat m_afPickMoveLink[2];
	GLfloat m_afPickDefPos[4];
	GLint m_iXLast, m_iYLast;
	int m_iPickItemPortIn;
	int m_iPickItemPortOut;
	int m_iPickContext0;
	bool m_bIsPickContext0;
	bool m_bIsPickDefpos;
	bool m_poPickItemMoveGridTrigg;
	bool m_bTextSelectAll;
	bool m_bTextProtected;
	unsigned int m_uiTextMode;
	TFuncDialog m_tDialogFunc;
	void *m_pvDialogArg0;
	unsigned int m_uiDialogArg1;
	
	GLfloat m_fStpX;
	GLfloat m_fStpY;
	
	bool m_bFuncKeyA; // rem: In der Regel 'Shift'.
	bool m_bFuncKeyB; // rem: In der Regel 'Ctrl'.
	bool m_bShowCursor;
	bool m_bShowLog;
	bool m_bShowOscilloscope;
	bool m_bIsDialog;
	CStr m_oStrBuffer;
	CStr m_oStrFileName;
	int m_iTextID;
	
	GLfloat m_afCoordTrans[3];
	CTime m_oTime;
	CGLTxtFont *m_poGLTxtFont;
	static CGLTxtPrinter m_oGLTxtPrinter_;
	static CGLTool m_oGLTool_;
	static GLubyte m_aubColorFG_[3];
	static GLubyte m_aubColorBG_[3];
	static GLubyte m_aubColorHigh_[3];
	static GLubyte m_aubColorSignal0_[3];
	static GLubyte m_aubColorSignal1_[3];
	static unsigned int m_uiFrames_;
};

#endif // _VIS_OP_H_
