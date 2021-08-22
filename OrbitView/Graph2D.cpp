#include "stdafx.h"
#include <math.h>
#include "Graph2D.h"

BOOL CGraph2D::m_bUsingDC=0;
BOOL CGraph2D::m_bUsingDC2=0;
BOOL CGraph2D::m_bUsingDC3=0;

#define Dx         ((m_nBx)-(m_nTx))
#define Dy         ((m_nBy)-(m_nTy))
#define DoubleGapX                    ((m_dGapX)+(m_dGapX))
#define DoubleGapY                    ((m_dGapY)+(m_dGapY))
#define Dy_DoubleGapY                 (Dy-DoubleGapY)
#define Dx_DoubleGapX                 (Dx-DoubleGapX)
#define SHIFT_TO_X 10

#define lX(X) (int)((X-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_nTransX) //They are in (int) form because it is in screen pixel unit
#define lY(Y) (int)(m_nBy_GapY-(Y-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY)//They are in (int) form because it is in screen pixel unit
CGraph2D::CGraph2D(int Tx, int Ty, int Bx, int By, char Xaxis[50], char Yaxis[50])
{
	m_IsBlownUp=0; m_IsTickY=0;
	m_nTx=Tx; m_nTy=Ty; m_nBx=Bx; m_nBy=By;
	m_BlownUpTx=200;m_BlownUpTy=5; m_BlownUpBx=820; m_BlownUpBy=605;
	m_dGapX=(Dx)/8.; m_dGapY=(Dy)/8.; //Gap calculation should come after Tx, ...
	m_nBy_GapY=m_nBy-m_dGapY;
	m_dStepX=0;m_dStepY=0;
	m_LateralMargin=0;
	m_howManyX=-1;m_howManyY=-1;
	m_page=0; m_NumPages=1;

	m_PinkPen.CreatePen(PS_SOLID, 1,RGB(250,230,230));
	m_OrangePen.CreatePen(PS_SOLID, 1,RGB(255,190,50));
	m_RedPen.CreatePen(PS_SOLID, 1,RGB(255,50,50));
	m_BluePen.CreatePen(PS_SOLID, 1,RGB(0,0,255));
	m_GreenPen.CreatePen(PS_SOLID, 1,RGB(0,200,0));
	m_BlackPen3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
	m_GrayPen.CreatePen(PS_SOLID, 1,RGB(170,170,170));
	m_LightGrayPen.CreatePen(PS_SOLID, 1,RGB(230,230,230));
	m_bkBrush.CreateStockObject(WHITE_BRUSH);
	m_Null_Pen.CreatePen(PS_NULL, 1,RGB(0,0,0));

	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;	LogBrh.lbColor=RGB(200,200,200);	
	m_GrayBrush.CreateBrushIndirect( &LogBrh );
	LogBrh.lbColor=RGB(240,240,240);
	m_LightGrayBrush.CreateBrushIndirect( &LogBrh );
	
	int i;
	for(i=0;i<MaxDC;i++)sprintf(m_cXaxis[i],"");
	for(i=0;i<MaxDC;i++)sprintf(m_cYaxis[i],"");
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name

	m_dMinX=0., m_dMaxX=1000., m_dMinY=0., m_dMaxY=1.; m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY; 
	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));	m_dLabelBias=0;m_dLabelScale=1;

	m_Red=RGB(255,0,0); m_Blue=RGB(100,100,255); m_Green=RGB(0,255,0); m_DkGreen=RGB(0,155,0); m_Pink=RGB(255,150,150); 
	m_Cyan=RGB(0,255,255); m_Yellow=RGB(240,240,0); m_Brown=RGB(240,130,0); m_Violet=RGB(200,90,250);m_BLACK=RGB(0,0,0);
}

CGraph2D::CGraph2D()
{
	m_IsBlownUp=0; m_IsTickY=0;
	m_nTx=500; m_nTy=0; m_nBx=1000; m_nBy=500;
	m_BlownUpTx=200;m_BlownUpTy=5; m_BlownUpBx=820; m_BlownUpBy=605;
	m_dGapX=(Dx)/8.; m_dGapY=(Dy)/8.; //Gap calculation should come after Tx, ...
	m_nBy_GapY=m_nBy-m_dGapY;
	m_dStepX=0;m_dStepY=0;
	m_LateralMargin=0;
	m_howManyX=-1;m_howManyY=-1;
	m_page=0; m_NumPages=1;

	m_PinkPen.CreatePen(PS_SOLID, 1,RGB(250,230,230));
	m_OrangePen.CreatePen(PS_SOLID, 1,RGB(255,190,50));
	m_RedPen.CreatePen(PS_SOLID, 1,RGB(255,50,50));
	m_BluePen.CreatePen(PS_SOLID, 1,RGB(0,0,255));
	m_GreenPen.CreatePen(PS_SOLID, 1,RGB(0,200,0));
	m_BlackPen3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
	m_LightGrayPen.CreatePen(PS_SOLID, 1,RGB(230,230,230));
	m_GrayPen.CreatePen(PS_SOLID, 1,RGB(170,170,170));
	m_Null_Pen.CreatePen(PS_NULL, 1,RGB(0,0,0));
	m_bkBrush.CreateStockObject(WHITE_BRUSH);
	
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;	LogBrh.lbColor=RGB(200,200,200);	
	m_GrayBrush.CreateBrushIndirect( &LogBrh );
	LogBrh.lbColor=RGB(240,240,240);
	m_LightGrayBrush.CreateBrushIndirect( &LogBrh );
	
	int i;
	for(i=0;i<MaxDC;i++)sprintf(m_cXaxis[i],"");
	for(i=0;i<MaxDC;i++)sprintf(m_cYaxis[i],"");

	m_dMinX=0., m_dMaxX=1000., m_dMinY=0., m_dMaxY=1.; m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY; 
	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));
	m_dLabelBias=0;m_dLabelScale=1;
	m_Red=RGB(255,0,0); m_Blue=RGB(100,100,255); m_Green=RGB(0,255,0); m_DkGreen=RGB(0,155,0); m_Pink=RGB(255,150,150); 
	m_Cyan=RGB(0,255,255); m_Yellow=RGB(240,240,0); m_Brown=RGB(240,130,0); m_Violet=RGB(200,90,250);m_BLACK=RGB(0,0,0);m_GRAYCOLOR=RGB(210,210,240);
}

