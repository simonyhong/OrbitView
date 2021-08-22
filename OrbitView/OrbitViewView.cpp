// OrbitViewView.cpp : implementation of the COrbitViewView class
//

#include "stdafx.h"
#include "OrbitView.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ThreeD.h"
#include "OrbitViewDoc.h"
#include "Graph2D.h"
#include "OrbitViewView.h"
#include "SinTable.h"
#include "DialogueBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL COrbitViewView::m_bGoFlg;
int COrbitViewView::m_dSampleAtEvery_Xms;
int COrbitViewView::m_nSampleRate;
/////////////////////////////////////////////////////////////////////////////
// COrbitViewView

IMPLEMENT_DYNCREATE(COrbitViewView, CView)

BEGIN_MESSAGE_MAP(COrbitViewView, CView)
	//{{AFX_MSG_MAP(COrbitViewView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_BIGGER_AMP, OnBiggerAmp)
	ON_COMMAND(ID_PLAY, OnPlay)
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_PLAY, OnUpdatePlay)
	ON_COMMAND(SMALLER_AMP, OnSmallerAmp)
	ON_COMMAND(ID_NORMAL_AMP, OnNormalAmp)
	ON_COMMAND(ID_PLAYFROMFILE, OnPlayFromFile)
	ON_COMMAND(ID_Parameters, OnChangeParameters)
	ON_COMMAND(ID_SPEED_UP, OnSpeedUp)
	ON_COMMAND(ID_SLOW_DOWN, OnSlowDown)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_FRONT_VIEW, &COrbitViewView::OnFrontView)
	ON_UPDATE_COMMAND_UI(ID_FRONT_VIEW, &COrbitViewView::OnUpdateFrontView)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, &COrbitViewView::OnUpdatePause)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SMALLER_PANNELS, &COrbitViewView::OnSmallerPanels)
	ON_COMMAND(ID_BIGGER_PANNELS, &COrbitViewView::OnBiggerPanels)
	ON_COMMAND(ID_APP_EXIT, &COrbitViewView::OnAppExit)
	ON_COMMAND(ID_SUBTRACT_AVERAGE, &COrbitViewView::OnSubtractAverage)
	ON_UPDATE_COMMAND_UI(ID_SUBTRACT_AVERAGE, &COrbitViewView::OnUpdateSubtractAverage)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// COrbitViewView construction/destruction
COrbitViewView::COrbitViewView()
{
	// TODO: add construction code here
	CRect rec(ThreeDLeft,  ThreeDUp, ThreeDRght, ThreeDDn);
	m_rectShow3D=rec;
	m_nColorShow=BLACK_BRUSH;
	m_bShow3D=0;  	m_bShow2D=0;  
	m_bFistTime=1;
	m_nKey=0;
	m_nSpeed=1;
	FlagVerScreen=0;
	m_bProceed=0;
	m_bIsToggleON=0;
	m_bGoFlg=0;
	m_dScaleUpDn=1;
	m_BlownUpPaneID=-1;
	m_dH_leye.SetSize(MAX_RECORD,0);
	m_dV_leye.SetSize(MAX_RECORD,0);
	m_dT_leye.SetSize(MAX_RECORD,0);
	m_dH_reye.SetSize(MAX_RECORD,0);
	m_dV_reye.SetSize(MAX_RECORD,0);
	m_dT_reye.SetSize(MAX_RECORD,0);
	m_nCountForRecord=0;
	m_nSampleRate=2;
	m_nShowTimeStart=0;m_nShowTimeEnd=10000;
	m_bPlay=0; m_bFront=1;
	m_dLargeMovement=1; 
	m_bDataLoaded=0;
	m_bPause=0;
	m_refreshScreen=0;
	m_startFrom=0; m_endAt=1000; m_DataEndsAt=0;
	OrdinateScale(5);
	////////////////////////////////////
	m_NULL.CreatePen(PS_NULL, 1,RGB(0,0,0));
	m_RED.CreatePen(PS_SOLID, 1,RGB(255,0,0));
	m_RED3.CreatePen(PS_SOLID, 3,RGB(255,0,0));
	m_RED5.CreatePen(PS_SOLID, 5,RGB(255,0,0));
	m_RED8.CreatePen(PS_SOLID, 8,RGB(255,0,0));
	m_GREEN.CreatePen(PS_SOLID, 1,RGB(0,255,0));
	m_BLUE.CreatePen(PS_SOLID, 1,RGB(100,100,255));
	m_BLUE3.CreatePen(PS_SOLID, 3,RGB(100,100,255));
	m_BLUE5.CreatePen(PS_SOLID, 5,RGB(100,100,255));
	m_BLUE8.CreatePen(PS_SOLID, 8,RGB(100,100,255));
	m_GREEN3.CreatePen(PS_SOLID, 3,RGB(0,255,0));
	m_GREEN5.CreatePen(PS_SOLID, 5,RGB(0,255,0));
	m_DkGREEN.CreatePen(PS_SOLID, 1,RGB(0,155,0));
	m_CYAN.CreatePen(PS_SOLID, 1,RGB(0,255,255));
	m_CYAN3.CreatePen(PS_SOLID, 3,RGB(200,255,255));
	m_PINK.CreatePen(PS_SOLID, 1,RGB(255,150,150));
	m_PINK3.CreatePen(PS_SOLID, 3,RGB(255,150,150));
	m_BROWN.CreatePen(PS_SOLID, 1,RGB(240,130,0));
	m_VIOLET.CreatePen(PS_SOLID, 1,RGB(200,90,250));
	m_VIOLET2.CreatePen(PS_SOLID, 2,RGB(200,90,250));
	m_VIOLET3.CreatePen(PS_SOLID, 3,RGB(200,90,250));
	m_GRAY.CreatePen(PS_SOLID, 1,RGB(170,170,170));
	m_GRAY3.CreatePen(PS_SOLID, 3,RGB(170,170,170));
	m_GRAY5.CreatePen(PS_SOLID, 5,RGB(170,170,170));
	m_BlueGRAY.CreatePen(PS_SOLID, 1,RGB(210,210,240));
	m_BLACK.CreatePen(PS_SOLID, 1,RGB(0,0,0));
	m_BLACK3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
	m_BLACK5.CreatePen(PS_SOLID, 5,RGB(0,0,0));

	int i,j;
	MyOriginalColorMix(m_RGB100,MaxRGB100);
	for(i=0;i<MaxPen;i++){
		j=(MaxRGB100-1)-i*(MaxRGB100/MaxPen);
		if(j<0)j=0;
		m_rgbPEN1[i].CreatePen(PS_SOLID, 1,m_RGB100[j]);
		m_rgbPEN2[i].CreatePen(PS_SOLID, 2,m_RGB100[j]);
		m_rgbPEN3[i].CreatePen(PS_SOLID, 3,m_RGB100[j]);
	}
	LOGBRUSH LogBrh[MaxBrush];
	for(i=0;i<MaxBrush;i++){
		LogBrh[i].lbStyle=BS_SOLID;
		LogBrh[i].lbColor=RGB((int)((255./MaxBrush)*i),(int)((255./MaxBrush)*i),(int)((255./MaxBrush)*i));
		m_Gray[i].CreateBrushIndirect( &LogBrh[i] );
	}

	/////////////// Parameters For Applications
	m_dVariable1=0;m_dVariable2=0;
	m_dVariable3=0;
	m_SubtractAvg=0;
	m_Ybias4Disp=1;//in degree, Give some bias to the right eye, so that it is more distinguisable from the left eye trace
	m_bIsToShowHead=0;
	
	/////  Statistics  ///////
	m_FirstTime0ElementErr=1;

}					


