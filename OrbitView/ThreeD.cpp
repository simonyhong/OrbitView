// ThreeD.cpp : implementation of the CThreeD class

#include "stdafx.h"
#include "stdio.h"
#include "ThreeD.h"
#include "Graph2D.h"
#include "AnalysisDoc.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include <afxwin.h>
#define MY_PEEK_MESSAGE {if(::PeekMessage (&message, NULL,0,0, PM_REMOVE)){::TranslateMessage(&message);::DispatchMessage(&message);}	}		


CMo::CMo():CObject()
{  };

CMo::~CMo()
{  };

CThreeD::CThreeD() : m_rectShow(0,0,800,600)
{
	m_nScreenOriginX =300;
	m_nScreenOriginY =200;
	//GetClientRect(m_rectShow);
	m_nColorShow=BLACK_BRUSH;
	//	 cocsi=0;
	////////////      Switches     /////////////////////////////
	m_bIs3D=0;  m_bUsingRetinal2D=0;  m_bDone=0;
	////////////////////////////////////////////////////////////
	m_nDrawingMode=DOT;
	//m_nKey=STOP;
	m_nKey='A';
	ObjCntrX=0 ;ObjCntrY=0; ObjCntrZ=0;
	ObjCntrRotX=0;   ObjCntrRotY=0;  ObjCntrRotZ=0.;  
	ObserverCntrX=0;  ObserverCntrY=0; ObserverCntrZ=0;	
	ObserverCntrRotX=0; ObserverCntrRotY=0;ObserverCntrRotZ=PIover2;//Turn around to point the object
	SpectatorCntrRotX=-PIover2; SpectatorCntrRotY=0;SpectatorCntrRotZ=0;
	LtSourceX=20;  LtSourceY=5;  LtSourceZ=12; 
	Scl_Y=1;Scl_X=1;Scl_Z=1;
	h=0;
	c=0;     old_c=0;
	g=500;   m_nInitLine=0;
	//ROTATION VECTOR
	r1=0;r2=0.;r3=0.0;

	for(int i=0;i<4;i++){axis[i][0]=0;axis[i][1]=0;axis[i][2]=0;} 

	//ie.SetSize(2,1); e.SetSize(2,1); mon.SetSize(2,1); old_mon.SetSize(2,1); //It's just for safety.
}


CThreeD::~CThreeD()
{ 
	m_bIs3D=0;
	e.RemoveAll();
	mon.RemoveAll();
	old_mon.RemoveAll();
}

//################################################################################################
//################################    3D. Displayer   ############################################
int CThreeD::ThreeD(double* iE, int gValue, int InitLine, LPVOID objView, int disp)
{  
	//////////////////////////////////////////////////////////////////
	CClientDC showDC((CWnd*)objView);										//
	CPen showPen;													//
	showPen.CreateStockObject(BLACK_PEN);							//
	showDC.SelectObject(showPen);									//
	showDC.SetTextColor(RGB(0,255,0));								//
	showDC.SetBkMode(TRANSPARENT);									//
																	//
	CClientDC eraseDC((CWnd*)objView);										//
	CPen erasePen;													//
	erasePen.CreateStockObject(WHITE_PEN);							//
	eraseDC.SelectObject(erasePen);									//
	eraseDC.SetTextColor(RGB(255,255,255));							//
	eraseDC.SetBkMode(TRANSPARENT);									//
	//////////////////////////////////////////////////////////////////
	ObjCntrX=0 ;ObjCntrY=0; ObjCntrZ=0;

	m_nDrawingMode=disp;
	//-------------------------------------------------
	g=gValue;	m_nInitLine=InitLine;
	ie.SetSize(3*g,1); 
	e. SetSize(3*g,1); 
	mon.SetSize(g,1);
	old_mon.SetSize(g,1);
	for(int i=0;i<g; i++){
		ie[3*i  ]=*(iE+3*i  );
		ie[3*i+1]=*(iE+3*i+1); 
		ie[3*i+2]=*(iE+3*i+2);
	}
	//------------------------------------------------
	do{
		if(m_nKey!=VK_SPACE){
 			Parameters(m_nKey);
			Working(&showDC, &eraseDC);
		}
	}while(m_bIs3D);
	//objDC.SetBkMode(OPAQUE);
	return 1;
}