CGraph2D::~CGraph2D()
{	
	m_PinkPen.DeleteObject();m_OrangePen.DeleteObject();m_RedPen.DeleteObject();m_BluePen.DeleteObject();m_GreenPen.DeleteObject();m_BlackPen3.DeleteObject();m_GrayPen.DeleteObject();m_LightGrayPen.DeleteObject();m_Null_Pen.DeleteObject();
	m_bkBrush.DeleteObject();m_GrayBrush.DeleteObject();m_LightGrayBrush.DeleteObject();
}


void CGraph2D::SetLabelsXY(char Xaxis[50], char Yaxis[50])
{
	////////////
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	////////////
	if(!m_IsBlownUp)SetFig_Positions_XY(m_normTx,m_normTy,m_normBx,m_normBy);
	else            SetFig_Positions_XY(m_BlownUpTx, m_BlownUpTy, m_BlownUpBx, m_BlownUpBy);
}

void CGraph2D::SetFig_Pos_n_Labels_XY(int Tx, int Ty, int Bx, int By, char Xaxis[50], char Yaxis[50])
{
	////////////
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	////////////
	SetFig_Positions_XY(Tx, Ty, Bx, By);
}

void CGraph2D::Set_MinMax_n_Labels_XY(char Xaxis[50], char Yaxis[50],double Min_X, double Max_X, double Min_Y, double Max_Y,int oxFRAME)
{
	////////////
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	////////////

	m_dMinX=Min_X, m_dMaxX=Max_X, m_dMinY=Min_Y, m_dMaxY=Max_Y; m_dMeanY=(Min_Y+Max_Y)/2.; m_dSpanX=Max_X-Min_X; m_dSpanY=Max_Y-Min_Y; m_bDrawFRAME=oxFRAME;
	if(!m_IsBlownUp)SetFig_Positions_XY(m_normTx,m_normTy,m_normBx,m_normBy);
	else            SetFig_Positions_XY(m_BlownUpTx, m_BlownUpTy, m_BlownUpBx, m_BlownUpBy);
}

void CGraph2D::SetFig_Positions_XY(int Tx, int Ty, int Bx, int By)
{
	m_nTx=Tx; m_nTy=Ty; m_nBx=Bx; m_nBy=By;
	m_dGapX=30+(Dx)/80.; m_dGapY=20+(Dy)/80.; //Gap calculation should come after Tx, ...
	m_nBy_GapY=m_nBy-m_dGapY;

	m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY; 
	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));
	m_nTransX=(int)(m_dGapX+m_nTx+SHIFT_TO_X);
}


void CGraph2D::Frame_Graph2D(double Min_X, double Max_X, double Min_Y, double Max_Y, CDC* pDC,int oxFRAME)
{
	m_dMinX=Min_X, m_dMaxX=Max_X, m_dMinY=Min_Y, m_dMaxY=Max_Y; m_dMeanY=(Min_Y+Max_Y)/2.; m_dSpanX=Max_X-Min_X; m_dSpanY=Max_Y-Min_Y; m_bDrawFRAME=oxFRAME;
	Frame_Graph2D(pDC);
}