COrbitViewView::~COrbitViewView() 
{
	m_bPlay=0;
	
	for(int timeOut=0; timeOut<10; timeOut++){
		if(ThD.m_bIs3D==0)break;
		Sleep(10);
	}	
	m_dH_leye.RemoveAll();
	m_dV_leye.RemoveAll();
	m_dT_leye.RemoveAll();
	m_dH_reye.RemoveAll();
	m_dV_reye.RemoveAll();
	m_dT_reye.RemoveAll();
	m_AuxVec.RemoveAll();

	int i;
	for(i=0; i<MaxDC; i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	for(i=0; i<MaxBrush; i++)m_Gray[i].DeleteObject();
	m_bkBrush.DeleteObject();
	for(i=0; i<MaxPen; i++){
		m_rgbPEN1[i].DeleteObject(); m_rgbPEN2[i].DeleteObject(); m_rgbPEN3[i].DeleteObject();
	}
	m_NULL.DeleteObject(); m_RED.DeleteObject(); m_BLUE.DeleteObject(); m_GREEN.DeleteObject();m_DkGREEN.DeleteObject();m_CYAN.DeleteObject(); m_PINK.DeleteObject();m_BROWN.DeleteObject();m_VIOLET.DeleteObject();
	m_VIOLET2.DeleteObject();m_VIOLET3.DeleteObject();m_BLACK.DeleteObject();m_BLACK3.DeleteObject();m_GRAY.DeleteObject();m_GRAY3.DeleteObject();m_GRAY5.DeleteObject();m_CYAN3.DeleteObject();m_BlueGRAY.DeleteObject();m_PINK3.DeleteObject();m_RED3.DeleteObject(); m_BLUE3.DeleteObject(); m_GREEN3.DeleteObject();m_GREEN5.DeleteObject();
	m_RED5.DeleteObject(); m_BLUE5.DeleteObject(); m_BLACK5.DeleteObject(); m_RED8.DeleteObject(); m_BLUE8.DeleteObject(); m_BLACK8.DeleteObject();
	m_font.DeleteObject();

}

BOOL COrbitViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COrbitViewView printing

BOOL COrbitViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COrbitViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COrbitViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COrbitViewView diagnostics

#ifdef _DEBUG
void COrbitViewView::AssertValid() const
{
	CView::AssertValid();
}

void COrbitViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COrbitViewDoc* COrbitViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COrbitViewDoc)));
	return (COrbitViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COrbitViewView message handlers


//######################################################################################################
//######################################################################################################

void COrbitViewView::VirMemDC()
{
	//########  Preparing Virtual Window  ##########
	if(FlagVerScreen==1){
		//m_DC[0].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
		return;
	}
	FlagVerScreen=1;
////////////////////////////////
   LOGFONT lf;                        // Used to create the CFont.
   memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
   lf.lfHeight = 13;                  // Request a 20-pixel-high font
   strcpy(lf.lfFaceName, "Times New Roman");    //    with face name "Arial".
   m_font.CreateFontIndirect(&lf);    // Create the font.
   // Use the font to paint a control. This code assumes
   // a control named IDC_TEXT1 in the dialog box.
   SetFont(&m_font);
//////////////////////////////////////
	
	m_nScrMaxX=GetSystemMetrics(SM_CXSCREEN);
	m_nScrMaxY=GetSystemMetrics(SM_CYSCREEN);
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;	LogBrh.lbColor=RGB(220,220,220);	
	m_bkBrush.CreateBrushIndirect( &LogBrh );
	//m_bkBrush.CreateStockObject(WHITE_BRUSH);
	CClientDC aDC(this);
	
	for(int i=0;i<MaxDC;i++){
		m_DC[i].CreateCompatibleDC(&aDC);
		m_bmp[i].CreateCompatibleBitmap(&aDC, m_nScrMaxX, m_nScrMaxY);
		m_DC[i].SelectObject(&m_bmp[i]);
		m_DC[i].SelectObject(&m_bkBrush);
		m_DC[i].SelectObject(&m_font);
		m_DC[i].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
	}
	m_SavedDC=m_DC[0].SaveDC();
}

/////////////////////////////////////////////////////////////////////////////
// COrbitViewView drawing

void COrbitViewView::OnDraw(CDC* pDC)
{
	COrbitViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bFistTime==1 ){		
		VirMemDC();
		pDC->TextOut(50,240,"OrbitView v1.1");
		pDC->TextOut(50,260,"Open an eye/head movement file to play.");
		pDC->TextOut(50,280,"Made and distributed by Simon Hong (LSR/NEI/NIH, 2010; MIT, 2013) for free. Spread the words!");
		m_bFistTime=0;
	}
	else{
		if(m_bPlay){m_refreshScreen=1;}
		else pDC->BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

UINT PLAY_FromFILE(LPVOID WinObjPtr)
{
	COrbitViewView* pBbView=(COrbitViewView*) WinObjPtr;

	pBbView->Load_and_Play_fromFile();

	return 0;
}

void COrbitViewView::OnPlayFromFile() 
{
		AfxBeginThread( PLAY_FromFILE, this);
}


void COrbitViewView::OnPause() 
{
	m_bPause=!m_bPause;
}
void COrbitViewView::OnStop() 
{
	m_bPlay=0; //Quit all the functions.
}

void COrbitViewView::OnUpdatePause(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bPause);
}

void COrbitViewView::SetGraphs()
{
	CClientDC aDC(this);

	//The 3D eye ball figure
	m_DC[0].SelectObject(m_bkBrush);
	m_DC[0].Rectangle(0,0,m_nScrMaxX,m_nScrMaxY);
	m_Board[0].SetFig_Pos_n_Labels_XY ( m_dScaleUpDn*ThreeDLeft,  m_dScaleUpDn*ThreeDUp, m_dScaleUpDn*ThreeDRght, m_dScaleUpDn*ThreeDDn,"",""); 
	m_Board[0].Frame_Graph2D(-ThreeDView/2,ThreeDView/2,-ThreeDView/2,ThreeDView/2, &m_DC[0],xFRAME); //The middle point is (0,0)
	if(m_bDataLoaded){
		int x, y;
		m_Board[0].FromDataToScreenXY(-ThreeDView/2+10,(ThreeDView/2)*.95,x,y);//The middle point is (0,0).  X:-ThreeDView/2 to ThreeDView/2. Y:-ThreeDView/2 to ThreeDView/2.
		CRect rect(x,y,x+strlen(m_JustFileName)*8,y+20);
		FontInRect(&m_DC[0],m_JustFileName,rect); //Display the name of the file to fit in a given rectangle.
	}
	//The HVT graph figure
	CGraph2D *pBd=&m_TwoD1;
	char StrBuf[1024];
	if(m_Ybias4Disp==1)sprintf(StrBuf,"LEFT (COLOR),  RIGHT (GRAY); Scale: degree                 *For display purpose, right eye traces have been shifted by %0.0f degree",m_Ybias4Disp);
	else sprintf(StrBuf,"LEFT (COLOR),  RIGHT (GRAY); Scale: degree                 *For display purpose, right eye traces have been shifted by %0.1f degrees",m_Ybias4Disp);
	pBd->SetFig_Pos_n_Labels_XY( m_dScaleUpDn*330,m_dScaleUpDn*5,m_dScaleUpDn*1045,m_dScaleUpDn*685,"",StrBuf); 
	pBd->m_dMinX=m_nShowTimeStart; pBd->m_dMaxX=m_nShowTimeEnd;
	Tics_n_Lines(pBd,(int)(pBd->m_dMaxX-pBd->m_dMinX)/1000+1,-1);
	pBd->Frame_Graph2D(pBd->m_dMinX,pBd->m_dMaxX,0,m_dMaxOrdinateAxis, &m_DC[0],oFRAME); 
	m_DC[0].SelectObject(m_BLACK3);
	pBd->Graph2D(pBd->m_dMinX,0  , pBd->m_dMaxX, 0,2, &m_DC[0]);
	
	char ToPrint[100];
	for(float i=0.1;i<=1;i+=0.1){
		pBd->Graph2D(pBd->m_dMinX+(-0.02)*(pBd->m_dMaxX-pBd->m_dMinX),pBd->m_dMinY+(i)*(pBd->m_dMaxY-pBd->m_dMinY), pBd->m_dMinX, pBd->m_dMinY+(i)*(pBd->m_dMaxY-pBd->m_dMinY), 2, &m_DC[0]);
	}
	pBd->Graph2D(pBd->m_dMinX,pBd->m_dMinY+(0.1)*(pBd->m_dMaxY-pBd->m_dMinY), pBd->m_dMinX, pBd->m_dMinY+(0.3)*(pBd->m_dMaxY-pBd->m_dMinY), 2, &m_DC[0]);
	pBd->Graph2D(pBd->m_dMinX,pBd->m_dMinY+(0.4)*(pBd->m_dMaxY-pBd->m_dMinY), pBd->m_dMinX, pBd->m_dMinY+(0.6)*(pBd->m_dMaxY-pBd->m_dMinY), 2, &m_DC[0]);
	pBd->Graph2D(pBd->m_dMinX,pBd->m_dMinY+(0.7)*(pBd->m_dMaxY-pBd->m_dMinY), pBd->m_dMinX, pBd->m_dMinY+(0.9)*(pBd->m_dMaxY-pBd->m_dMinY), 2, &m_DC[0]);
	int IsInteger;
	if((int)(m_individualMaxOrdinateAxis)==m_individualMaxOrdinateAxis)IsInteger=1;
	else IsInteger=0;
	if(IsInteger){
		sprintf(ToPrint,"+%d",(int)(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.9),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"H   0 "); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.8),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"-%d",(int)(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.7),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"+%d",(int)(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.6),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"V   0 "); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.5),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"-%d",(int)(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.4),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"+%d",(int)(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.3),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"T   0 "); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.2),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"-%d",(int)(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.1),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"Time (ms)"); pBd->PutChar_Graph2D(MsgPosX(pBd,0.5),MsgPosY(pBd,-0.06),ToPrint,&m_DC[0]);
	}
	else{
		sprintf(ToPrint,"+%0.1f",(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.9),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"H   0 "); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.8),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"-%0.1f",(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.7),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"+%0.1f",(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.6),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"V   0 "); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.5),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"-%0.1f",(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.4),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"+%0.1f",(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.3),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"T   0 "); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.2),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"-%0.1f",(m_individualMaxOrdinateAxis)); pBd->PutChar_Graph2D(MsgPosX(pBd,-0.06),MsgPosY(pBd,0.1),ToPrint,&m_DC[0]);
		sprintf(ToPrint,"Time (ms)"); pBd->PutChar_Graph2D(MsgPosX(pBd,0.5),MsgPosY(pBd,-0.06),ToPrint,&m_DC[0]);
	}

	//The AUX signal figure
	if(m_howmanyAuxVectors>0){
		CGraph2D *pBd2=&m_TwoD2;
		pBd2->SetFig_Pos_n_Labels_XY( m_dScaleUpDn*330,m_dScaleUpDn*720,m_dScaleUpDn*1045,m_dScaleUpDn*1000,"","Aux signal"); 
		//###################  Decide the scale and labeling of the ordinate axis #############
		pBd2->m_dMinX=m_nShowTimeStart; pBd2->m_dMaxX=m_nShowTimeEnd;
		pBd2->m_dMinY=m_MinAux; pBd2->m_dMaxY=m_MaxAux;
		AdjustScale(pBd2->m_dMinY,pBd2->m_dMaxY);
		Tics_n_Lines(pBd2,(int)(pBd2->m_dMaxX-pBd2->m_dMinX)/1000+1,5);
		//#####################################################################################
		pBd2->Frame_Graph2D(pBd2->m_dMinX,pBd2->m_dMaxX,pBd2->m_dMinY,pBd2->m_dMaxY, &m_DC[0],oFRAME); 
		if(pBd2->m_dMinY<0 && pBd2->m_dMaxY>0){
			m_DC[0].SelectObject(m_GRAY);
			pBd2->Graph2D(pBd2->m_dMinX,0,pBd2->m_dMaxX,0,2,&m_DC[0]);//Show the "0" line.
			pBd2->PutChar_Graph2D(MsgPosX(pBd2,1.03),0,"0",&m_DC[0]);
		}
		//Legend
#define LabelRecWidth 80
#define LabelRecLeft (pBd2->m_nTx-LabelRecWidth)
		m_DC[0].SelectStockObject(WHITE_BRUSH);
		m_DC[0].SelectObject(m_NULL);
		m_DC[0].Rectangle(LabelRecLeft, pBd2->m_nTy, pBd2->m_nTx,pBd2->m_nBy);
		LOGFONT LogFont;
		m_font.GetLogFont(&LogFont);
		int	HalfLetterHeight=LogFont.lfHeight/2;
		int i, idx,x,y;
		for(i=0;i<m_howmanyAuxVectors;i++){
			idx=(int)((MaxPen-1)*(i+1.0)/m_howmanyAuxVectors);
			m_DC[0].SelectObject(&m_rgbPEN1[idx]);
			pBd2->FromDataToScreenXY(pBd2->m_dMinX, (MsgPosY(pBd2,i/(float)m_howmanyAuxVectors)),x,y);
			m_DC[0].MoveTo(LabelRecLeft+LabelRecWidth/2.+9,y+HalfLetterHeight); m_DC[0].LineTo(LabelRecLeft+LabelRecWidth-9,y+HalfLetterHeight);
			m_DC[0].TextOut(LabelRecLeft+5,y,m_AuxLabels[i]);
		}
		///
		m_DC[0].SelectObject(m_BLACK3);
		pBd2->Graph2D(pBd2->m_dMinX,pBd2->m_dMinY  , pBd2->m_dMaxX,pBd2->m_dMinY,2, &m_DC[0]);
	}
	
	aDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
}

