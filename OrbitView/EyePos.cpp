// ThreeD.cpp : implementation of the CThreeD class

#include "stdafx.h"
#include "stdio.h"
#include "ThreeD.h"
#include "Graph2D.h"
#include "OrbitViewDoc.h"
#include "OrbitViewView.h"
#include "SinTable.h"
#include <afxwin.h>
#include "Mmsystem.h" 
//In the following m_bIs3D signals if this function is running (m_bIs3D=1) or not (m_bIs3D=0).
void CThreeD::HVT_EyePosition(int gValue)
{
	m_bIs3D=1;
	//////////////////////////////////////////////////////////////////
	CClientDC showDC((CWnd*)m_pView);								//
	CDC* pBbMemDC3D=&(m_pView->m_DC[0]);						//
//	CPen showPen;													//
	CPen PenForObserver(PS_INSIDEFRAME,1,RGB(255,155,0));				//
//	showPen.CreateStockObject(WHITE_PEN);//BLACK_PEN);				//
	showDC.SelectObject(PenForObserver);									//
//	showDC.SetTextColor(RGB(0,255,0));								//
//	showDC.SetBkMode(TRANSPARENT);									//
	//////////////////////////////////////////////////////////////////

	//-------------------------------------------------
	g=gValue;
	ie.SetSize(3*g,1); 
	e.SetSize(3*g,1); 
	double *plH=&(m_pView->m_dLeftH),*plV=&(m_pView->m_dLeftV),*plT=&(m_pView->m_dLeftT), *prH=&(m_pView->m_dRghtH),*prV=&(m_pView->m_dRghtV),*prT=&(m_pView->m_dRghtT);
	int k,n,Color, i,j,refreshCnt=0;
	unsigned startFrom=m_pView->m_startFrom;
	i=startFrom/COrbitViewView::m_dSampleAtEvery_Xms;
	double RotatedObserverCnteredX,RotatedObserverCnteredY, RotatedObserverCnteredZ;
	double tmpX,tmpY,tmpZ;
	float prevSpeed=m_pView->m_nSpeed;
	unsigned  InitialT=GetTickCount();//-COrbitViewView::m_dSampleAtEvery_Xms;
	clock_t start=clock(), finish;
	BOOL ToDoLCblm;
	double fabsObserverCntrY, EyeCenterBias;
	CPen IRIS,IRISx2,IRISx3,IRISx4,IRISx5,IRISx6, GrayPenx1,GrayPenx2,GrayPenx3,GrayPenx4,GrayPenx5,GrayPenx6;
	CPen BLUE_Pen;
	IRIS.CreatePen(PS_SOLID, 2,RGB(110,160,160));
	IRISx2.CreatePen(PS_SOLID, 4,RGB(110,160,160));
	IRISx3.CreatePen(PS_SOLID, 6,RGB(110,160,160));
	IRISx4.CreatePen(PS_SOLID, 8,RGB(110,160,160));
	IRISx5.CreatePen(PS_SOLID, 10,RGB(110,160,160));
	IRISx6.CreatePen(PS_SOLID, 12,RGB(110,160,160));
	GrayPenx1.CreatePen(PS_SOLID, 1,RGB(170,170,170));
	GrayPenx2.CreatePen(PS_SOLID, 3,RGB(170,170,170));
	GrayPenx3.CreatePen(PS_SOLID, 5,RGB(170,170,170));
	GrayPenx4.CreatePen(PS_SOLID, 7,RGB(170,170,170));
	GrayPenx5.CreatePen(PS_SOLID, 9,RGB(170,170,170));
	GrayPenx6.CreatePen(PS_SOLID, 11,RGB(170,170,170));
	BLUE_Pen.CreatePen(PS_SOLID, 3,RGB(90 ,90 ,255));
	CGraph2D *pBd=&(m_pView->m_Board[0]);

	/////////////
	double R1,R2,R3,rho,b,cc;
 	/////////////
	//SimpleParameters(m_nKey);

	//match this with 	timeEndPeriod(1); 
	timeBeginPeriod(1); 
	do{
		if(m_nKey!=ESC){

			//########## Pace the display to the given timing!!#######      
			if(prevSpeed!=m_pView->m_nSpeed){//The speed has changed->Equalize the timing so that there is no catch up.
				InitialT=GetTickCount()-((COrbitViewView::m_dSampleAtEvery_Xms*i-startFrom)*m_pView->m_nSpeed);//Slower speed is like lengthening the time here
				prevSpeed=m_pView->m_nSpeed;
			}
			while(GetTickCount()-InitialT<(COrbitViewView::m_dSampleAtEvery_Xms*i-startFrom)*m_pView->m_nSpeed){//Slower speed is like lengthening the time here
				Sleep(1);// "i" increases by something other than 1. See at the end of this function.
				//The following line tries to increase the frame rate in case there is a lot of need to pause. This will increase the temporal resolution of the 3D image.
				if(m_pView->m_FramesPerSec<m_pView->m_OriginalFrameRate){m_pView->m_FramesPerSec++;	m_pView->m_nSampleRate=m_pView->m_OriginalFrameRate/m_pView->m_FramesPerSec;}
				if(prevSpeed!=m_pView->m_nSpeed){//The speed has changed->Equalize the timing so that there is no catch up.
					InitialT=GetTickCount()-((COrbitViewView::m_dSampleAtEvery_Xms*i-startFrom)*m_pView->m_nSpeed);//Slower speed is like lengthening the time here
					prevSpeed=m_pView->m_nSpeed;
				}
			}
			//The following tries to decrease the frame rate in case the play time of data is 1 COrbitViewView::m_nSampleRate behind of the actual time
			if(GetTickCount()-InitialT>(COrbitViewView::m_dSampleAtEvery_Xms*(i+1*COrbitViewView::m_nSampleRate)-startFrom)*m_pView->m_nSpeed){//Slower speed is like lengthening the time here
				if(m_pView->m_FramesPerSec>20){m_pView->m_FramesPerSec--;m_pView->m_nSampleRate=m_pView->m_OriginalFrameRate/m_pView->m_FramesPerSec;}
			}
			//########################################################

			if(m_pView->m_bPlay){
				//H-axis uses the left hand-screw rule.
				//V, T-ases use right hand-screw rule.
				m_pView->PlotEyeTraces(i); // "i" increases by something other than 1. See at the end of this function.
				*plH=(-(m_dlH[i]-m_AveLH*m_pView->m_SubtractAvg))/57.2958;	// 57.2958 is applied to make the angle in radian
				*plV=(  m_dlV[i]-m_AveLV*m_pView->m_SubtractAvg) /57.2958;	
				*plT=(  m_dlT[i]-m_AveLT*m_pView->m_SubtractAvg) /57.2958;
				*prH=(-(m_drH[i]-m_AveRH*m_pView->m_SubtractAvg))/57.2958;	
				*prV=(  m_drV[i]-m_AveRV*m_pView->m_SubtractAvg) /57.2958;	
				*prT=(  m_drT[i]-m_AveRT*m_pView->m_SubtractAvg) /57.2958;
			}
			else break; //m_pView->m_bPlay==0
			/////////////// Left Eye
			ToDoLCblm=1;
			r3=m_pView->m_dLargeMovement*(*plH);
			r1=m_pView->m_dLargeMovement*(*plV); 
			r2=m_pView->m_dLargeMovement*(*plT);  
			///////// EyeRotation/////////////
forRightEye:	HVT_EyeRotation();
			/////////////////////////////////////////////
			c=0;
			for(n=0;n<g;n++){
				if(!m_pView->m_bFront){// In the ObserverPrmtrs ev11= 1; ev22= 1; ev33=1; and all the others are 0.
					m_EgoCtrcCoordt_SrfcPnt[n][0]= (ObjCntrX+e[n*3+0]) -ObserverCntrX;
					m_EgoCtrcCoordt_SrfcPnt[n][1]= (ObjCntrY+e[n*3+1]) -ObserverCntrY;
					m_EgoCtrcCoordt_SrfcPnt[n][2]= (ObjCntrZ+e[n*3+2]) -ObserverCntrZ;
				}
				else{                    // In the ObserverPrmtrs ev11=-1; ev22=-1; ev33=1; and all the others are 0.
					m_EgoCtrcCoordt_SrfcPnt[n][0]=-(ObjCntrX+e[n*3+0]) +ObserverCntrX;
					m_EgoCtrcCoordt_SrfcPnt[n][1]=-(ObjCntrY+e[n*3+1]) +ObserverCntrY;
					m_EgoCtrcCoordt_SrfcPnt[n][2]= (ObjCntrZ+e[n*3+2]) -ObserverCntrZ;
				}
				//3D -> 2D mapping
				#define MY_BLACK 55
				#define MY_IRIS  57
				#define MY_GRAY 56
				fabsObserverCntrY=fabs(ObserverCntrY);
				if(m_EgoCtrcCoordt_SrfcPnt[n][1]>1){
					// Look, Y-AXIS is the GAZE axis!
					tmpX=(Scl*m_EgoCtrcCoordt_SrfcPnt[n][0]/m_EgoCtrcCoordt_SrfcPnt[n][1]);
					tmpY=(Scl*m_EgoCtrcCoordt_SrfcPnt[n][2]/m_EgoCtrcCoordt_SrfcPnt[n][1]);
					if(tmpX>99||tmpX<-99||tmpY>99||tmpY<-99)continue;
					ObserverMON[c].x=tmpX; ObserverMON[c].y=tmpY;
					if(m_EgoCtrcCoordt_SrfcPnt[n][1]>fabsObserverCntrY)ObserverMON[c].color=MY_GRAY ; 
					else                                               ObserverMON[c].color=MY_BLACK;
					c++;
				}
			}
			if(ToDoLCblm){
				/// Dispaly left eye
				pBbMemDC3D->SelectObject(pBd->m_Null_Pen);
				if(m_pView->m_bIsToShowHead==0)pBd->EraseFrameWHITE_MiddlePart(pBbMemDC3D);
				else                           pBd->EraseFrameWHITE_3Quarters_MiddlePart(pBbMemDC3D);

				if(m_pView->m_bIsToShowHead==0){
					//The reference lines
					pBbMemDC3D->SelectObject(m_pView->m_BLUE);
					// +
					// x
					pBd->Graph2D(-44,0  , -35, 0,2, pBbMemDC3D); pBd->Graph2D(-40,-5,-40,5,2, pBbMemDC3D);
					pBd->Graph2D( 36 ,0  , 45, 0,2, pBbMemDC3D); pBd->Graph2D( 40,-5, 40,5,2, pBbMemDC3D);
				}
				//##### Dispaly left eye
				if(m_pView->m_isLeftEye || m_pView->m_bIsToShowHead==1){
					// Eye
					if(!m_pView->m_bFront){
						if(m_pView->m_bIsToShowHead==0)EyeCenterBias=-40; else EyeCenterBias=0;
						if(m_pView->m_dScaleUpDn<=0.3)pBbMemDC3D->SelectObject(GrayPenx1);    
						else if(m_pView->m_dScaleUpDn<=0.5)pBbMemDC3D->SelectObject(GrayPenx2);    
						else if(m_pView->m_dScaleUpDn<=1.0)pBbMemDC3D->SelectObject(GrayPenx3);    
						else if(m_pView->m_dScaleUpDn<=1.4)pBbMemDC3D->SelectObject(GrayPenx4);    
						else if(m_pView->m_dScaleUpDn<=1.8)pBbMemDC3D->SelectObject(GrayPenx5);    
						else	pBbMemDC3D->SelectObject(GrayPenx6);    
					}
					else{
						if(m_pView->m_bIsToShowHead==0)EyeCenterBias=+40; else EyeCenterBias=0;
						if(m_pView->m_dScaleUpDn<=0.3)pBbMemDC3D->SelectObject(IRIS);    
						else if(m_pView->m_dScaleUpDn<=0.5)pBbMemDC3D->SelectObject(IRISx2);    
						else if(m_pView->m_dScaleUpDn<=1.0)pBbMemDC3D->SelectObject(IRISx3);    
						else if(m_pView->m_dScaleUpDn<=1.4)pBbMemDC3D->SelectObject(IRISx4);    
						else if(m_pView->m_dScaleUpDn<=1.8)pBbMemDC3D->SelectObject(IRISx5);    
						else	pBbMemDC3D->SelectObject(IRISx6);    
					}
					/////////////// Sclera //////////////////
					if(!m_pView->m_bFront)for(k=m_nNumPointsForCross; k<c ; k++)if(ObserverMON[k].color==MY_GRAY)pBd->MySetPixelGRAY(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y,pBbMemDC3D);
					for(k=m_nNumPointsForCross; k<c ; k++){
						if(ObserverMON[k].color!=MY_GRAY)pBd->MySetPixelBLACK(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y,pBbMemDC3D);
					}
					if(m_pView->m_bIsToShowHead==0){
						/////////////// IRIS  ///////////////////
						k=m_nNumPointsForCross; pBd->MyMoveTo(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y, pBbMemDC3D);
						for(k=m_nNumPointsForCross+1; k<m_nEndOfIRIS; k++)pBd->MyLineTo(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y, pBbMemDC3D);
						/////////Cross for the Eye///////
						for(k=9; k<m_nNumPointsForCross ; k++)
							if(ObserverMON[k].color!=MY_GRAY)pBd->MySetPixelBLACK(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y, pBbMemDC3D);
					}
				}
				ToDoLCblm=0;
				//#################################### Right #####################################
				r3=m_pView->m_dLargeMovement*(*prH);
				r1=m_pView->m_dLargeMovement*(*prV); 
				r2=m_pView->m_dLargeMovement*(*prT);  
				goto forRightEye;
			}
			//##### Dispaly Right eye
			if(m_pView->m_isRightEye && m_pView->m_bIsToShowHead==0){//Draw left eye
				/// Eye
				if(!m_pView->m_bFront){
					if(m_pView->m_bIsToShowHead==0)EyeCenterBias=+40; else EyeCenterBias=0;
					if(m_pView->m_dScaleUpDn<=0.3)pBbMemDC3D->SelectObject(GrayPenx1);    
					else if(m_pView->m_dScaleUpDn<=0.5)pBbMemDC3D->SelectObject(GrayPenx2);    
					else if(m_pView->m_dScaleUpDn<=1.0)pBbMemDC3D->SelectObject(GrayPenx3);    
					else if(m_pView->m_dScaleUpDn<=1.4)pBbMemDC3D->SelectObject(GrayPenx4);    
					else if(m_pView->m_dScaleUpDn<=1.8)pBbMemDC3D->SelectObject(GrayPenx5);    
					else	pBbMemDC3D->SelectObject(GrayPenx6);    
				}
				else{
					if(m_pView->m_bIsToShowHead==0)EyeCenterBias=-40; else EyeCenterBias=0;
					if(m_pView->m_dScaleUpDn<=0.3)pBbMemDC3D->SelectObject(IRIS);    
					else if(m_pView->m_dScaleUpDn<=0.5)pBbMemDC3D->SelectObject(IRISx2);    
					else if(m_pView->m_dScaleUpDn<=1.0)pBbMemDC3D->SelectObject(IRISx3);    
					else if(m_pView->m_dScaleUpDn<=1.4)pBbMemDC3D->SelectObject(IRISx4);    
					else if(m_pView->m_dScaleUpDn<=1.8)pBbMemDC3D->SelectObject(IRISx5);    
					else	pBbMemDC3D->SelectObject(IRISx6);    
				}
				/////////////// Sclera //////////////////
				if(!m_pView->m_bFront)for(k=m_nNumPointsForCross; k<c ; k++)if(ObserverMON[k].color==MY_GRAY)pBd->MySetPixelGRAY(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y, pBbMemDC3D);
				for(k=m_nNumPointsForCross; k<c ; k++)if(ObserverMON[k].color!=MY_GRAY)pBd->MySetPixelBLACK(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y, pBbMemDC3D);
				if(m_pView->m_bIsToShowHead==0){
					//////////////////  IRIS   ///////////////////
					k=m_nNumPointsForCross; pBd->MyMoveTo(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y, pBbMemDC3D);
					for(k=m_nNumPointsForCross+1; k<m_nEndOfIRIS; k++)pBd->MyLineTo(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y, pBbMemDC3D);
					/////////Cross for the Eye///////
					for(k=9; k<m_nNumPointsForCross ; k++){
						if(ObserverMON[k].color!=MY_GRAY)pBd->MySetPixelBLACK(ObserverMON[k].x+EyeCenterBias,ObserverMON[k].y,pBbMemDC3D);
					}
				}
			}

			if(refreshCnt++%10==0)pBd->RefreshFrame(&showDC,pBbMemDC3D);//Every once in a while, refresh everything
			else if(m_pView->m_bIsToShowHead==0)pBd->RefreshFrame_OneThird_MiddlePart(&showDC,pBbMemDC3D);
			else pBd->RefreshFrame_3Quarters_MiddlePart(&showDC,pBbMemDC3D);//Every once in a while, refresh everything

		}
		//Pause
		#define PauseUnit 10
		while(m_pView->m_bPause){Sleep(PauseUnit); InitialT+=PauseUnit; if(!m_pView->m_bPlay)break;}
		if(!m_pView->m_bPlay)break;

		if((i+=COrbitViewView::m_nSampleRate)>=(m_nSizeOfEachChannel-2*COrbitViewView::m_nSampleRate))break;
		if(m_pView->m_endAt<=COrbitViewView::m_dSampleAtEvery_Xms*(i+1*COrbitViewView::m_nSampleRate))break;
	}while(1);
	timeEndPeriod(1);//match this with 	timeBeginPeriod(1); 
	char ToPrint[100];
	//match this with 	timeEndPeriod(1); 
	finish=clock(); 
	double duration=(double)(finish-start)/CLOCKS_PER_SEC; sprintf(ToPrint,"Duration:%0.2lf", duration);showDC.TextOut(1100,250,ToPrint);
	m_bIs3D=0;
}