void CGraph2D::Frame_Graph2D(CDC* pDC)
{
#define TICK_Y 6
#define Tick 10

	int nSaveDC=pDC->SaveDC();

	//pDC->Rectangle((int)m_nTx-20, (int)m_nTy, (int)m_nTx,(int)m_nTy+100);
	if(m_bDrawFRAME==1){ //Draw the frame and the grid of the pannel
		int    shift=0;
		int i, sw=1;
		double  stepX,stepY;
		double L,Lo;
		char charVarTemp[100];
		m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
		m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));

		pDC->SelectObject(&m_BlackPen3); 
		pDC->MoveTo((int)m_nTx+5,(int)m_nBy);pDC->LineTo((int)m_nBx,(int)m_nBy);
		pDC->MoveTo((int)m_nBx,(int)m_nTy+5);pDC->LineTo((int)m_nBx,(int)m_nBy);
		pDC->SelectObject(&m_GrayPen); 
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Rectangle((int)m_nTx, (int)m_nTy, (int)m_nBx,(int)m_nBy);
		
		if(m_howManyY<0){ //Default setting (10 horizontal lines)
			stepY=(m_dSpanY)/10.;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
			pDC->SelectObject(&m_LightGrayPen);
			//----------------------------------Horizontal lines
			shift=0;
			for(i=0,L=m_dMinY;L<m_dMaxY+stepY/10.;L+= stepY, i++){
				Lo=lY(L);
				pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)Lo);
				pDC->LineTo((int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)Lo);
				//if(i%5==0){
				//	if(L==(int)L)sprintf(charVarTemp, "%.0lf ", L );
				//	else sprintf(charVarTemp, "%.2lf ", L );	
				//	//pDC->TextOut((int)(m_nTx+shift+SHIFT_TO_X-7), (int)Lo-5,charVarTemp);
				//	pDC->TextOut((int)(m_nTx+shift+SHIFT_TO_X-5), (int)Lo-5,charVarTemp);
				//}
			}
		}
		else{ //User defined vertical lines & ticks
			shift=15;
			pDC->SelectObject(&m_LightGrayPen);
			for(i=0;i<m_howManyY;i++){
				Lo=lY(m_YTics[i]);
				//Ticks
				//pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X-m_LateralMargin),(int)Lo); 
				//pDC->LineTo((int)(m_nTx+Tick+m_dGapX+SHIFT_TO_X-m_LateralMargin),(int)Lo);
				//Guidelines
				pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)Lo);
				pDC->LineTo((int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)Lo);
				if(m_IsLabeledTicY[i]){
					if(m_YTics[i]==(int)m_YTics[i])sprintf(charVarTemp, "%.0lf ", m_YTics[i] );
					else sprintf(charVarTemp, "%.1lf ", m_YTics[i] );	
					pDC->TextOut((int)(m_nTx+shift+SHIFT_TO_X-5), (int)Lo-5,charVarTemp);
				}
			}
		}
		if(m_howManyX<0){ //Default setting (10 vertical lines)
			stepX=m_dSpanX/10.;//Get_Var("Input interval :");
			//-------------------------Vertical lines
			for(i=0,L=m_dMinX;L<m_dMaxX+stepX/10.;L+=stepX,i++){
				Lo=lX(L);
				pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
				pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));
				if(sw==0){ sw=1; continue; }; sw=0;//print when even turn
				if(i%2==0){
					if(L*m_dLabelScale+m_dLabelBias==(int)(L*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", L*m_dLabelScale+m_dLabelBias );
					else sprintf(charVarTemp, "%.2lf", L*m_dLabelScale+m_dLabelBias );	
					//if(L==(int)L)sprintf(charVarTemp, "%.0lf ", L );
					//else sprintf(charVarTemp, "%.2lf ", L );	
				if(L>=10.)Lo-=5.;
					pDC->TextOut((int)(Lo)-3,(int)(m_nBy-m_dGapY+1+15*(i%2)),charVarTemp);
					//pDC->TextOut((int)(Lo)-5,(int)(m_nBy-m_dGapY+10+15*(i%2)),charVarTemp);
				}
			}
		}
		else{ //User defined horizontal lines & ticks
			shift=0;
			//-------------------------X axis
			for(i=0;i<m_howManyX;i++){
				Lo=lX(m_XTics[i]);
				//Ticks
				//pDC->MoveTo((int)Lo,(int)(m_nBy		-m_dGapY));	
				//pDC->LineTo((int)Lo,(int)(m_nBy-Tick-m_dGapY));
				//Guidelines
				pDC->SelectObject(&m_LightGrayPen);
				pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY)); pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));//Pink vertical guide lines		
				if(m_IsLabeledTicX[i]){
					pDC->SelectObject(m_BlackPen3);
					pDC->MoveTo((int)Lo,(int)(m_nBy-m_dGapY)); pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY+10)); //Black ticks		
					if(m_XTics[i]*m_dLabelScale+m_dLabelBias==(int)(m_XTics[i]*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", m_XTics[i]*m_dLabelScale+m_dLabelBias );
					else sprintf(charVarTemp, "%.2lf",m_XTics[i]*m_dLabelScale+m_dLabelBias );	
					//pDC->TextOut((int)(Lo)-3,(int)(m_nBy-m_dGapY+1),charVarTemp);
					pDC->TextOut((int)(Lo)-3,(int)(lY(m_dMinY-0.02*(m_dMaxY-m_dMinY))),charVarTemp);
				}
			}
		}
		//---------------------------X,Y axis
		pDC->TextOut((int)(m_nTx+m_dGapX+SHIFT_TO_X+45),(int)(m_nBy-m_dGapY+15),m_cXaxis[m_page]);//Horizontal coordinate Axis Name
		m_nYlabelPosX=(int)(m_nTx+m_dGapX+SHIFT_TO_X-30);m_nYlabelPosY=(int)(m_nTy+m_dGapY-20);
		pDC->TextOut((int)(m_nTx+m_dGapX+SHIFT_TO_X-30),(int)(m_nTy+m_dGapY-20),m_cYaxis[m_page]);//Vertical coordinate Axis Name
	}
	else{//Draw an EMPTY pannel.
		m_dGapX=0; m_dGapY=0; //Gap calculation should come after Tx, ...
		m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
		m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));
		m_nBy_GapY=m_nBy-m_dGapY;
		m_nTransX=(int)(m_dGapX+m_nTx);

		//ScreenXY(m_dMinX, m_dMinY, m_nPaneTx, m_nPaneTy); //It maps relative point (i,j) of the graph to a point on the screen.	
		//ScreenXY(m_dMaxX, m_dMaxY, m_nPaneBx, m_nPaneBy); //It maps relative point (i,j) of the graph to a point on the screen.	
		pDC->SelectStockObject(BLACK_BRUSH);
		pDC->Rectangle((int)m_nTx-1, (int)m_nTy-1, (int)m_nBx+1,(int)m_nBy+1);
		pDC->SelectObject(&m_BlackPen3); 
		pDC->MoveTo((int)m_nTx+5,(int)m_nBy);pDC->LineTo((int)m_nBx,(int)m_nBy);
		pDC->MoveTo((int)m_nBx,(int)m_nTy+5);pDC->LineTo((int)m_nBx,(int)m_nBy);
		pDC->SelectObject(&m_GrayPen); 
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Rectangle((int)m_nTx, (int)m_nTy, (int)m_nBx,(int)m_nBy);
		pDC->TextOut((int)(m_nTx+m_dGapX+SHIFT_TO_X-10),(int)(m_nBy-m_dGapY-12),m_cXaxis[m_page]);//Horizontal coordinate Axis Name
		m_nYlabelPosX=(int)(m_nTx+m_dGapX+SHIFT_TO_X-10);m_nYlabelPosY=(int)(m_nTy+m_dGapY-12);
		pDC->TextOut((int)(m_nTx+m_dGapX+SHIFT_TO_X-10),(int)(m_nTy+m_dGapY-12),m_cYaxis[m_page]);//Vertical coordinate Axis Name
	}

	pDC->RestoreDC(nSaveDC); 
} 