void CThreeD::Working(CClientDC* showDC, CClientDC* eraseDC)
{
	double AlloCtrcCoordt_SrfcPnt[3];//,vv,gr[3],ggr,li[3],lli,l2[3],ll2;)
		    
	ObjectRotation();
	for(int n=0;n<g;n++){
		//if(e[n][0] <TOLERANCE && e[n][1] <TOLERANCE && e[n][2] <TOLERANCE &&e[n][0] >-TOLERANCE && e[n][1] >-TOLERANCE && e[n][2] >-TOLERANCE){
	 	//if(n==200){
		//Delay(200);
		//}
		//Vectors(n);!!!
		AlloCtrcCoordt_SrfcPnt[0]=ObjCntrX+e[n*3]; AlloCtrcCoordt_SrfcPnt[1]=ObjCntrY+e[n*3+1]; AlloCtrcCoordt_SrfcPnt[2]=ObjCntrZ+e[n*3+2];
		preRot_EgoCtrcCoordt_SrfcPntX=AlloCtrcCoordt_SrfcPnt[0]-ObserverCntrX;    preRot_EgoCtrcCoordt_SrfcPntY=AlloCtrcCoordt_SrfcPnt[1]-ObserverCntrY; preRot_EgoCtrcCoordt_SrfcPntZ=AlloCtrcCoordt_SrfcPnt[2]-ObserverCntrZ;
		//vv =M3D(preRot_EgoCtrcCoordt_SrfcPntX,preRot_EgoCtrcCoordt_SrfcPntY,preRot_EgoCtrcCoordt_SrfcPntZ);
		//gr[0]=2*e[n*3]; gr[1]=2*e[n*3+1]; gr[2]=0; ggr=M3D(gr[0],gr[1],gr[2]);
		//li[0]=AlloCtrcCoordt_SrfcPnt[0]-LtSourceX;    li[1]=AlloCtrcCoordt_SrfcPnt[1]-LtSourceY;    li[2]=AlloCtrcCoordt_SrfcPnt[2]-LtSourceZ;  lli=M3D(li[0],li[1],li[2]);
		/*
		if(ggr==0){cozeta=0;l2[0]=li[0];l2[1]=li[1];l2[2]=li[2];ll2=M3D(l2[0],l2[1],l2[2]);
			coalpha=0;cocsi=(l2[0]*(-preRot_EgoCtrcCoordt_SrfcPntX)+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPntY)+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPntZ))/(ll2*vv);
		}
		else {
			cozeta=Divide(-(gr[0]*li[0]+gr[1]*li[1]+gr[2]*li[2]),(ggr*lli));
			l2[0]=li[0]+2*(gr[0]/ggr)*lli*cozeta;
			l2[1]=li[1]+2*(gr[1]/ggr)*lli*cozeta;
			l2[2]=li[2]+2*(gr[2]/ggr)*lli*cozeta;
			ll2=M3D(l2[0],l2[1],l2[2]);
			cocsi=Divide((l2[0]*(-preRot_EgoCtrcCoordt_SrfcPntX)+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPntY)+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPntZ)),(ll2*vv));
			coalpha=Divide((preRot_EgoCtrcCoordt_SrfcPntX*gr[0]+preRot_EgoCtrcCoordt_SrfcPntY*gr[1]+preRot_EgoCtrcCoordt_SrfcPntZ*gr[2]),(ggr*vv));
		}
		*/
		//End of Vectors(n);!!!
		/*
		if(CHOICE==9){          //9: Sphere()
			if(coalpha<=0)	VisibleArea();
		}
		else */VisibleArea();
	//}
	}
//	if(old_c!=0)m_pBbView->m_3D1.EraseFrameBlack(pBbMemDC3D);
	if(m_nDrawingMode!=NETWORK)CoordinateAxis(showDC);
	//Light();
	Show(showDC);
	// if(disp==LINE)Line(c);
	old_c=c;
	c=0;
}

void CThreeD::VisibleArea()
{
/*	if(CHOICE==9){                    //9: Sphere()
		mon[c].color=RGB(0,0,0);
		ObserverRotation();

		if(cozeta>=0){        // lighted area
			mon[c].color=RGB(255,0,0);
			ThreeD_to_2D_Mapping();
			//if(cocsi>=.95){
			//Reflection();
			//}
		}
		else ThreeD_to_2D_Mapping();
	}
	else{
*/		ObserverRotation();
		mon[c].color=RGB(200,0,0);
		ThreeD_to_2D_Mapping();
//	}
}