void COrbitViewView::RefreshEverything() 
{	
	VirMemDC();
	SetGraphs();
}

void COrbitViewView::OnChangeParameters() 
{
	int CorrectInput=1;
	CDialogueBox	ParameterBOX;

	ParameterBOX.m_dVariable1=m_startFrom;
	ParameterBOX.m_dVariable2=m_endAt;
	ParameterBOX.m_dVariable3=m_individualMaxOrdinateAxis;
	int response=ParameterBOX.DoModal();
	if(response==IDOK){
		if(ParameterBOX.m_dVariable1<ParameterBOX.m_dVariable2 && ParameterBOX.m_dVariable1>=0 && ParameterBOX.m_dVariable2>=0){
			m_startFrom=ParameterBOX.m_dVariable1;
			m_endAt=ParameterBOX.m_dVariable2;
		}
		else { CorrectInput=0; AfxMessageBox("Unacceptable time interval: Do it again!");}

		if(ParameterBOX.m_dVariable3!=0){
			m_dVariable3=fabs(ParameterBOX.m_dVariable3);
			OrdinateScale(m_dVariable3);
		}
		else { CorrectInput=0; AfxMessageBox("The amplitude of an axis cannot be 0: Do it again!");}
	}
	if(CorrectInput==1 && response==IDOK)RefreshEverything();
}