#define periRATIO 0.25
#define periGAP 10
void CGraph2D::formal_Frame2D(CDC* pDC, int IsPeriHist)
{
//#define Tick 5
#define Tick 10
	int nSaveDC=pDC->SaveDC();
	int shift, i;
	double  stepX,stepY, L,Lo;
	char charVarTemp[100];
	m_dMeanY=(m_dMinY+m_dMaxY)/2.;  m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY;
	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/double(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/double(m_dSpanX));

	pDC->SelectObject(&m_BluePen); 
	pDC->SelectObject(m_GrayBrush);
	if(IsPeriHist)pDC->Rectangle(lX(m_dMinX)-200,lY(m_dMaxY)-200,lX(m_dMaxX)+200,lY(m_dMinY)+200);
	else pDC->Rectangle(lX(m_dMinX)-50,lY(m_dMaxY)-50,lX(m_dMaxX)+50,lY(m_dMinY)+50);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->SelectObject(&m_BlackPen3); 
	pDC->Rectangle(lX(m_dMinX)-m_LateralMargin,lY(m_dMaxY),lX(m_dMaxX)+m_LateralMargin,lY(m_dMinY));

	if(m_dStepX==0)stepX=m_dSpanX/10.;
	else stepX=m_dStepX;
	if(m_dStepY==0)stepY=m_dSpanY/4.;
	else stepY=m_dStepY;
	//-------------------------Y axis
	shift=0;
	for(i=0;i<m_howManyY;i++){
		Lo=lY(m_YTics[i]);
		pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X-m_LateralMargin),(int)Lo); pDC->LineTo((int)(m_nTx+Tick+m_dGapX+SHIFT_TO_X-m_LateralMargin),(int)Lo);
		if(m_IsLabeledTicY[i]){
			if(m_YTics[i]==(int)m_YTics[i])sprintf(charVarTemp, "%.0lf ", m_YTics[i] );
			else sprintf(charVarTemp, "%.1lf ", m_YTics[i] );	
			pDC->TextOut((int)(m_nTx+shift+SHIFT_TO_X-5), (int)Lo-5,charVarTemp);
		}
	}
	//-------------------------X axis
	for(i=0;i<m_howManyX;i++){
		Lo=lX(m_XTics[i]);
		pDC->MoveTo((int)Lo,(int)(m_nBy		-m_dGapY));	pDC->LineTo((int)Lo,(int)(m_nBy-Tick-m_dGapY));
		if(m_IsLabeledTicX[i]){
			if(m_XTics[i]*m_dLabelScale+m_dLabelBias==(int)(m_XTics[i]*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", m_XTics[i]*m_dLabelScale+m_dLabelBias );
			else sprintf(charVarTemp, "%.2lf",m_XTics[i]*m_dLabelScale+m_dLabelBias );	
			pDC->TextOut((int)(Lo)-3,(int)(m_nBy-m_dGapY+1),charVarTemp);
		}
	}

	if(IsPeriHist==1){//peri-Histogram
		pDC->Rectangle(lX(m_dMinX),lY(m_dMaxY+periRATIO*(m_dMaxY-m_dMinY))-periGAP,lX(m_dMaxX),lY(m_dMaxY)-periGAP);//Vertical one
		pDC->Rectangle(lX(m_dMaxX)+periGAP,lY(m_dMaxY),lX(m_dMaxX+periRATIO*(m_dSpanX))+periGAP,lY(m_dMinY));//Horizontal one
	}
	pDC->RestoreDC(nSaveDC); 
}
//<----------------------------------------------------------------------------------|	
													//Don't Change! this allignment=>| 
void CGraph2D::Graph2D(double X, double Y,int Disp, CDC* pDC)//------------------|
{  

   if(Disp>10&&Disp<16){	
		  pDC->Rectangle( lX(X)+1,lY(Y)-1,lX(X)+1+(Disp-7),	lY(Y)-1+(Disp-7));
			return;
   }
   switch(Disp){ // Plot the object with (dots,line,circle...)
/*	  case 1:
		 pDC->Rectangle(lX(prevX), lY(Y),lX(X),(int)(m_nBy-m_dGapY));
		 break;
*/	  case 2:  //with Line
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 3:  //with Circle
		 pDC->Ellipse(CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3) );
		 break;
	  case 33:  //with Circle
		 pDC->Ellipse(CRect(lX(X)-2,lY(Y)-2,lX(X)+7,lY(Y)+7) );
		 break;
	  case 4:
		 pDC->LineTo(lX(X),lY(Y));
		 pDC->Ellipse(CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3) );
		 break;
	  case 5:
		pDC->SetPixel( lX(X),lY(Y),RGB(0,0,0));
		 break;
	  case 55:
		pDC->SetPixel( lX(X),lY(Y),RGB(0,0,0));
		 break;
	  case 56:
		pDC->SetPixel( lX(X),lY(Y),RGB(210,210,240));
		 break;
	  case 57: //  Color of IRIS
		//pDC->SetPixel( lX(X),lY(Y),RGB(19,63,17));
		//pDC->SetPixel( lX(X),lY(Y),RGB(91,150,32));
		pDC->SetPixel( lX(X),lY(Y),RGB(91,150,92));
		//pDC->SetPixel( lX(X),lY(Y),RGB(91,100,192));  //BLUE
		 break;
	  case 600:
		 pDC->MoveTo(lX(-40),lY(0));
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 601:
		 pDC->MoveTo(lX(+40),lY(0));
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 6:  //with Circle
		char strTemp[100];
		 pDC->Ellipse(CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3) );
		sprintf(strTemp,"      %0.3lf", Y);     
		pDC->TextOut(lX(X)+5, lY(Y), strTemp);
		sprintf(strTemp,"x%d:", X+1);     
		pDC->TextOut(lX(X)+5,lY(Y), strTemp);
		 break;
	  case 7:  //with Circle
		char strTemp1[100];
		sprintf(strTemp1,"       %0.3lf", Y);     
		pDC->TextOut(lX(0)-350, lY(1-0.053*X-.21), strTemp1);
		sprintf(strTemp1,"x%d:", X+1);     
		pDC->TextOut(lX(0)-350,lY(1-0.053*X-.21), strTemp1);
		 break;
	  case 8:  //with Circle
		char strTemp2[100];
		sprintf(strTemp2,"       %0.3lf", Y);     
		pDC->TextOut(lX(0)-560, lY(1-0.053*X-.21), strTemp2);
		sprintf(strTemp2,"I%d:", X+1);     
		pDC->TextOut(lX(0)-560, lY(1-0.053*X-.21), strTemp2);
		 break;
	  case 9:   //Move the initial point to (x,y) 
		 pDC->MoveTo(lX(X),lY(Y));
		 break;
	  case 10:  //with 1x1 Rectangle  
		  pDC->Rectangle( lX(X),lY(Y),lX(X)+2,lY(Y)+2);
		 break;
	  case 12:  //with 1x1 Rectangle  
		  pDC->Rectangle( lX(X),lY(Y),lX(X)+4,lY(Y)+4);
		 break;
	  case 30: // Histogram
		  pDC->MoveTo(lX(X),lY(0));
		  pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 31: // Histogram
		  pDC->Rectangle( lX(X)-1,lY(0),lX(X)+1,lY(Y));
		 break;
	  case 32: // Histogram
		  pDC->Rectangle( lX(X)-2,lY(0),lX(X)+2,lY(Y));
		 break;
	  case 34: // Histogram (the width of the column starts at the X and extend to X+10)
		  pDC->Rectangle( lX(X),lY(0),lX(X)+10,lY(Y));
		 break;
	  case 60: // Histogram 2
		  pDC->MoveTo(lX(X),lY(m_dMeanY));
		  pDC->LineTo(lX(X),lY(Y+m_dMeanY));
		 break;
	  case 90: // Histogram 3
		  pDC->MoveTo(lX(X),lY(m_dMaxY));
		  pDC->LineTo(lX(X),lY(m_dMaxY-Y));
		 break;
	  case 91: // Histogram 3
		  pDC->MoveTo(lX(X),lY(m_dMinY));
		  pDC->LineTo(lX(X),lY(m_dMinY-Y*0.4));
		 break;

	  default:  break;
   }//End of Switch

}
void CGraph2D::MyMoveTo(double X, double Y, CDC* pDC)
{
	pDC->MoveTo(lX(X),lY(Y));
}
void CGraph2D::MyLineTo(double X, double Y, CDC* pDC)
{
	pDC->LineTo(lX(X),lY(Y));
}