//Euler Vector.
//This solution was taken from Wikipedia: Rotation matrix given an axis and an angle
void CThreeD::HVT_EyeRotation()
{
	int k;
	double u1, u2, u3, c, s, RotAmp=sqrt(r1*r1+r2*r2+r3*r3);
	u1=Divide(r1,RotAmp); u2=Divide(r2,RotAmp); u3=Divide(r3,RotAmp); 
	s=sin(RotAmp); c=cos(RotAmp);

	//Rotation along the unit vector (u1,u2, u3) with amplitude of angle RotAmp
	R11=u1*u1+(1-u1*u1)*c; R12=u1*u2*(1-c)-u3*s;  R13=u1*u3*(1-c)+u2*s;
	R21=u1*u2*(1-c)+u3*s;  R22=u2*u2+(1-u2*u2)*c; R23=u2*u3*(1-c)-u1*s;
	R31=u1*u3*(1-c)-u2*s;  R32=u2*u3*(1-c)+u1*s;  R33=u3*u3+(1-u3*u3)*c;
	for(k=0;k<g;k++){//Object Rotation
		e[k*3+0]= R11*ObjCntrd_Coordnt_SrfcPoint[k][0] + R12*ObjCntrd_Coordnt_SrfcPoint[k][1] + R13*ObjCntrd_Coordnt_SrfcPoint[k][2];
		e[k*3+1]= R21*ObjCntrd_Coordnt_SrfcPoint[k][0] + R22*ObjCntrd_Coordnt_SrfcPoint[k][1] + R23*ObjCntrd_Coordnt_SrfcPoint[k][2];
		e[k*3+2]= R31*ObjCntrd_Coordnt_SrfcPoint[k][0] + R32*ObjCntrd_Coordnt_SrfcPoint[k][1] + R33*ObjCntrd_Coordnt_SrfcPoint[k][2];
	}
}