void COrbitViewView::OnBiggerAmp() 
{
	m_dLargeMovement*=2.;
	VirMemDC();
	CGraph2D *pBd=&m_Board[0]; //The 3D window
	char ToPrint[100];
	sprintf(ToPrint,"Amplitude: x %0.2f",(float)m_dLargeMovement); 
	pBd->PutChar_Graph2D(MsgPosX(pBd,0.1),MsgPosY(pBd,-0.2),ToPrint,&m_DC[0]);
	if(m_bPlay){m_refreshScreen=1;}
	else{
		CClientDC aDC(this);
		aDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	}

}
void COrbitViewView::OnSmallerAmp() 
{
	m_dLargeMovement/=2.;
	VirMemDC();
	CGraph2D *pBd=&m_Board[0]; //The 3D window
	char ToPrint[100];
	sprintf(ToPrint,"Amplitude: x %0.2f",(float)m_dLargeMovement); 
	pBd->PutChar_Graph2D(MsgPosX(pBd,0.1),MsgPosY(pBd,-0.2),ToPrint,&m_DC[0]);
	if(m_bPlay){m_refreshScreen=1;}
	else{
		CClientDC aDC(this);
		aDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	}
}
void COrbitViewView::OnNormalAmp() 
{
	m_dLargeMovement=1.;
	VirMemDC();
	CGraph2D *pBd=&m_Board[0]; //The 3D window
	char ToPrint[100];
	sprintf(ToPrint,"Amplitude: x %0.2f",(float)m_dLargeMovement); 
	pBd->PutChar_Graph2D(MsgPosX(pBd,0.1),MsgPosY(pBd,-0.2),ToPrint,&m_DC[0]);
	if(m_bPlay){m_refreshScreen=1;}
	else{
		CClientDC aDC(this);
		aDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	}
}

