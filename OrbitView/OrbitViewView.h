// OrbitViewView.h : interface of the COrbitViewView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//###############################################################################################################################
//----------------------------------------   My_Math.h --------------------------------------------------
#define RAND_RATE       ( (double)rand()/(double)RAND_MAX )
#define PROBABILITY(P)  (RAND_RATE<(double)P)
#define Random(N)       (rand()%(N))  // N should be an integer.

#define Sq2D(x,y)   ((x)*(x)+(y)*(y))        //Distance from the original to the given vector A
#define Sq3D(x,y,z) ((x)*(x)+(y)*(y)+(z)*(z))//Distance from the original to the given vector A
#define M3D(x,y,z)  sqrt((x)*(x)+(y)*(y)+(z)*(z))
#define M2D(x,y)    sqrt(((x)*(x)+(y)*(y)))
#define SQ_DISTANCE_AB_1D(x1,  x2)          ( ((x1)-(x2))*((x1)-(x2))  )
#define DISTANCE_AB_1D(x1,  x2)          sqrt( SQ_DISTANCE_AB_1D(x1,  x2) )
#define SQ_DISTANCE_AB(x1,y1,  x2,y2)          ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) )
#define DISTANCE_AB(x1,y1,  x2,y2) ( sqrt(           SQ_DISTANCE_AB(x1,y1,  x2,y2)                )  )
#define DISTANCE_OA(x1,y1) ( sqrt(   Sq2D(x1,y1)    )  )
#define	SQ_DISTANCE_AB_2D(x1,y1,  x2,y2)          ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) )
inline double CrossProduct(int a1, int a2, int b1, int b2 ){return a1*b2 - b1*a2; }
inline double CrossProduct(float a1, float a2, int b1, int b2 ){return a1*b2 - b1*a2; }
inline double CrossProduct(double a1, double a2, double b1, double b2 ){return a1*b2 - b1*a2; }
inline double DotProduct( int a1, int a2, int b1, int b2 ){return a1*b1 + a2*b2; }
inline double DotProduct( double a1, double a2, double b1, double b2 ){return a1*b1 + a2*b2; }
inline double DotProduct( float a1, float a2, float b1, float b2 ){return a1*b1 + a2*b2; }
inline double MAXab(double a, double b ){if(a>b)return a; else return b; }
inline double MINab(double a, double b ){if(a<b)return a; else return b; }

#define WM_MY_MESSAGE (WM_USER + 1)
#define SimDurationx10    1
#define MsgPosX(pB, relX) (pB->m_dMinX+(relX)*(pB->m_dMaxX-pB->m_dMinX))
#define MsgPosY(pB, relY) (pB->m_dMinY+(relY)*(pB->m_dMaxY-pB->m_dMinY))
#define ChrPosX(ID,relX) (m_Board[ID].m_dMinX+(relX)*(m_Board[ID].m_dMaxX-m_Board[ID].m_dMinX))
#define ChrPosY(ID,relY) (m_Board[ID].m_dMinY+(relY)*(m_Board[ID].m_dMaxY-m_Board[ID].m_dMinY))
#define RemoveForwardSlashAtEnd(Dir) if(Dir[strlen(Dir) - 1] == '\\') Dir[strlen(Dir) - 1] = '\0'; //Remove the forward slash.
#define AddFrwdSlashAtEndIfNotHave(Dir)    if(Dir[strlen(Dir) - 1] != '\\') strcat(Dir,"\\"); //Add the forward slash.


class CGraph2D;
class COrbitViewView : public CView
{
protected: // create from serialization only
	COrbitViewView();
	DECLARE_DYNCREATE(COrbitViewView)
//-------------------------------------------
public:
	//#######################################################################################################################################
	//#####################  Application starts from HERE  ##################################################################################
	int m_SizeOfEachChannel;
	int m_howmanyEyeVectors,m_VctrID[MaxEyeVectors+MaxAuxVectors],m_is_left_H,m_is_left_V,m_is_left_T,m_is_right_H,m_is_right_V,m_is_right_T,m_isLeftEye,m_isRightEye;
	int m_totVectors, m_howmanyAuxVectors;
	double m_avgAux[MaxAuxVectors],m_MinAux,m_MaxAux, m_prvAux[MaxAuxVectors]; 
	char m_AuxLabels[MaxAuxVectors][200];
	CArray<float, float&> m_AuxVec;
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int	MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);
	double m_dLargeMovement;
	double	m_dLeftX,m_dLeftY,m_dLeftZ,m_dRghtX,m_dRghtY,m_dRghtZ;
	double	m_dLeftH,m_dLeftV,m_dLeftT,m_dRghtH,m_dRghtV,m_dRghtT;
	float m_MaxH, m_MinH, m_MaxV, m_MinV, m_MaxT, m_MinT;
	float m_avelH,m_avelV,m_avelT, m_averH,m_averV,m_averT,m_Ybias4Disp;
	CArray<double, double&> m_dH_leye,m_dV_leye,m_dT_leye, m_dH_reye,m_dV_reye,m_dT_reye;
	BOOL	m_bFront,m_bPlay,m_bDataLoaded,m_bPause,m_SubtractAvg;
	#define MAX_RECORD 20000
	int	m_nCountForRecord;
	static int m_dSampleAtEvery_Xms,m_nSampleRate;
	void	PlotEyeTraces(int FrameNum);
	void	Load_and_Play_fromFile();
	void	Play();
	void	SetGraphs();
	#define MEM    10000			  //How many dots do you want to deal with?
	#define ThreeDView 200
	#define ThreeDLeft 5
	#define ThreeDUp   5
	#define ThreeDRght 315
	#define ThreeDDn   315
	int	EyeBall();
	int	Head_Shape();
	void	Observer_Origin();
	void	Tics_n_Lines(CGraph2D *pBrd, int HowManyTicsX, int HowManyTicsY) ;
	int m_nShowTimeStart, m_nShowTimeEnd, m_bIsToShowHead;
	double	m_OriginalFrameRate;
	int		m_FramesPerSec,m_refreshScreen;
	double m_dMaxOrdinateAxis,m_individualMaxOrdinateAxis;
	void	OrdinateScale(float individualMaxOrdinateAxis);
	double m_prvTime,m_prevrH,m_prevrV,m_prevrT,m_prevlH,m_prevlV,m_prevlT;
	double m_dHalfOrdinateAxis,m_d1FifthOrdinateAxis,m_d4FifthOrdinateAxis,m_dHalfOrdinateAxisPlus,m_d1FifthOrdinateAxisPlus,m_d4FifthOrdinateAxisPlus;
	char	m_FileName[500],m_FolderPath[500],m_JustFileName[100],m_FolderName[100]; 
	int GetFile_WholePath_Name_CurPath(BOOL IsToREAD);
	unsigned m_startFrom,m_endAt,m_DataEndsAt;

	//#####################  File  #########################################################################################
	//#######################################################################################################################
	int FromA_findB_addC_giveD(char FromHere[], char *FindThis, char AddThis[], char toMakeThis[]);
	int		FindAllFiles(char* searchThisDir, bool isToSearchSubDirs, char* FindJustFileName);	
	CString GetExecutableName();
	int	GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);

	//////////////////  Statistics ///////////////////////
	int		m_FirstTime0ElementErr;

	//////////////////  Basic Stuff //////////////////