void CGraph2D::peri_Histogram(double X, double Y, int BinWidth, CDC* pDC)
{
	pDC->Rectangle( lX(X),lY(m_dMaxY)-periGAP,lX(X+BinWidth)-4,lY(m_dMaxY+periRATIO*(m_dMaxY-m_dMinY)*Y)-periGAP);//This "-4" in lX(XBinWidth)-4 is to extend the bin width to the next bin level
}


void CGraph2D::Histogram4(double X, double Y, int BinWidth,int Nth, CDC* pDC)
{
	pDC->Rectangle( lX(X+Nth*(BinWidth*0.25)),lY(Y),lX(X+(Nth+1)*(BinWidth*0.25)),lY(m_dMinY));
}

void CGraph2D::Histogram4(double X, double Y, float BinWidth,int Nth, CDC* pDC)
{
	pDC->Rectangle( lX(X+Nth*(BinWidth*0.25)),lY(Y),lX(X+(Nth+1)*(BinWidth*0.25)),lY(m_dMinY));
}

void CGraph2D::peri_Hist_Horizontal(double X, double Y, int BinWidth, CDC* pDC)
{
	pDC->Rectangle( lX(m_dMaxX)+periGAP,lY(Y),lX(m_dMaxX+periRATIO*(m_dSpanX)*X)+periGAP,lY(Y+BinWidth)+4);//This "+4" in lY(Y+BinWidth)+4 is to extend the bin width to the next bin level
}