void COrbitViewView::OnSlowDown() 
{
	m_nSpeed*=2;

	VirMemDC();
	CGraph2D *pBd=&m_Board[0]; //The 3D window
	char ToPrint[100];
	sprintf(ToPrint,"Speed: x %0.2f",(float)1./(float)m_nSpeed); pBd->PutChar_Graph2D(MsgPosX(pBd,0.1),MsgPosY(pBd,-0.1),ToPrint,&m_DC[0]);
	CClientDC aDC(this);
	pBd->PutChar_Graph2D(MsgPosX(pBd,0.1),MsgPosY(pBd,-0.1),ToPrint,&aDC);

}

void COrbitViewView::OnSpeedUp()//The speed does not go faster than the noraml speed! 
{
	m_nSpeed*=0.5;
	VirMemDC();
	CGraph2D *pBd=&m_Board[0]; //The 3D window
	char ToPrint[100];
	sprintf(ToPrint,"Speed: x %0.2f",(float)1./(float)m_nSpeed); pBd->PutChar_Graph2D(MsgPosX(pBd,0.1),MsgPosY(pBd,-0.1),ToPrint,&m_DC[0]);
	CClientDC aDC(this);
	pBd->PutChar_Graph2D(MsgPosX(pBd,0.1),MsgPosY(pBd,-0.1),ToPrint,&aDC);
}

void COrbitViewView::OnFrontView()
{
	m_bFront=!m_bFront;
	Observer_Origin();
}