#define MaxBrush 10
#define MaxPen 4
	CDC m_DC[MaxDC];
	CBitmap m_bmp[MaxDC];
	CBrush	m_Gray[MaxBrush];
	CBrush m_bkBrush;
	CPen m_rgbPEN1[MaxPen],m_rgbPEN2[MaxPen],m_rgbPEN3[MaxPen];
	CPen	m_NULL, m_RED, m_BLUE, m_GREEN,m_DkGREEN,m_CYAN, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_VIOLET2,m_VIOLET3,m_BLACK,m_BLACK3,m_GRAY,m_GRAY3,m_GRAY5,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED3, m_BLUE3, m_GREEN3,m_GREEN5;
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;

	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown;
	int		m_selPaneID; //This is for the mouse manipulation
	#define MaxRGB100 100
	COLORREF m_RGB100[MaxRGB100], m_BnW100[MaxRGB100];
	int m_nScrMaxX, m_nScrMaxY;
	static BOOL	 m_bGoFlg;
	BOOL	 m_bFistTime;
	BOOL	 m_bShow2D, m_bShow3D;
	int      m_nKey; //
	float      m_nSpeed; //
	int		 m_nColorShow,m_SavedDC;
	int		m_maxPaneX,m_minPaneX, m_minPaneY,m_maxPaneY,m_InfoPosX;
	int		m_BlownUpPaneID;
	int m_StepByStep;
	double	m_dScaleUpDn;
	BOOL m_bIsToggleON;
	CRect	 m_rectShow3D;
	CGraph2D m_TwoD1,m_TwoD2,m_TwoD3,m_TwoD4, m_TwoD5, m_TwoD6, m_TwoD7, m_TwoD8, m_TwoD9, m_TwoD10;
	CGraph2D m_TwoD11, m_TwoD12, m_TwoD13,m_TwoD14,m_TwoD15,m_TwoD16,m_TwoD17,m_TwoD18,m_TwoD19;
	#define MAX_GRAPHS 100
	CGraph2D m_3D1,m_3D2,m_3D3, m_TwoD101,m_TwoD102,m_TwoD103, m_TwoD104,m_Board[MAX_GRAPHS];
	void	VirMemDC();
	void	Init_Fig_Positions();
	BOOL	FlagVerScreen;
	BOOL	m_bProceed,m_bIsScrXYforIJ;
	/////////////// Variables for applications (For the Dialogue Box)
	double m_dVariable1, m_dVariable2, m_dVariable3;
	clock_t m_start,m_finish;
	//////////////////////////////////////////////////////////
	int		m_fontHeight;
	CRect	m_savedClientRect;
	void	FontInRect(CDC *pDC, CString sText, CRect &rFont);
	double	DifferenceToRound(double &NumToAdjust, double VariableRange);
	void	AdjustScale(double &minY, double &maxY);
	void	MyOriginalColorMix(COLORREF RGB_array[],int arraySize);
	int		Open_and_GetFileName(BOOL IsToREAD);
	int		wildcmp(char *wild, char *string);
	int		Find_LastNum_inString(char *GivenString);
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	int		WhichPane(CPoint point);
	void	RefreshEverything();

	CThreeD ThD;
//-------------------------------------------

// Attributes
public:
	COrbitViewDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrbitViewView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COrbitViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COrbitViewView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnBiggerAmp();
	afx_msg void OnSmallerAmp();
	afx_msg void OnNormalAmp();
	afx_msg void OnPlayFromFile();
	afx_msg void OnUpdatePlay(CCmdUI* pCmdUI);
	afx_msg void OnChangeParameters();
	afx_msg void OnSpeedUp();
	afx_msg void OnSlowDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFrontView();
public:
	afx_msg void OnUpdateFrontView(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePause(CCmdUI *pCmdUI);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnSmallerPanels();
public:
	afx_msg void OnBiggerPanels();
	afx_msg void OnAppExit();
	afx_msg void OnSubtractAverage();
	afx_msg void OnUpdateSubtractAverage(CCmdUI *pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in OrbitViewView.cpp
inline COrbitViewDoc* COrbitViewView::GetDocument()
   { return (COrbitViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