////This one is NOT correct!! ==> depending on which axis i rotated first, the final position varies.
////The following program uses Ternions (Mutze 2005) to calculate the rotation matrix.
//void CThreeD::HVT_EyeRotation()// --> Fix this one to a faster one. 
//{
//	int k;
//	double R1,R2,R3;
//	double rho, b, c;
//
//	//Horizontal: Along the Z axis of the eyeball
//	R1=ObjCntrd_Coordnt_SrfcPoint[2][0]*r3; R2=ObjCntrd_Coordnt_SrfcPoint[2][1]*r3; R3=ObjCntrd_Coordnt_SrfcPoint[2][2]*r3;//=>The same as: R1=0;R2=0;R3=r3; 
//	rho=SQMAGNITUDE3D(R1,R2,R3); b=(1-rho)/2.; c=2./(1.+rho);
//	R11=c*(R1*R1+b );R12=c*(R1*R2+R3);R13=c*(R1*R3-R2);
//	R21=c*(R1*R2-R3);R22=c*(R2*R2+b );R23=c*(R2*R3+R1);
//	R31=c*(R1*R3+R2);R32=c*(R2*R3-R1);R33=c*(R3*R3+b );
//	for(k=0;k<g;k++){//Object Rotation
//		e[k*3+0]= R11*ObjCntrd_Coordnt_SrfcPoint[k][0] + R12*ObjCntrd_Coordnt_SrfcPoint[k][1] + R13*ObjCntrd_Coordnt_SrfcPoint[k][2];
//		e[k*3+1]= R21*ObjCntrd_Coordnt_SrfcPoint[k][0] + R22*ObjCntrd_Coordnt_SrfcPoint[k][1] + R23*ObjCntrd_Coordnt_SrfcPoint[k][2];
//		e[k*3+2]= R31*ObjCntrd_Coordnt_SrfcPoint[k][0] + R32*ObjCntrd_Coordnt_SrfcPoint[k][1] + R33*ObjCntrd_Coordnt_SrfcPoint[k][2];
//	}
//	//Vertical: Along the X axis of the rotated eyeball: e[0],e[1],e[2] represents the rotated X axis
//	R1=e[0]*r1;R2=e[1]*r1;R3=e[2]*r1; 
//	rho=SQMAGNITUDE3D(R1,R2,R3); b=(1-rho)/2.; c=2./(1.+rho);
//	R11=c*(R1*R1+b );R12=c*(R1*R2+R3);R13=c*(R1*R3-R2);
//	R21=c*(R1*R2-R3);R22=c*(R2*R2+b );R23=c*(R2*R3+R1);
//	R31=c*(R1*R3+R2);R32=c*(R2*R3-R1);R33=c*(R3*R3+b );
//	for(k=0;k<g;k++){//Object Rotation
//		ie[k*3+0]= R11*e[k*3+0] + R12*e[k*3+1] + R13*e[k*3+2];
//		ie[k*3+1]= R21*e[k*3+0] + R22*e[k*3+1] + R23*e[k*3+2];
//		ie[k*3+2]= R31*e[k*3+0] + R32*e[k*3+1] + R33*e[k*3+2];
//	}
//	//Tortional: Along the Y axis of the rotated eyeball: ie[0],ie[1],ie[2] represents the rotated Y axis
//	R1=ie[3]*r2;R2=ie[4]*r2;R3=ie[5]*r2;
//	rho=SQMAGNITUDE3D(R1,R2,R3); b=(1-rho)/2.; c=2./(1.+rho);
//	R11=c*(R1*R1+b );R12=c*(R1*R2+R3);R13=c*(R1*R3-R2);
//	R21=c*(R1*R2-R3);R22=c*(R2*R2+b );R23=c*(R2*R3+R1);
//	R31=c*(R1*R3+R2);R32=c*(R2*R3-R1);R33=c*(R3*R3+b );
//	for(k=0;k<g;k++){//Object Rotation
//		e[k*3+0]= R11*ie[k*3+0] + R12*ie[k*3+1] + R13*ie[k*3+2];
//		e[k*3+1]= R21*ie[k*3+0] + R22*ie[k*3+1] + R23*ie[k*3+2];
//		e[k*3+2]= R31*ie[k*3+0] + R32*ie[k*3+1] + R33*ie[k*3+2];
//	}
//}