void COrbitViewView::Observer_Origin()
{
	if(m_bFront){
		ThD.ObserverCntrRotX=0; ThD.ObserverCntrRotY=0;ThD.ObserverCntrRotZ=ONE_PI;
		ThD.ObserverCntrX=0 ;
		if(m_bIsToShowHead==0){
			ThD.ObserverCntrY=0.5*DISTANCE_FROM_PATIENCE; 
			ThD.ObserverCntrZ=0;
		}
		else{
			ThD.ObserverCntrY=DISTANCE_FROM_PATIENCE; 
			ThD.ObserverCntrZ=20;
		}
	}
	else{
		ThD.ObserverCntrRotX=0; ThD.ObserverCntrRotY=0;ThD.ObserverCntrRotZ=0;
		ThD.ObserverCntrX=0 ;
		if(m_bIsToShowHead==0){
			ThD.ObserverCntrY=-0.5*DISTANCE_FROM_PATIENCE; 
			ThD.ObserverCntrZ=0;
		}
		else{
			ThD.ObserverCntrY=-DISTANCE_FROM_PATIENCE; 
			ThD.ObserverCntrZ=20;
		}
	}
}
void COrbitViewView::OnUpdateFrontView(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bFront);
}

void COrbitViewView::OnBiggerPanels()
{
	if(m_dScaleUpDn<2.1)m_dScaleUpDn+=0.1;	
	m_bIsScrXYforIJ=0;
	m_DC[0].SelectObject(m_bkBrush);m_DC[0].Rectangle(0,0,m_nScrMaxX,m_nScrMaxY);
	m_bPause=1; Sleep(50); //Pause the on-going paly first.
	RefreshEverything();
	m_bPause=0; 
}

void COrbitViewView::OnSmallerPanels()
{
	if(m_dScaleUpDn>0.3)m_dScaleUpDn-=0.1;	
	m_bIsScrXYforIJ=0;
	m_DC[0].SelectObject(m_bkBrush);m_DC[0].Rectangle(0,0,m_nScrMaxX,m_nScrMaxY);
	m_bPause=1; Sleep(50); //Pause the on-going paly first.
	RefreshEverything();
	m_bPause=0; 
}


void COrbitViewView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	ThD.KeyDown(nChar);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void COrbitViewView::Tics_n_Lines(CGraph2D *pBrd, int HowManyTicsX, int HowManyTicsY) 
{
#define TICK_STEPX(pB) ((pB->m_dMaxX-pB->m_dMinX)/(pB->m_howManyX-1))
#define TICK_STEPY(pB) ((pB->m_dMaxY-pB->m_dMinY)/(pB->m_howManyY-1))
	int i;
	float TickStepX, TickStepY;

	pBrd->m_howManyX=HowManyTicsX; TickStepX=TICK_STEPX(pBrd);
	for(i=0;i<pBrd->m_howManyX;i++){pBrd->m_IsLabeledTicX[i]=1;pBrd->m_XTics[i]=pBrd->m_dMinX+i*TickStepX;} 
	pBrd->m_howManyY=HowManyTicsY; TickStepY=TICK_STEPY(pBrd);
	for(i=0;i<pBrd->m_howManyY;i++){pBrd->m_IsLabeledTicY[i]=1;pBrd->m_YTics[i]=pBrd->m_dMinY+i*TickStepY;} 
}


//#####################################################################################################################################################
//#####################################################################################################################################################
void COrbitViewView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC objDC(this);
	double relX, relY;

	m_selPaneID=WhichPane(point);

	CView::OnLButtonDown(nFlags, point);
}