void CGraph2D::Circle(double X, double Y,int size,CDC* pDC)
{
	 pDC->Ellipse(CRect(lX(X)-size,lY(Y)-size,lX(X)+size,lY(Y)+size) );
}

void CGraph2D::Triangle(double X, double Y,int size,CDC* pDC) // 
{
#define sqroot3 1.73205
#define HalfSqRoot3 0.866025
	m_vert[0].x=lX(X);					m_vert[0].y=lY(Y)-size; 
	m_vert[1].x=lX(X)-(int)(HalfSqRoot3*size);	m_vert[1].y=lY(Y)+(int)(0.5*size); 
	m_vert[2].x=lX(X)+(int)(HalfSqRoot3*size);	m_vert[2].y=lY(Y)+(int)(0.5*size); 

	Polygon(*pDC,m_vert,3);
}

void CGraph2D::Abs_Ellipse(double X, double Y,int size, CDC* pDC)
{  
	 pDC->Ellipse(CRect(lX(X)-size,lY(Y)-size,lX(X)+size,lY(Y)+size) );
}

void CGraph2D::ErrorBar(double X, double Y, double error, CDC* pDC)
{  
	 pDC->MoveTo(lX(X),lY(Y+error)); pDC->LineTo(lX(X),lY(Y-error)); //Vertical var
	 pDC->MoveTo(lX(X)-5,lY(Y+error)); pDC->LineTo(lX(X)+5,lY(Y+error)); //Upper horizontal bar
	 pDC->MoveTo(lX(X)-5,lY(Y-error)); pDC->LineTo(lX(X)+5,lY(Y-error)); //Lower horizontal bar
}

void CGraph2D::ErrorWing(double X, double Y, double error, CDC* pDC)
{  
	 pDC->MoveTo(lX(X),lY(Y+error)); pDC->LineTo(lX(X),lY(Y-error)); //Vertical var
}


void CGraph2D::Graph2D(double X1, double Y1, double X, double Y,int Disp, CDC* pDC)//------------------|
{  
   switch(Disp){ // Plot the object with (dots,line,circle...)
/*	  case 1:
		 pDC->Rectangle(lX(prevX),lY(Y), lX(X),(int)(m_nBy-m_dGapY));
		 break;
*/	  case 2:  //with Line
		 pDC->MoveTo(lX(X1),lY(Y1));
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 3:  //with Circle
		 pDC->Ellipse(	CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3)	 );
		 break;
	  case 4:
//		 pDC->MoveTo((int)((prevX-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_dGapX+m_nTx),lY(prevY));
		 pDC->LineTo(lX(X),lY(Y));
		 pDC->Ellipse(	CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3)	 );
		 break;
	  case 5:
		pDC->SetPixel( lX(X),lY(Y),RGB(200,0,0));
		 break;
	  case 6:  //with Circle
		char strTemp[100];
		 pDC->Ellipse(CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3) );
		sprintf(strTemp,"      %0.3lf", Y);     
		pDC->TextOut(lX(X)+5,	 lY(Y), strTemp);
		sprintf(strTemp,"x%d:", X1+1);     
		pDC->TextOut(lX(X)+5,	 lY(Y), strTemp);
		 break;
	  case 7:  //with Circle
		char strTemp1[100];
		sprintf(strTemp1,"       %0.3lf", Y);     
		pDC->TextOut(lX(0)-350, lY(1-0.053*X1-.21), strTemp1);
		sprintf(strTemp1,"x%d:", X1+1);     
		pDC->TextOut(lX(0)-350, lY(1-0.053*X1-.21), strTemp1);
		 break;
	  case 8:  //with Circle
		char strTemp2[100];
		sprintf(strTemp2,"       %0.3lf", Y);     
		pDC->TextOut(lX(0)-560, lY(1-0.053*X1-.21), strTemp2);
		sprintf(strTemp2,"I%d:", X1+1);     
		pDC->TextOut(lX(0)-560, lY(1-0.053*X1-.21), strTemp2);
		 break;
	  default:  break;
   }//End of Switch
}