void CThreeD::KeyDown(UINT nChar) 
{
	if(nChar==34){  //PgDown 
		if(0.05>0.002)m_dW-=.001;
		if(0.1>0.1)m_dFar-=0.01;
		return;
	}
	if(nChar==33){  //PgUp
		if(0.05<0.2)m_dW+=.001;
		if(0.1<3.)m_dFar+=0.01;
		return;
	}
	m_nKey=nChar;
}

void CThreeD::SimpleParameters(int Key) 
{
	double coswx,coswy,coswz,sinwx,sinwy,sinwz;
#define StepSize 0.01
#define dPhi 0.001

	switch(Key){
		case VK_UP  :ObserverCntrX-=StepSize*sin(ObserverCntrRotZ); ObserverCntrY+=StepSize*cos(ObserverCntrRotZ); break;		
		case VK_DOWN:ObserverCntrX+=StepSize*sin(ObserverCntrRotZ); ObserverCntrY-=StepSize*cos(ObserverCntrRotZ); break;	

		case VK_LEFT :ObserverCntrRotZ+=dPhi;break;		case VK_RIGHT:ObserverCntrRotZ-=dPhi;break;
		case 'VK_H'  :ObserverCntrRotX+=.0006;break;   	case 'VK_K':ObserverCntrRotX-=.0006;break;
		case 'j'     :ObserverCntrRotY+=.06;break;  	case 'n':ObserverCntrRotY-=.06;break;

		default:break;
	}
	coswx=cos(ObserverCntrRotX);coswy=cos(ObserverCntrRotY);coswz=cos(ObserverCntrRotZ);
	sinwx=sin(ObserverCntrRotX);sinwy=sin(ObserverCntrRotY);sinwz=sin(ObserverCntrRotZ);
	ev11= coswy*coswz;
	ev12= coswz*sinwy*sinwx+coswx*sinwz;
	ev13=-coswz*sinwy*coswx+sinwz*sinwx;
	ev21=-coswy*sinwz;
	ev22=-sinwz*sinwy*sinwx+coswx*coswz;
	ev23= sinwz*sinwy*coswx+coswz*sinwx;
	ev31= sinwy;
	ev32=-sinwx*coswy;
	ev33= coswx*coswy;
}

CThreeD::CThreeD()
{
	m_bIs3D=0;
}
CThreeD::~CThreeD()
{
	e.RemoveAll();
	ie.RemoveAll();
	mon.RemoveAll();
	old_mon.RemoveAll();
	ObserverMON.RemoveAll();
	m_dlH.RemoveAll();
	m_dlV.RemoveAll();
	m_dlT.RemoveAll();
	m_drH.RemoveAll();
	m_drV.RemoveAll();
	m_drT.RemoveAll();

}


CMo::CMo():CObject()
{  };

CMo::~CMo()
{  };