//This "WhichPane()" function changes the size of the pane and its position, when the "blow-up button" is pressed. 
int COrbitViewView::WhichPane(CPoint point) 
{
#define BNDRY_MRGN 20
	int found=-1,j;
	CClientDC objDC(this);
	CGraph2D *pBrdJ;

	//If the rectangle on screen (m_paneTopX, m_paneTopY, m_paneBottomX, m_paneBottomY) is not defined for the graph pane, define it!
	//if(!m_bIsScrXYforIJ){ 
	//	for(j=0;j<MAX_GRAPHS;j++){
	//		pBrdJ=&m_Board[j];
	//		pBrdJ->ScreenXY((double)pBrdJ->m_dMinX, (double)pBrdJ->m_dMaxY, pBrdJ->m_nPaneTx, pBrdJ->m_nPaneTy); //It maps a relative point (i,j) of the graph to a point on the screen.	
	//		pBrdJ->ScreenXY((double)pBrdJ->m_dMaxX, (double)pBrdJ->m_dMinY, pBrdJ->m_nPaneBx, pBrdJ->m_nPaneBy); //It maps a relative point (i,j) of the graph to a point on the screen.	
	//	}
	//	m_bIsScrXYforIJ=1;
	//}
	for(j=0;j<MAX_GRAPHS;j++){
		pBrdJ=&m_Board[j];
		if(m_BlownUpPaneID>=0 && j!=m_BlownUpPaneID)continue; //This makes the non-blown up graphs inactive 
		if((point.x>=pBrdJ->m_nTx && point.x<=pBrdJ->m_nBx)&&(point.y<=pBrdJ->m_nBy&&point.y>=pBrdJ->m_nTy)){ 
			//Hit inside the pane
			found=j; 
			m_minPaneX=pBrdJ->m_dMinX; m_maxPaneX=pBrdJ->m_dMaxX; m_minPaneY=pBrdJ->m_dMinY; m_maxPaneY=pBrdJ->m_dMaxY;
			if((point.x<=pBrdJ->m_nBx && point.x>=pBrdJ->m_nBx-2*SmallButtonWidth)&&(point.y<=pBrdJ->m_nBy && point.y>=pBrdJ->m_nBy-2*SmallButtonHight)){ 
				//Hit inside the blow-up button
				pBrdJ->m_IsBlownUp=!pBrdJ->m_IsBlownUp;
				if(pBrdJ->m_IsBlownUp){
					pBrdJ->SetFig_Positions_XY(pBrdJ->m_BlownUpTx, pBrdJ->m_BlownUpTy, pBrdJ->m_BlownUpBx, pBrdJ->m_BlownUpBy);
					m_BlownUpPaneID=found; //This is true for now becasue there is just one blown-up figure at a time
				}
				else{
					pBrdJ->SetFig_Positions_XY(pBrdJ->m_normTx, pBrdJ->m_normTy, pBrdJ->m_normBx, pBrdJ->m_normBy);
					m_BlownUpPaneID=-1; //This is true for now becasue there  is just one blown-up figure at a time
				}
				RefreshEverything();
				m_bIsScrXYforIJ=0;//Remap the blown-up graph (and the rest).
			}
			//If hit inside the Y-axis adjustment button
			else if((point.x>=pBrdJ->m_nTx && point.x<=pBrdJ->m_nTx+SmallButtonWidth)&&(point.y>=pBrdJ->m_nTy && point.y<=pBrdJ->m_nTy+SmallButtonHight)){ //+Button (top left)
				pBrdJ->m_dMaxY+=.10;
			}
			else if((point.x>=pBrdJ->m_nTx && point.x<=pBrdJ->m_nTx+SmallButtonWidth)&&(point.y>=pBrdJ->m_nTy+8 && point.y<=pBrdJ->m_nTy+SmallButtonHight+8)){ //-Button (top left)
				if(pBrdJ->m_dMinY<pBrdJ->m_dMaxY-.10)pBrdJ->m_dMaxY-=.10;
			}
			else if((point.x>=pBrdJ->m_nTx && point.x<=pBrdJ->m_nTx+SmallButtonWidth)&&(point.y<=pBrdJ->m_nBy-8 && point.y>=pBrdJ->m_nBy-SmallButtonHight-8)){ //+Button (bottom left)
				if(pBrdJ->m_dMaxY>pBrdJ->m_dMinY+.10)pBrdJ->m_dMinY+=.10;
			}
			else if((point.x>=pBrdJ->m_nTx && point.x<=pBrdJ->m_nTx+SmallButtonWidth)&&(point.y<=pBrdJ->m_nBy && point.y>=pBrdJ->m_nBy-SmallButtonHight)){ //-Button (bottom left)
				pBrdJ->m_dMinY-=.10;
			}
			//Hit inside the paging bar
			else if((point.x>=pBrdJ->m_nYlabelPosX &&point.x<=pBrdJ->m_nYlabelPosX+ButtonWidth)&&(point.y>=pBrdJ->m_nYlabelPosY&&point.y<=pBrdJ->m_nYlabelPosY+ButtonHight)){
				if(pBrdJ->m_page<pBrdJ->m_NumPages-1)pBrdJ->m_page++; else pBrdJ->m_page=0;
			}
			pBrdJ->RefreshFrame(&objDC,&m_DC[pBrdJ->m_page]);
			m_bIsScrXYforIJ=0;//Remap the blown-up graph (and the rest).
			break;
		}
	}
	return found;
}


void COrbitViewView::OnAppExit()
{
	m_bPlay=0;
	
	for(int timeOut=0; timeOut<10; timeOut++){
		if(ThD.m_bIs3D==0)break;
		Sleep(10);
	}	
	exit(0);
}

void COrbitViewView::OnSubtractAverage()
{
	m_SubtractAvg=!m_SubtractAvg;
}

void COrbitViewView::OnUpdateSubtractAverage(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_SubtractAvg);
}


void COrbitViewView::MyOriginalColorMix(COLORREF RGB_array[],int arraySize) 
{	
	int R,G,B;
	double SqDist;
	COLORREF RGB;
	for(int i=0; i<arraySize; i++){
		//########  RED
		R=(255/(1+exp(-(((double)i)/arraySize-0.6)/0.03)));
		//########  GREEN
		SqDist=SQ_DISTANCE_AB_1D(i,arraySize*0.55);
		G=255*exp(-SqDist/(arraySize*10.));
		//########  BLUE
		SqDist=SQ_DISTANCE_AB_1D(i,arraySize*0.30);
		B=255*exp(-SqDist/(arraySize*6.));
		RGB_array[i]=RGB(R, G, B);
	}
}