void CGraph2D::PutIntVal_Custom_Graph2D(double X, double Y, int NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"Number of Nns=%d", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}


void CGraph2D::PutVal_Graph2D(double X, double Y, double NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%0.3lf", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}

void CGraph2D::PutVal_p5(double X, double Y, double NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%0.5lf", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}


void CGraph2D::PutIntVal_Graph2D(double X, double Y, int NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%d", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}
void CGraph2D::PutChar_Graph2D(double X, double Y, char ToPrint[], CDC* pDC)
{  
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), ToPrint);
}

void CGraph2D::PutChar_After10(double X, double Y, char ToPrint[], CDC* pDC)
{  
	pDC->TextOut((int)(lX(X)+10),(int)(lY(Y)-5), ToPrint);
}

void CGraph2D::RectangleAB(double X1, double Y1, double X2, double Y2,CDC* pDC)
{  
	pDC->Rectangle(lX(X1),lY(Y1),lX(X2),lY(Y2));
}

void CGraph2D::FromDataToScreenXY(double X, double Y, int &x, int &y)
{
	x=       (lX(X)); //They are in (int) form because it is in screen pixel unit
	y=       (lY(Y)); //They are in (int) form because it is in screen pixel unit
}

void CGraph2D::FillSolidRect(int X, int Y,int width, int height, COLORREF rgb ,CDC* pDC)
{ 
	pDC->FillSolidRect(lX(X)-width/2,lY(Y)-height/2,width,height,	rgb);
}
void CGraph2D::FillSolidRect(double X, double Y,int width, int height, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),width,height,	rgb);
}
void CGraph2D::FillSolidRect_CenteredY(double X, double Y,int width, int height, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y)-height/2,width,height,	rgb);
}

void CGraph2D::FillSolidRect13x13(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),13,	13,	rgb);
}

void CGraph2D::FillSolidRect8x8(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),8,	8,	rgb);
}

void CGraph2D::FillSolidRect7x7(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),7,	7,	rgb);
}

void CGraph2D::MySetPixel(double X, double Y, int GRAY, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),RGB(GRAY,GRAY,GRAY));
}

void CGraph2D::MySetPixelRED(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Red);
}

void CGraph2D::MySetPixelGREEN(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Green);
}

void CGraph2D::MySetPixelDKGRN(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_DkGreen);
}

void CGraph2D::MySetPixelBLUE(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Blue);
}

void CGraph2D::MySetPixelPINK(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Pink);
}

void CGraph2D::MySetPixelCYAN(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Cyan);
}

void CGraph2D::MySetPixelYELLOW(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Yellow);
}

void CGraph2D::MySetPixelBROWN(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Brown);
}

void CGraph2D::MySetPixelVIOLET(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Violet);
}
void CGraph2D::MySetPixelBLACK(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_BLACK);
}
void CGraph2D::MySetPixelGRAY(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_GRAYCOLOR);
}

void CGraph2D::MySetBox(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_BLACK);
	pDC->SetPixel( lX(X)+1,lY(Y),m_BLACK);
	pDC->SetPixel( lX(X),lY(Y)+1,m_BLACK);
	pDC->SetPixel( lX(X)+1,lY(Y)+1,m_BLACK);
}

void CGraph2D::MySetRedBox(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Red);
	pDC->SetPixel( lX(X)+1,lY(Y),m_Red);
	pDC->SetPixel( lX(X),lY(Y)+1,m_Red);
	pDC->SetPixel( lX(X)+1,lY(Y)+1,m_Red);
}

void CGraph2D::MySetPixelRGB(double X, double Y, int R, int G, int B, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),RGB(R,G,B));
}

void CGraph2D::MySetPixelRGB(int X, int Y, int R, int G, int B, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),RGB(R,G,B));
}


void CGraph2D::MySetPixelRGB(double X, double Y, COLORREF *rgb, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),*rgb);
}

void CGraph2D::ScreenXY(double X, double Y, int &x, int &y)
{
	x=       (lX(X)); //They are in (int) form because it is in screen pixel unit
	y=       (lY(Y)); //They are in (int) form because it is in screen pixel unit
}
void CGraph2D::ScreenXY(double X, double Y, float &x, float &y)
{
	x=   (float) (lX(X)); //They are in (int) form because it is in screen pixel unit
	y=   (float) (lY(Y)); //They are in (int) form because it is in screen pixel unit
}