void CThreeD::Parameters(int Key)
{
	double coswx,coswy,coswz,sinwx,sinwy,sinwz;

	switch(Key){
		// Observer
	//	case VK_LEFT :ObserverCntrX-=1;break;			case VK_RIGHT:ObserverCntrX+=1;break;
	//	case VK_F11  :ObserverCntrY-=0.3;break;			case VK_F12:ObserverCntrY+=0.3;break;
	//	case VK_DOWN :ObserverCntrZ-=1;break;			case VK_UP :ObserverCntrZ+=1;break;
		case VK_F11  :ObserverCntrY-=0.3;break;			case VK_F12:ObserverCntrY+=0.3;break;

		case VK_LEFT :ObserverCntrRotZ+=0.001;break;	case VK_RIGHT:ObserverCntrRotZ-=0.001;break;
		case VK_UP  :ObserverCntrX+=0.1*cos(ObserverCntrRotZ); ObserverCntrY+=0.1*sin(ObserverCntrRotZ); break;		
		case VK_DOWN:ObserverCntrX-=0.1*cos(ObserverCntrRotZ); ObserverCntrY-=0.1*sin(ObserverCntrRotZ); break;	

		// Light Source
		case 'g':LtSourceX+=3;break;			    case 'd':LtSourceX-= 3;break;
		case 'f':LtSourceY+=3;break;				case ' ':LtSourceY-= 3;break;
		//case 'r':LtSourceZ+=3;break;				case 'v':LtSourceZ-= 3;break;
		
		// 
		//case 'u':ObserverCntrRotZ+=.06;break;    		case 'm':ObserverCntrRotZ-=.06;break;
		case 'VK_H':ObserverCntrRotX+=.0006;break;    		case 'VK_K':ObserverCntrRotX-=.0006;break;
		case 'j':ObserverCntrRotY+=.06;break;    		case 'n':ObserverCntrRotY-=.06;break;

		case VK_NUMPAD2:ObjCntrRotX+=0.05;break;      case '*':ObjCntrRotX-=.1;break;
		//case VK_UP:ObjCntrRotY+=0.05;break;      //case VK_DOWN:ObjCntrRotY-=.1;break;
		case VK_HOME:ObjCntrRotZ+=0.01;break;      case VK_END:ObjCntrRotZ-=0.01;break;

		case VK_F2 :Scl_X+=.01;            break;
		case VK_F1 :if(Scl_X>.3)Scl_X-=.01; break;
		case VK_F4 :Scl_Z+=.01;            break;
		case VK_F3 :if(Scl_Z>.3)Scl_Z-=.01; break;
		case VK_F6:Scl_Y+=.01;		    break;
		case VK_F5:if(Scl_Y>.5)Scl_Y-=.01; break;

		default:break;
	}

	coswx=cos(ObjCntrRotX);coswy=cos(ObjCntrRotY);coswz=cos(ObjCntrRotZ);
	sinwx=sin(ObjCntrRotX);sinwy=sin(ObjCntrRotY);sinwz=-sin(ObjCntrRotZ);	
	e11=  coswy*coswz*Scl_X;
	e12=( coswz*sinwy*sinwx+coswx*sinwz)*Scl_Z;
	e13=(-coswz*sinwy*coswx+sinwz*sinwx )*Scl_Y;
	e21= -coswy*sinwz*Scl_X;
	e22=(-sinwz*sinwy*sinwx+coswx*coswz)*Scl_Z;
	e23=( sinwz*sinwy*coswx+coswz*sinwx)*Scl_Y;
	e31=  sinwy*Scl_X;
	e32=(-sinwx*coswy)*Scl_Z;
	e33=  coswx*coswy*Scl_Y;
	
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

// This transformation is REVERSED in Z rotation because the viewer (the Observer) is rotating, 
// making the objects spin the other way.
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

// Object centered rotation! 
//This transformation is NOT REVERSAED in Z rotation because the the object (Observer and its ...) is rotating.
void CThreeD::ParametersForObserver(int Key)
{
	double coswx,coswy,coswz,sinwx,sinwy,sinwz;
	coswx=cos(ObserverCntrRotX);coswy=cos(ObserverCntrRotY);coswz=cos(ObserverCntrRotZ);
	sinwx=sin(ObserverCntrRotX);sinwy=sin(ObserverCntrRotY);sinwz=-sin(ObserverCntrRotZ);
	e11= coswy*coswz;
	e12= coswz*sinwy*sinwx+coswx*sinwz;
	e13=-coswz*sinwy*coswx+sinwz*sinwx;
	e21=-coswy*sinwz;
	e22=-sinwz*sinwy*sinwx+coswx*coswz;
	e23= sinwz*sinwy*coswx+coswz*sinwx;
	e31= sinwy;
	e32=-sinwx*coswy;
	e33= coswx*coswy;
}

void CThreeD::ParametersForSpectator()
{
	double coswx,coswy,coswz,sinwx,sinwy,sinwz;

	coswx=cos(SpectatorCntrRotX);coswy=cos(SpectatorCntrRotY);coswz=cos(SpectatorCntrRotZ);sinwx=sin(SpectatorCntrRotX);sinwy=sin(SpectatorCntrRotY);sinwz=sin(SpectatorCntrRotZ);
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

void CThreeD::ObjectRotation()
{
	int k;
	for(k=0;k<(g);k++){
		e[k*3  ]= ie[k*3]*e11 + ie[k*3+1]*e12 + ie[k*3+2]*e13;
		e[k*3+1]= ie[k*3]*e21 + ie[k*3+1]*e22 + ie[k*3+2]*e23;
		e[k*3+2]= ie[k*3]*e31 + ie[k*3+1]*e32 + ie[k*3+2]*e33;
	}
}

void CThreeD::ObserverRotation()
{
	EgoCtrcCoordt_SrfcPntX= preRot_EgoCtrcCoordt_SrfcPntX*ev11 +preRot_EgoCtrcCoordt_SrfcPntY*ev12 +preRot_EgoCtrcCoordt_SrfcPntZ*ev13;
	EgoCtrcCoordt_SrfcPntY= preRot_EgoCtrcCoordt_SrfcPntX*ev21 +preRot_EgoCtrcCoordt_SrfcPntY*ev22 +preRot_EgoCtrcCoordt_SrfcPntZ*ev23;
	EgoCtrcCoordt_SrfcPntZ= preRot_EgoCtrcCoordt_SrfcPntX*ev31 +preRot_EgoCtrcCoordt_SrfcPntY*ev32 +preRot_EgoCtrcCoordt_SrfcPntZ*ev33;
}

void CThreeD::CoordinateAxis(CDC* pBbMemDC3D)
{
	CPen objPen;
	objPen.CreateStockObject(WHITE_PEN);
	//objDC->SelectObject(objPen);
	m_pBbView->m_memDC3D.SelectObject(objPen);

	int i=0,n,x,y;
	double AlloCtrcCoordt_SrfcPnt[3];
	//double iaxis[4][3];
	
	//Object-Centered axis
//	iaxis[i][0]=0;  iaxis[i][1]=0;  iaxis[i++][2]=0;
//	iaxis[i][0]=10; iaxis[i][1]=0;  iaxis[i++][2]=0;
//	iaxis[i][0]=0;  iaxis[i][1]=10; iaxis[i++][2]=0;
//	iaxis[i][0]=0;  iaxis[i][1]=0;  iaxis[i++][2]=10;
	axis[i][0]=0;  axis[i][1]=0;  axis[i++][2]=0;
	axis[i][0]=10; axis[i][1]=0;  axis[i++][2]=0;
	axis[i][0]=0;  axis[i][1]=10; axis[i++][2]=0;
	axis[i][0]=0;  axis[i][1]=0;  axis[i++][2]=10;

//	for(k=0;k<i;k++){ //BodyRotation , They can be loaded to itself because they are initialized each time of calling CoordinateAxis().
//		axis[k][0]= iaxis[k][0]*e11 +iaxis[k][1]*e12 +iaxis[k][2]*e13;
//		axis[k][1]= iaxis[k][0]*e21 +iaxis[k][1]*e22 +iaxis[k][2]*e23;
//		axis[k][2]= iaxis[k][0]*e31 +iaxis[k][1]*e32 +iaxis[k][2]*e33;
//	}
	#define Tolerance 3
	for(n=0;n<i;n++){         //Working
		AlloCtrcCoordt_SrfcPnt[0]=ObjCntrX+axis[n][0]; AlloCtrcCoordt_SrfcPnt[1]=ObjCntrY+axis[n][1]; AlloCtrcCoordt_SrfcPnt[2]=ObjCntrZ+axis[n][2];
		preRot_EgoCtrcCoordt_SrfcPntX=AlloCtrcCoordt_SrfcPnt[0]-ObserverCntrX;    preRot_EgoCtrcCoordt_SrfcPntY=AlloCtrcCoordt_SrfcPnt[1]-ObserverCntrY;    preRot_EgoCtrcCoordt_SrfcPntZ=AlloCtrcCoordt_SrfcPnt[2]-ObserverCntrZ;
		ObserverRotation();
		if(fabs(EgoCtrcCoordt_SrfcPntY)<Tolerance){
			if(n==0)break;
			continue;
		}
		IsoMapping(&x,&y);   // It only handles the X,Y,Z axies, not the original data.
		AxisPoint[n].x =x; AxisPoint[n].y =y;   
		if(n==0)continue;
		m_pBbView->m_3D.Abs_Graph2D(AxisPoint[0].x,AxisPoint[0].y,9, pBbMemDC3D);
		m_pBbView->m_3D.Abs_Graph2D(AxisPoint[n].x,AxisPoint[n].y,2, pBbMemDC3D);
	}
	for(n=1;n<i;n++){         //Show
		//objDC->MoveTo(AxisPoint[0]);
		//objDC->LineTo(AxisPoint[n]);
	}
}

void CThreeD::ThreeD_to_2D_Mapping()                       // Quick Mapping 
{
	mon[c  ].x=(int)(Scl*EgoCtrcCoordt_SrfcPntX/EgoCtrcCoordt_SrfcPntY);
	mon[c++].y=(int)(Scl*EgoCtrcCoordt_SrfcPntZ/EgoCtrcCoordt_SrfcPntY);
}

void CThreeD::IsoMapping(int* x,int* y)
{
	*x=(int)(Scl*EgoCtrcCoordt_SrfcPntX/EgoCtrcCoordt_SrfcPntY);
	*y=(int)(Scl*EgoCtrcCoordt_SrfcPntZ/EgoCtrcCoordt_SrfcPntY);
}

void CThreeD::Show(CClientDC* objDC)
{
	int Idx;
	char chTemp[5];

	for(Idx=0; Idx<c ; Idx++){
		//objDC->SetPixel(mon[Idx].x, mon[Idx].y ,RGB(255,0,0));
		m_pBbView->m_3D.Abs_Graph2D(mon[Idx].x,mon[Idx].y,5, objDC);
	}
}


void CThreeD::Erase(CClientDC* objDC)
{
}

// The vector from Origin to the origin of the object is   
// -- BB=(ObjCntrX,ObjCntrY,ObjCntrZ) --                                  
// The vector from the Origin to the surface point G(e[n][0],e[n][1],e[n][2]) is 
//  -- TT2=BB+TT=(ObjCntrX+e[n][0],ObjCntrY+e[n][1],ObjCntrZ+e[n][2]);TT2(AlloCtrcCoordt_SrfcPnt[]x,AlloCtrcCoordt_SrfcPnt[]y,AlloCtrcCoordt_SrfcPnt[]z) 
// The observing vector, from V(ObserverCntrX,ObserverCntrY,ObserverCntrZ) to G(e[n][0],e[n][1],e[n][2]) is 
// -- VV(AlloCtrcCoordt_SrfcPnt[]-ObserverCntrX,AlloCtrcCoordt_SrfcPnt[]-ObserverCntrY,AlloCtrcCoordt_SrfcPnt[]-ObserverCntrZ) ;VV(vx,vy,vz) --            
// The gradiant at the point G(e[n][0],e[n][1],e[n][2]) is 
// -- GG=(2*e[n][0], 2*e[n][1], 2*e[n][2]);GG(gr[n][]x,gr[]y,gr[]z) -- 
//  The vector from the light-source to  the point G(e[n][0],e[n][1],e[n][2]) is
// -- LL=(e[n][0]-LtSourceX,e[n][1]-LtSourceY,e[n][2]-(*st??LtSourceZ);LL(lx,li[]y,li[]z) --
// The vector which is reflected and has the same magnitude of LL is 
// -- LL2=LL+2*(GG/gr)*lli*cos(zeta)  ;LL2(l2[]x,l2[]y,l2[]z) --  
// -- cos(zeta) = -((GG @ LL)/(gr*lli)) --                
//  The  cosine,   between  reflected light(LL2)  and  negative observing vector(-VV) is 
// -- cocsi=((LL2 @ -VV)/(ll2*vv)) --                      


/*
void Vectors(double* e,int n)
{
		double ggr,vv,AlloCtrcCoordt_SrfcPnt[3],gr[3],li[3],lli,l2[3],ll2;

		AlloCtrcCoordt_SrfcPnt[0]=ObjCntrX+e[n][0]; AlloCtrcCoordt_SrfcPnt[1]=ObjCntrY+e[n][1]; AlloCtrcCoordt_SrfcPnt[2]=ObjCntrZ+e[n][2];
		preRot_EgoCtrcCoordt_SrfcPntX=AlloCtrcCoordt_SrfcPnt[0]-ObserverCntrX;    preRot_EgoCtrcCoordt_SrfcPntY=AlloCtrcCoordt_SrfcPnt[1]-ObserverCntrY; preRot_EgoCtrcCoordt_SrfcPntZ=AlloCtrcCoordt_SrfcPnt[2]-ObserverCntrZ; vv =M3D(preRot_EgoCtrcCoordt_SrfcPntX,preRot_EgoCtrcCoordt_SrfcPntY,preRot_EgoCtrcCoordt_SrfcPntZ);
		gr[0]=2*e[n][0]; gr[1]=2*e[n][1]; gr[2]=0; ggr=M3D(gr[0],gr[1],gr[2]);
		li[0]=AlloCtrcCoordt_SrfcPnt[0]-LtSourceX;    li[1]=AlloCtrcCoordt_SrfcPnt[1]-LtSourceY;    li[2]=AlloCtrcCoordt_SrfcPnt[2]-LtSourceZ;  lli=M3D(li[0],li[1],li[2]);
	     //	if(n==17){
	     //	  Delay(20);
	     //	}
		if(ggr==0){cozeta=0;l2[0]=li[0];l2[1]=li[1];l2[2]=li[2];ll2=M3D(l2[0],l2[1],l2[2]);
				   coalpha=0;cocsi=(l2[0]*(-preRot_EgoCtrcCoordt_SrfcPntX)+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPntY)+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPntZ))/(ll2*vv);}
		else {
			cozeta=Divide(-(gr[0]*li[0]+gr[1]*li[1]+gr[2]*li[2]),(ggr*lli));
			l2[0]=li[0]+2*(gr[0]/ggr)*lli*cozeta;
			l2[1]=li[1]+2*(gr[1]/ggr)*lli*cozeta;
			l2[2]=li[2]+2*(gr[2]/ggr)*lli*cozeta;
			ll2=M3D(l2[0],l2[1],l2[2]);
			cocsi=Divide((l2[0]*(-preRot_EgoCtrcCoordt_SrfcPntX)+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPntY)+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPntZ)),(ll2*vv));
			coalpha=Divide((preRot_EgoCtrcCoordt_SrfcPntX*gr[0]+preRot_EgoCtrcCoordt_SrfcPntY*gr[1]+preRot_EgoCtrcCoordt_SrfcPntZ*gr[2]),(ggr*vv));
		}
}

  void Reflection()
{
	setcolor(15);
	setfillstyle(SOLID_FILL,15);
	bar( ((mon)+(c-1))->mox,((mon)+(c-1))->moy,((mon)+(c-1))->mox+4,((mon)+(c-1))->moy+4);
	setcolor(0);
	setfillstyle(SOLID_FILL,0);
	bar(((mon)+(c-1))->mox,((mon)+(c-1))->moy,((mon)+(c-1))->mox+4,((mon)+(c-1))->moy+4);
}

 
void Light()
{
	  int ex,ey,ez;
		 int n;
//		 int x,y;

		 setcolor(15);
		 for(n=0;n<10;n++){
			ex=random(2)-1;  ey=random(2)-1;  ez=random(2)-1;
			preRot_EgoCtrcCoordt_SrfcPntX=LtSourceX-ex-ObserverCntrX;   preRot_EgoCtrcCoordt_SrfcPntY=LtSourceY-ey-ObserverCntrY;   preRot_EgoCtrcCoordt_SrfcPntZ=LtSourceZ-ez-ObserverCntrZ;
			if(preRot_EgoCtrcCoordt_SrfcPntY>5 && preRot_EgoCtrcCoordt_SrfcPntY<TOLERANCE){
			   ObserverRotation(sts);
			   x0[n]=getmaxx()/2+500*EgoCtrcCoordt_SrfcPntX/EgoCtrcCoordt_SrfcPntY;
			   y0[n]=getmaxy()/2-500*EgoCtrcCoordt_SrfcPntZ/EgoCtrcCoordt_SrfcPntY;
	  //       IsoMapping(&x,&y);
			   preRot_EgoCtrcCoordt_SrfcPntX=LtSourceX+ex-ObserverCntrX;   preRot_EgoCtrcCoordt_SrfcPntY=LtSourceY+ey-ObserverCntrY;   preRot_EgoCtrcCoordt_SrfcPntZ=LtSourceZ+ez-ObserverCntrZ;
			   ObserverRotation(sts);
			   x1[n]=getmaxx()/2+500*EgoCtrcCoordt_SrfcPntX/EgoCtrcCoordt_SrfcPntY;
			   y1[n]=getmaxy()/2-500*EgoCtrcCoordt_SrfcPntZ/EgoCtrcCoordt_SrfcPntY;
	  //	   IsoMapping(&x,&y);
			   line(x0[n],y0[n],x1[n],y1[n]);
			}
		 }
} 

void Line(int c)
{
   int n;
   setcolor(12);
   for(n=0;n<c-1;n++){           // from dot to dot
	line(((mon)+n)->mox,((mon)+n)->moy,((mon)+n+1)->mox,((mon)+n+1)->moy);
   }
}
*/


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

// ***********************************************************************************************
// ***********************************************************************************************
// ***********************************************************************************************
// ***********************************************************************************************

int CThreeD::ThreeD2(double* iE, int gValue, int InitLine, LPVOID objView, int disp, int ScreenX)
{  
	//////////////////////////////////////////////////////////////////
	CClientDC showDC((CWnd*)objView);								//
	CPen showPen;													//
	showPen.CreateStockObject(BLACK_PEN);							//
	showDC.SelectObject(showPen);									//
	showDC.SetTextColor(RGB(0,255,0));								//
	showDC.SetBkMode(TRANSPARENT);									//
																	//
	CClientDC eraseDC((CWnd*)objView);								//
	CPen erasePen;													//
	erasePen.CreateStockObject(WHITE_PEN);							//
	eraseDC.SelectObject(erasePen);									//
	eraseDC.SetTextColor(RGB(255,255,255));							//
	eraseDC.SetBkMode(TRANSPARENT);									//
	//////////////////////////////////////////////////////////////////
	ObjCntrX=(ScreenX>-100 && ScreenX<200)?ScreenX:200; ObjCntrY=25; ObjCntrZ=10;

	ie.SetSize(2,1); e.SetSize(2,1); mon.SetSize(2,1); old_mon.SetSize(2,1); //It's just for safety.


	m_nDrawingMode=disp;
	//-------------------------------------------------
	g=gValue;	m_nInitLine=InitLine;
//	ie.SetSize(3*g,1); 
	for(int i=0;i<g; i++){
	//	Init_ObjCntrd_Coordnt_SrfcPoint[i][0]=*(iE+3*i  );
	//	Init_ObjCntrd_Coordnt_SrfcPoint[i][1]=*(iE+3*i+1); 
	//	Init_ObjCntrd_Coordnt_SrfcPoint[i][2]=*(iE+3*i+2);
	}
//	e. SetSize(3*g,1); 
//	mon.SetSize(g,1);
//	old_mon.SetSize(g,1);
	//------------------------------------------------
	MSG message;

//	CHOICE=Cho;
	
	do{
		if(m_nKey!=VK_SPACE){
 			Parameters(m_nKey);
			Working2(&showDC, &eraseDC);
		}
		MY_PEEK_MESSAGE;
	}while(m_bIs3D);
	//objDC.SetBkMode(OPAQUE);
	return 1;
}
	
void CThreeD::Working2(CClientDC* showDC, CClientDC* eraseDC)
{
	double AlloCtrcCoordt_SrfcPnt[3];//,vv,gr[3],ggr,li[3],lli,l2[3],ll2;)
		    
	ObjectRotation2();
	for(int n=0;n<g;n++){
		//if(ObjCntrd_Coordnt_SrfcPoint[n][0] <TOLERANCE && ObjCntrd_Coordnt_SrfcPoint[n][1] <TOLERANCE && ObjCntrd_Coordnt_SrfcPoint[n][2] <TOLERANCE &&ObjCntrd_Coordnt_SrfcPoint[n][0] >-TOLERANCE && ObjCntrd_Coordnt_SrfcPoint[n][1] >-TOLERANCE && ObjCntrd_Coordnt_SrfcPoint[n][2] >-TOLERANCE){
	 	//if(n==200){
		//Delay(200);
		//}
		//Vectors(n);!!!
		AlloCtrcCoordt_SrfcPnt[0]=ObjCntrX+ObjCntrd_Coordnt_SrfcPoint[n][0]; AlloCtrcCoordt_SrfcPnt[1]=ObjCntrY+ObjCntrd_Coordnt_SrfcPoint[n][1]; AlloCtrcCoordt_SrfcPnt[2]=ObjCntrZ+ObjCntrd_Coordnt_SrfcPoint[n][2];
		preRot_EgoCtrcCoordt_SrfcPntX=AlloCtrcCoordt_SrfcPnt[0]-ObserverCntrX;    preRot_EgoCtrcCoordt_SrfcPntY=AlloCtrcCoordt_SrfcPnt[1]-ObserverCntrY; preRot_EgoCtrcCoordt_SrfcPntZ=AlloCtrcCoordt_SrfcPnt[2]-ObserverCntrZ;
		//vv =M3D(preRot_EgoCtrcCoordt_SrfcPntX,preRot_EgoCtrcCoordt_SrfcPntY,preRot_EgoCtrcCoordt_SrfcPntZ);
		//gr[0]=2*ObjCntrd_Coordnt_SrfcPoint[n][0]; gr[1]=2*ObjCntrd_Coordnt_SrfcPoint[n][1]; gr[2]=0; ggr=M3D(gr[0],gr[1],gr[2]);
		//li[0]=AlloCtrcCoordt_SrfcPnt[0]-LtSourceX;    li[1]=AlloCtrcCoordt_SrfcPnt[1]-LtSourceY;    li[2]=AlloCtrcCoordt_SrfcPnt[2]-LtSourceZ;  lli=M3D(li[0],li[1],li[2]);
		/*
		if(ggr==0){cozeta=0;l2[0]=li[0];l2[1]=li[1];l2[2]=li[2];ll2=M3D(l2[0],l2[1],l2[2]);
			coalpha=0;cocsi=(l2[0]*(-preRot_EgoCtrcCoordt_SrfcPntX)+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPntY)+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPntZ))/(ll2*vv);
		}
		else {
			cozeta=Divide(-(gr[0]*li[0]+gr[1]*li[1]+gr[2]*li[2]),(ggr*lli));
			l2[0]=li[0]+2*(gr[0]/ggr)*lli*cozeta;
			l2[1]=li[1]+2*(gr[1]/ggr)*lli*cozeta;
			l2[2]=li[2]+2*(gr[2]/ggr)*lli*cozeta;
			ll2=M3D(l2[0],l2[1],l2[2]);
			cocsi=Divide((l2[0]*(-preRot_EgoCtrcCoordt_SrfcPntX)+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPntY)+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPntZ)),(ll2*vv));
			coalpha=Divide((preRot_EgoCtrcCoordt_SrfcPntX*gr[0]+preRot_EgoCtrcCoordt_SrfcPntY*gr[1]+preRot_EgoCtrcCoordt_SrfcPntZ*gr[2]),(ggr*vv));
		}
		*/
		//End of Vectors(n);!!!
		/*
		if(CHOICE==9){          //9: Sphere()
			if(coalpha<=0)	VisibleArea();
		}
		else */VisibleArea2();
	//}
	}
	if(old_c!=0)Erase2(eraseDC);
	if(m_nDrawingMode!=NETWORK)CoordinateAxis(showDC);
	//Light();
	Show2(showDC);
	// if(disp==LINE)Line(c);
	old_c=c;
	c=0;
}

void CThreeD::VisibleArea2()
{
/*	if(CHOICE==9){                    //9: Sphere()
		mon[c].color=RGB(0,0,0);
		ObserverRotation();

		if(cozeta>=0){        // lighted area
			mon[c].color=RGB(255,0,0);
			ThreeD_to_2D_Mapping();
			//if(cocsi>=.95){
			//Reflection();
			//}
		}
		else ThreeD_to_2D_Mapping();
	}
	else{//구가 아니면
*/		ObserverRotation();
		//MON[c].color=RGB(200,0,0);
		Mapping2();
//	}
}

void CThreeD::ObjectRotation2()
{
	int k;
	for(k=0;k<(g);k++){
	//	ObjCntrd_Coordnt_SrfcPoint[k][0]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e11 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e12 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e13;
	//	ObjCntrd_Coordnt_SrfcPoint[k][1]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e21 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e22 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e23;
	//	ObjCntrd_Coordnt_SrfcPoint[k][2]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e31 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e32 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e33;
	}
}

void CThreeD::Mapping2()                       // Quick Mapping 
{
	MON[c  ].x=(int)(m_nScreenOriginX + Scl*EgoCtrcCoordt_SrfcPntX/EgoCtrcCoordt_SrfcPntY);
	MON[c++].y=(int)(m_nScreenOriginY + Scl*EgoCtrcCoordt_SrfcPntZ/EgoCtrcCoordt_SrfcPntY);
}

void CThreeD::Show2(CClientDC* objDC)
{
}

void CThreeD::Erase2(CClientDC* objDC)
{
}


// ***********************************************************************************************
// This is the currently used function.

int CThreeD::ThreeD3(int gValue, int InitLine, int disp)
{
	//////////////////////////////////////////////////////////////////
	CClientDC showDC((CWnd*)m_pBbView);								//
	CDC* pBbMemDC3D=&(m_pBbView->m_memDC3D);						//
//	CPen showPen;													//
	CPen PenForObserver(PS_INSIDEFRAME,1,RGB(255,155,0));				//													//
//	showPen.CreateStockObject(WHITE_PEN);//BLACK_PEN);				//
	showDC.SelectObject(PenForObserver);									//
//	showDC.SetTextColor(RGB(0,255,0));								//
//	showDC.SetBkMode(TRANSPARENT);									//
	//////////////////////////////////////////////////////////////////

	m_nDrawingMode=disp;
	//-------------------------------------------------
	g=gValue;	m_nInitLine=InitLine;
	MSG message;
	CAnalysisDoc* pDoc=(CAnalysisDoc*) ( m_pBbView->GetDocument() );
	int k,n,Color, maxIdx,i,j;
	double RotatedObserverCnteredX,RotatedObserverCnteredY, RotatedObserverCnteredZ;
	double MinDist,temp, tmpX,tmpY,tmpZ,tmpRSQ,OriUnitAct;
#define Mat_Size 200
	int MatxMat=Mat_Size*Mat_Size, ori,Mat_Sizexi;
	int NumComittedVPVOR,FirstTime=1; 
	int EgoFromSpecC, AlloFromSpecC;

 	SimpleParameters(m_nKey);
	do{
		if(m_nKey!=ESC){
			r1=pDoc->m_CblNwL.m_dH;r2=pDoc->m_CblNwL.m_dV;r3=pDoc->m_CblNwL.m_dT;

			/////////////////  (Pane 1),  2D View from the Observer  //////////////
 			if(pDoc->m_bNavigate)SimpleParameters(m_nKey);
			ObjectRotationMatrix();
			c=0;
			for(n=0;n<g;n++){
				preRot_EgoCtrcCoordt_SrfcPntX=(ObjCntrX+ObjCntrd_Coordnt_SrfcPoint[n][0]) -ObserverCntrX;   
				preRot_EgoCtrcCoordt_SrfcPntY=(ObjCntrY+ObjCntrd_Coordnt_SrfcPoint[n][1]) -ObserverCntrY;   
				preRot_EgoCtrcCoordt_SrfcPntZ=(ObjCntrZ+ObjCntrd_Coordnt_SrfcPoint[n][2]) -ObserverCntrZ;
				//Observer Rotation
				m_EgoCtrcCoordt_SrfcPnt[n][0]= preRot_EgoCtrcCoordt_SrfcPntX*ev11 +preRot_EgoCtrcCoordt_SrfcPntY*ev12 +preRot_EgoCtrcCoordt_SrfcPntZ*ev13;
				m_EgoCtrcCoordt_SrfcPnt[n][1]= preRot_EgoCtrcCoordt_SrfcPntX*ev21 +preRot_EgoCtrcCoordt_SrfcPntY*ev22 +preRot_EgoCtrcCoordt_SrfcPntZ*ev23;
				m_EgoCtrcCoordt_SrfcPnt[n][2]= preRot_EgoCtrcCoordt_SrfcPntX*ev31 +preRot_EgoCtrcCoordt_SrfcPntY*ev32 +preRot_EgoCtrcCoordt_SrfcPntZ*ev33;
				//3D -> 2D mapping
				if(m_EgoCtrcCoordt_SrfcPnt[n][1]>1){
					tmpX=(int)(Scl*m_EgoCtrcCoordt_SrfcPnt[n][0]/m_EgoCtrcCoordt_SrfcPnt[n][1]);//ThreeD_to_2D_Mapping
					tmpY=(int)(Scl*m_EgoCtrcCoordt_SrfcPnt[n][2]/m_EgoCtrcCoordt_SrfcPnt[n][1]);
					if(tmpX>99||tmpX<-99||tmpY>99||tmpY<-99)continue;
					ObserverMON[c].x=tmpX; ObserverMON[c].y=tmpY;
					c++;
				}
			}
			m_pBbView->m_3D.EraseFrameWHITE(pBbMemDC3D);
			for(k=0; k<c ; k++){
				m_pBbView->m_3D.Abs_Graph2D(ObserverMON[k].x,ObserverMON[k].y,55, pBbMemDC3D);
			}
			m_pBbView->m_3D.RefreshFrame(&showDC,pBbMemDC3D);
			m_pBbView->m_3D.PutVal_Graph2D(110,40,r1,&showDC);
			m_pBbView->m_3D.PutVal_Graph2D(110,20,r2,&showDC);
			m_pBbView->m_3D.PutVal_Graph2D(110,0.,r3,&showDC);
			//Sleep(100);
			Sleep(m_pBbView->m_nSpeed);
		}
		if(CNw::m_bQuitAll)return 0;
	}while(m_bIs3D);
	return 1;
}

void CThreeD::ObjectRotationMatrix()
{
	int k;
	double tempX,tempY,tempZ;
	double rho=SQMAGNITUDE3D(r1,r2,r3), b=(1-rho)/2., c=2./(1.+rho);
	double R1=Rectify(r1-0.001),R2=Rectify(r2-0.001),R3=Rectify(r3-0.001);	
	R11=c*(R1*R1+b );R12=c*(R1*R2+R3);R13=c*(R1*R3-R2);
	R21=c*(R1*R2-R3);R22=c*(R2*R2+b );R23=c*(R2*R3+R1);
	R31=c*(R1*R3+R2);R32=c*(R2*R3-R1);R33=c*(R3*R3+b );
	for(k=0;k<(g);k++){//Object Rotation
		tempX= ObjCntrd_Coordnt_SrfcPoint[k][0]*R11 + ObjCntrd_Coordnt_SrfcPoint[k][1]*R12 + ObjCntrd_Coordnt_SrfcPoint[k][2]*R13;
		tempY= ObjCntrd_Coordnt_SrfcPoint[k][0]*R21 + ObjCntrd_Coordnt_SrfcPoint[k][1]*R22 + ObjCntrd_Coordnt_SrfcPoint[k][2]*R23;
		tempZ= ObjCntrd_Coordnt_SrfcPoint[k][0]*R31 + ObjCntrd_Coordnt_SrfcPoint[k][1]*R32 + ObjCntrd_Coordnt_SrfcPoint[k][2]*R33;
		ObjCntrd_Coordnt_SrfcPoint[k][0]=tempX;
		ObjCntrd_Coordnt_SrfcPoint[k][1]=tempY;
		ObjCntrd_Coordnt_SrfcPoint[k][2]=tempZ;
	}
}