void COrbitViewView::AdjustScale(double &minY, double &maxY) 
{
	int	i, howManyDivisionsY=4;
	int multiplied=0;
#define MultipliedBy 1000000.
	if(fabs(maxY)<1 && fabs(minY)<1){
		maxY*=MultipliedBy;minY*=MultipliedBy; 
		multiplied=1;
	}
	double spanY=maxY-minY;
	double cntrY=spanY/2+minY, savedCntrY=cntrY, diff;   
	double rawDivision=spanY/howManyDivisionsY;
	double ToleratDev20p=0.2*spanY;//20% of deviation may look okay.
	double DividedDev=ToleratDev20p/howManyDivisionsY;//This is the margin that a diviaion can move around
	
	char numberY0[100], numberY1[100],numberY2[100];
	int decimal, sign;
	//Translate the center
	diff=DifferenceToRound(cntrY,DividedDev);
	minY+=diff; maxY+=diff;		

	//Rescale the axis
	DifferenceToRound(maxY,DividedDev);
	minY=cntrY-((maxY+diff)-cntrY);
	spanY=maxY-minY;

	rawDivision=spanY/howManyDivisionsY;
	ToleratDev20p=0.2*spanY;//20% of deviation may look okay.
	DividedDev=ToleratDev20p/howManyDivisionsY;//This is the margin that a diviaion can move around
	diff=DifferenceToRound(minY,DividedDev);
//	maxY=cntrY+(cntrY-(minY+diff));//Don't do it.

	if(multiplied==1){maxY/=MultipliedBy;minY/=MultipliedBy;}

}
double COrbitViewView::DifferenceToRound(double &NumToAdjust, double VariableRange) 
{
	char  smallerNumStr[100],originalNumStr[100],biggerNumStr[100];
	int decimalSmall,decimalOriginal,decimalBigger, signSmall, signOriginal, signBigger;
	double NumToAdjustSaved=NumToAdjust, diff=0;
	double smallerNum=NumToAdjust-VariableRange,biggerNum=NumToAdjust+VariableRange;
	
	_fcvt_s(originalNumStr,sizeof(originalNumStr),NumToAdjust,0,&decimalOriginal, &signOriginal);//&decimal: Pointer to the stored decimal-point position.  &sign: Pointer to the stored sign indicator.
	if(fabs(NumToAdjust)<1.)sprintf(originalNumStr,"%s","0");
	_fcvt_s(smallerNumStr,sizeof(smallerNumStr),smallerNum,0,&decimalSmall, &signSmall);//&decimal: Pointer to the stored decimal-point position.  &sign: Pointer to the stored sign indicator.
	if(fabs(smallerNum)<1.)sprintf(originalNumStr,"%s","0");
	_fcvt_s(biggerNumStr,sizeof(biggerNumStr),biggerNum,0,&decimalBigger, &signBigger);//&decimal: Pointer to the stored decimal-point position.  &sign: Pointer to the stored sign indicator.
	if(fabs(biggerNum)<1.)sprintf(originalNumStr,"%s","0");
	if(*(smallerNumStr)!=*(biggerNumStr)||signSmall!=signBigger){
		if(signSmall!=signBigger){//The sign flipped: There is "0": this is a priority.
			NumToAdjust=0;
		}
		else if(*(smallerNumStr)!=*(originalNumStr)){//subtracting made a difference 
			//round down the numbers except the first one
			NumToAdjust=(int)NumToAdjust/(int)pow(10.,(int)(strlen(originalNumStr)-1))+1+(-1*signBigger);
			NumToAdjust*=pow(10.,(int)strlen(originalNumStr)-1);
		}
		else if(*(biggerNumStr)!=*(originalNumStr)){//adding made a difference ==>round up
			//round up the numbers except the first one
			NumToAdjust=(int)NumToAdjust/(int)pow(10.,(int)(strlen(originalNumStr)-1))+1+(-1*signBigger);
			NumToAdjust*=pow(10.,(int)strlen(originalNumStr)-1);
		}
		if(NumToAdjust>NumToAdjustSaved)diff=NumToAdjust-NumToAdjustSaved; else diff=NumToAdjustSaved-NumToAdjust;
	}
	else{//See if the second highest nuber can be rounded
		CString DoubleString;
		DoubleString = (originalNumStr+1);
		if(NumToAdjust<0)DoubleString.Insert(0, _T("-"));//Insert a characther like - or . to a specified location
		double secondNum = atof(DoubleString);
		diff=DifferenceToRound(secondNum, VariableRange); 
		NumToAdjust+=diff;
	}
	return diff;
}


// Writes a string sText with a size which fits perfectly in a rectangle rFont
// Font already attached with device context pDC is scaled
// This does not work for default font of the device contex
void COrbitViewView::FontInRect(CDC *pDC, CString sText, CRect &rFont)
{
	// Gets current font details
	LOGFONT LogFont;
	m_font.GetLogFont(&LogFont);

	// Scales the font to fit in the rectangle
	LogFont.lfHeight = -72 * rFont.Height() / pDC->GetDeviceCaps(LOGPIXELSY);
	LogFont.lfWidth = -72 * rFont.Width() / pDC->GetDeviceCaps(LOGPIXELSX) / sText.GetLength();

	// Create a font of calculated size
	CFont Font;
	Font.CreateFontIndirect(&LogFont);

	// Rechecks the string width and height
	CFont* pFont;
	pFont = pDC->SelectObject(&Font);
	CSize Size = pDC->GetTextExtent(sText);
	pDC->SelectObject(pFont);
	Font.DeleteObject();

	// Fine tunes the font width and height
	LogFont.lfWidth = (int) (LogFont.lfWidth * rFont.Width() / Size.cx);
	LogFont.lfHeight = (int) (LogFont.lfHeight * rFont.Height() / Size.cy);

	// Create a font with accurate size and displays string
	Font.CreateFontIndirect(&LogFont);
	pFont = pDC->SelectObject(&Font);
	pDC->DrawText(sText, rFont, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(pFont);
	Font.DeleteObject();
}

void COrbitViewView::OnLButtonUp(UINT nFlags, CPoint point)
{

	CView::OnLButtonUp(nFlags, point);
}