void CGraph2D::ErasePane_2D( CDC* pDC)  
{
   double  stepX,stepY;
   double L,Lo;

	int nSaveDC=pDC->SaveDC();
	pDC->SelectObject(&m_LightGrayPen);

	stepX=(m_dSpanX)/10.;//Get_Var("Input interval :");
	stepY=(m_dSpanY)/10.;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->Rectangle((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)(m_nTy+m_dGapY),(int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)(m_nBy-m_dGapY+1));
   //----------------------------------Horizontal lines
   for(L=m_dMinY;L<m_dMaxY;L+= stepY){
	   Lo=m_nBy - (L-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY-m_dGapY;
	   pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)Lo);
	   pDC->LineTo((int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)Lo);
   }
   //----------------------------Vertical lines
   for(L=m_dMinX;L<m_dMaxX;L+=stepX){
	   Lo=(L-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_dGapX+m_nTx+SHIFT_TO_X;
	   pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
	   pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));
   }
	pDC->RestoreDC(nSaveDC); //Before deleting the registered resource, you should unregister the resource using RestoreDC(). 
}

void CGraph2D::RepaintPane_2D( CDC* pDC)  
{
   double  stepX,stepY;
   double L,Lo;

	int nSaveDC=pDC->SaveDC();
	pDC->SelectObject(&m_LightGrayPen);

	stepX=(m_dMaxX-m_dMinX)/10.;//Get_Var("Input interval :");
	stepY=(m_dMaxY-m_dMinY)/10.;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
	pDC->SelectObject(&m_bkBrush);  
	pDC->Rectangle(lX(0),lY(2000), lX(2000), lY(0));
 
	//---------------------------Horizontal lines
   for(L=m_dMinY;L<m_dMaxY;L+= stepY){
	   Lo=m_nBy - (L-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY-m_dGapY;
	   pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)Lo);
	   pDC->LineTo((int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)Lo);
   }
   //----------------------------Vertical lines
   for(L=m_dMinX;L<m_dMaxX;L+=stepX){
	   Lo=(L-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_dGapX+m_nTx+SHIFT_TO_X;
	   pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
	   pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));
   }
	pDC->RestoreDC(nSaveDC); //Before deleting the registered resource, you should unregister the resource using RestoreDC(). 
}

void CGraph2D::RefreshPane( CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(
		(int)(m_nTx+m_dGapX+SHIFT_TO_X),
		(int)(m_nTy+m_dGapY),
		(int)((m_nBx-m_dGapX+SHIFT_TO_X+1)-(m_nTx+m_dGapX+SHIFT_TO_X)),
		(int)((m_nBy-m_dGapY)-(m_nTy+m_dGapY)+1),
		pm_memDC,(int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)(m_nTy+m_dGapY),SRCCOPY);
}

void CGraph2D::RefreshFrame( CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(m_nTx,m_nTy,m_nBx-m_nTx,m_nBy-m_nTy,	pm_memDC,m_nTx,m_nTy,SRCCOPY);
}
void CGraph2D::RefreshFrame_OneThird_MiddlePart( CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(m_nTx,m_nTy+0.333*(m_nBy-m_nTy),m_nBx-m_nTx,+0.333*(m_nBy-m_nTy),	pm_memDC,m_nTx,m_nTy+0.333*(m_nBy-m_nTy),SRCCOPY);
}
void CGraph2D::RefreshFrame_3Quarters_MiddlePart( CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(m_nTx,m_nTy+0.1*(m_nBy-m_nTy),m_nBx-m_nTx,+0.9*(m_nBy-m_nTy),	pm_memDC,m_nTx,m_nTy+0.1*(m_nBy-m_nTy),SRCCOPY);
}

void CGraph2D::EraseFrameBlack(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(BLACK_BRUSH);
	pm_memDC->Rectangle(m_nTx,m_nTy,m_nBx,m_nBy);
	pm_memDC->RestoreDC(nSaveDC); 
}
void CGraph2D::EraseFrameWHITE(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(WHITE_BRUSH);
	pm_memDC->Rectangle(m_nTx,m_nTy,m_nBx,m_nBy);
	pm_memDC->RestoreDC(nSaveDC); 
}
void CGraph2D::EraseFrameWHITE_MiddlePart(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(WHITE_BRUSH);
	pm_memDC->Rectangle(m_nTx+5,m_nTy+0.333*(m_nBy-m_nTy),m_nBx-5,m_nTy+0.666*(m_nBy-m_nTy));
	pm_memDC->RestoreDC(nSaveDC); 
}
void CGraph2D::EraseFrameWHITE_3Quarters_MiddlePart(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(WHITE_BRUSH);
	pm_memDC->Rectangle(m_nTx+1,m_nTy+0.1*(m_nBy-m_nTy),m_nBx-1,m_nTy+(m_nBy-m_nTy));
	pm_memDC->RestoreDC(nSaveDC); 
}
#undef Dx
#undef Dy
#undef DoubleGapX
#undef DoubleGapY
#undef Dy_DoubleGapY
#undef Dx_DoubleGapX

