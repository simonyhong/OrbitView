#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<afxtempl.h>
//-----------------------------  Constants --------------------------------------
#define ENTER 0x000d
#define UP    0x4800
#define DOWN  0x5000
#define LEFT  0x4b00
#define RIGHT 0x4d00
#define BkSpace 0x0008
#define ESC   0x001b  //d27
#define PgUp  0x2100
#define PgDn  0x2200
#define F1    0x3b00
#define F2    0x3c00
#define HOME  0x4700
#define End   0x4f00
#define TOLERANCE 1000
#define Scl   100
#define EXIT  3

//----------------------------------------   My_Math.h --------------------------------------------------
#define Sq2D(x,y)   ((x)*(x)+(y)*(y))
#define Sq3D(x,y,z) ((x)*(x)+(y)*(y)+(z)*(z))
#define M3D(x,y,z)  sqrt((x)*(x)+(y)*(y)+(z)*(z))
#define M2D(x,y)    sqrt(((x)*(x)+(y)*(y)))
//-----------------------------------------  My_3D.h  --------------------------------------------------
#define	SQMAGNITUDE3D(x,y,z)       (     (x)*(x) + (y)*(y)+ (z)*(z)     )

#define MAX_MEM 10000

class CMo:public CObject 
{
public:
	double x, y; 
	int  color;
	CMo();
	virtual ~CMo();
};
class CThrMo:public CObject 
{
public:
	double ThrX, ThrY, ThrZ; 
	CThrMo(){};
	virtual ~CThrMo(){};
};

class COrbitViewView;
//######################################  3D  #######################################################
class CThreeD : public CObject
{
public:
	__int64 m_CPUfrq, m_newVal, m_lastVal;
	double HighPerformanceTime(__int64 lastVal);
	void HighPerformanceSleep(double SleepPeriod);
	COrbitViewView* m_pView;
	int		ThreeD (double* , int , int , LPVOID , int );
	int		ThreeD2(double* , int , int , LPVOID , int ,int );
	void    KeyDown(UINT) ;
	///////////  Eye Rotation //////////
	void    HVT_EyePosition(int);			
	void    HVT_EyeRotation();		
	/////////////// Cross for the Eye /////////////////////
	int m_nNumPointsForCross,m_nEndOfIRIS;
	/////////////// Average DC eye positions  /////////////
	float m_AveLH,m_AveLV,m_AveLT, m_AveRH,m_AveRV,m_AveRT;

	CThreeD();
	virtual ~CThreeD();

//	double  coalpha,cozeta,cocsi;
	double  ObjCntrRotX,ObjCntrRotY,ObjCntrRotZ;
	
	/////////// Observer 1  /////////////
	double  ObserverCntrX,ObserverCntrY,ObserverCntrZ;
	double  ObserverCntrRotX,ObserverCntrRotY,ObserverCntrRotZ;//Viewer1's direction vector;
	double  preRot_EgoCtrcCoordt_SrfcPntX, preRot_EgoCtrcCoordt_SrfcPntY, preRot_EgoCtrcCoordt_SrfcPntZ;
	double  EgoCtrcCoordt_SrfcPntX, EgoCtrcCoordt_SrfcPntY, EgoCtrcCoordt_SrfcPntZ;
	double  m_EgoCtrcCoordt_SrfcPnt[MAX_MEM][3];

	/////////// Observer 2  /////////////
	double  SpectatorCntrX,SpectatorCntrY,SpectatorCntrZ;
	double  SpectatorCntrRotX,SpectatorCntrRotY,SpectatorCntrRotZ;//Viewer2's direction vector;
	
	double	ObjCntrd_Coordnt_SrfcPoint[MAX_MEM][3];
	//CMo		ObserverMON[MAX_MEM];
	CArray<CMo, CMo&> ObserverMON;
	double  LtSourceX,LtSourceY,LtSourceZ,ObjCntrX,ObjCntrY,ObjCntrZ;
	int     x0[10],y0[10],x1[10],y1[10];// Light  -->these are used in Erease()
	double  axis[4][3];				 //Coord. axis 
	CPoint  AxisPoint[4];			//Coord. axis  -->these are used in Erease()
	double  Scl_Y,Scl_X,Scl_Z;
	int     m_nScreenOriginX;
	int     m_nScreenOriginY;
	
	// Object rotation along its own axis
	double  e11,e12,e13;
	double  e21,e22,e23;
	double  e31,e32,e33;

	// Object rotation using ROTATION VECTORS
	double  R11,R12,R13;
	double  R21,R22,R23;
	double  R31,R32,R33;
	//ROTATION VECTOR
	double  r1,r2,r3;

	//Viewer rotation 
	double  ev11,ev12,ev13;
	double  ev21,ev22,ev23;
	double  ev31,ev32,ev33;


	int     c,Ego3Dc,old_c,h;
	//double  *ie;//,*e;
	////////////////////////
	BOOL	m_bGoFlg;
	BOOL	m_bIs3D, m_bUsingRetinal2D, m_bDone;
	int		m_nShowMode;
	int     m_nKey; // 
	int CHOICE;
	int     g;
	double  m_dW;
	double	m_dFar;
	int		m_nColorShow;
	CRect	m_rectShow;
	/////////////////////////////////////
	//////////   Eye   //////////////////
	//double m_dlH[MAX_MEM],m_dlV[MAX_MEM],m_dlT[MAX_MEM],m_drH[MAX_MEM],m_drV[MAX_MEM],m_drT[MAX_MEM];
	CArray<double, double&> m_dlH,m_dlV,m_dlT,m_drH,m_drV,m_drT;
	int    m_nSizeOfEachChannel;
	/////////////////////////////////////
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	void    Working(CClientDC*, CClientDC*);         
	void    VisibleArea();						     
	void    Formulas(int );							
	void    Parameters(int );			   	       
	void	SimpleParameters(int Key);
	void	ParametersForObserver(int Key);
	void	ParametersForSpectator();
	void    ObjectRotation();							   
	void    ObjectRotationMatrix();							   
	void	IsoMapping(int*, int*);
	void    ThreeD_to_2D_Mapping();							   
	void    Erase(CClientDC*);					
	void    Show(CClientDC*);                      
	void    CoordinateAxis(CDC*);				
	CArray<double, double& > ie, e; 
	CArray<CMo,CMo&>         mon;
	CArray<CMo,CMo&>         old_mon;

	//////////////////////////////////////////	

	void    Working2(CClientDC*, CClientDC*);         
	void    VisibleArea2();						     
	void    ObjectRotation2();							 
	void    Mapping2();							     
	void    Erase2(CClientDC*);						 
	void    Show2(CClientDC*);
	//////////////////////////////////////////
};

