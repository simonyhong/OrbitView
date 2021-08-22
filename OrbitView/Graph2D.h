#include "VarDefinitions.h"

#define xFRAME 0
#define oFRAME 1
#define PERI 2
#define xSCALE 0
#define oSCALE 1
#define MaxTICS 100
#define ButtonWidth 70
#define ButtonHight 13
#define SmallButtonWidth 10
#define SmallButtonHight 6

class CGraph2D: public CObject
{
public:
	int	   m_IsBlownUp,m_IsTickY; //To blow up the window
	int	   m_nTx,m_nTy,m_nBx,m_nBy, m_nSavedTx,m_nSavedTy,m_nSavedBx,m_nSavedBy, m_BlownUpTx,m_BlownUpTy,m_BlownUpBx,m_BlownUpBy, m_normTx,m_normTy,m_normBx,m_normBy;
	int	   m_nPaneTx,m_nPaneTy,m_nPaneBx,m_nPaneBy;
	int		m_page,m_NumPages;
	int		m_howManyY,m_howManyX,m_IsLabeledTicX[MaxTICS],m_IsLabeledTicY[MaxTICS];
	float	m_YTics[MaxTICS],m_XTics[MaxTICS];
	char   m_cXaxis[MaxDC][100], m_cYaxis[MaxDC][100];
	int		m_nYlabelPosX,m_nYlabelPosY;
	char   m_cWhatever[100];
	double m_dMinX, m_dMaxX, m_dMinY, m_dMaxY, m_dMeanY, m_dStepX,m_dStepY,m_dSpanX, m_dSpanY;
	double m_dGapX,m_dGapY,m_nBy_GapY;
	double m_dDy_DoubleGapY_Per_Max_MinY;
	double m_dDx_DoubleGapX_Per_Max_MinX;
	int m_nTransX,m_LateralMargin;
	POINT m_vert[3]; //For triangle

	CPen m_PinkPen,m_OrangePen,m_RedPen,m_BluePen,m_GreenPen,m_BlackPen3,m_GrayPen,m_LightGrayPen,m_Null_Pen;
	CBrush m_bkBrush,m_GrayBrush,m_LightGrayBrush;

	double m_dLabelBias,m_dLabelScale;
	COLORREF m_Red, m_Blue, m_Green,m_DkGreen, m_Pink, m_Cyan, m_Yellow,m_Brown,m_Violet,m_BLACK,m_GRAYCOLOR;
	static BOOL m_bUsingDC,m_bUsingDC2, m_bUsingDC3;
	BOOL m_bDrawFRAME;

	CGraph2D();
	CGraph2D(int Tx, int Ty, int Bx, int By, char Xaxis[100], char Yaxis[100]);
	virtual ~CGraph2D();
	void SetLabelsXY(char Xaxis[100], char Yaxis[100]);
	void FromDataToScreenXY(double X, double Y, int &x, int &y);
	void SetFig_Pos_n_Labels_XY(int Tx, int Ty, int Bx, int By, char Xaxis[100], char Yaxis[100]);
	void Set_MinMax_n_Labels_XY(char Xaxis[100], char Yaxis[100],double Min_X, double Max_X, double Min_Y, double Max_Y, int oxFRAME);
	void SetFig_Positions_XY(int Tx, int Ty, int Bx, int By);
	void Frame_Graph2D(double Min_X, double Max_X, double Min_Y, double Max_Y, CDC* pDC, int oxFRAME);
	void Frame_Graph2D(CDC* pDC);
	void formal_Frame2D(CDC* pDC, int oxFRAME);
	void Graph2D(double X1, double Y1, double X, double Y,int Disp, CDC* pDC);
	void Graph2D(double , double ,int , CDC* ); 
	void Triangle(double X, double Y,int size,CDC* pDC);
	void Circle(double X, double Y,int size,CDC* pDC);
	void ErrorBar(double X, double Y, double error, CDC* pDC);
	void ErrorWing(double X, double Y, double error, CDC* pDC);
	void peri_Histogram(double X, double Y,int BinWidth, CDC* pDC);
	void Histogram4(double X, double Y, int BinWidth,int Nth, CDC* pDC);
	void Histogram4(double X, double Y, float BinWidth,int Nth, CDC* pDC);
	void peri_Hist_Horizontal(double X, double Y,int BinWidth, CDC* pDC);
	void PutIntVal_Graph2D(double X, double Y, int NumToPrint, CDC* pDC);
	void PutIntVal_Custom_Graph2D(double X, double Y, int NumToPrint, CDC* pDC);
	void PutVal_p5(double X, double Y, double NumToPrint, CDC* pDC);
	void PutVal_Graph2D(double X, double Y, double NumToPrint, CDC* pDC);
	void PutChar_Graph2D(double X, double Y,char ToPrint[], CDC* pDC);
	void PutChar_After10(double X, double Y,char ToPrint[], CDC* pDC);
	void MyMoveTo(double X, double Y,CDC* pDC);
	void MyLineTo(double X, double Y,CDC* pDC);
	void MySetPixel(double X, double Y, int GRAY, CDC* pDC);
	void MySetPixelRED(double X, double Y, CDC* pDC);
	void MySetPixelGREEN(double X, double Y, CDC* pDC);
	void MySetPixelDKGRN(double X, double Y, CDC* pDC);
	void MySetPixelBLUE(double X, double Y, CDC* pDC);
	void MySetPixelYELLOW(double X, double Y, CDC* pDC);
	void MySetPixelPINK(double X, double Y, CDC* pDC);
	void MySetPixelCYAN(double X, double Y, CDC* pDC);
	void MySetPixelBROWN(double X, double Y, CDC* pDC);
	void MySetPixelVIOLET(double X, double Y, CDC* pDC);
	void MySetPixelBLACK(double X, double Y, CDC* pDC);
	void MySetPixelGRAY(double X, double Y, CDC* pDC);
	void MySetPixelRGB(double X, double Y, COLORREF *rgb, CDC* pDC);
	void MySetPixelRGB(int X, int Y, int R, int G, int B, CDC* pDC);
	void MySetPixelRGB(double X, double Y, int R, int G, int B, CDC* pDC);
	void MySetBox(double X, double Y, CDC* pDC);
	void MySetRedBox(double X, double Y, CDC* pDC);
	void FillSolidRect(double tlX, double tlY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect_CenteredY(double X, double Y,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect(int tlX, int tlY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect13x13(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FillSolidRect8x8(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FillSolidRect7x7(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void ScreenXY(double X, double Y, int &x, int &y);
	void ScreenXY(double X, double Y, float &x, float &y);
	void ErasePane_2D( CDC* pDC);
	void Abs_Ellipse(double X1, double Y1,int size, CDC* pDC);
	void EraseFrameBlack(CDC* pm_memDC);
	void EraseFrameWHITE(CDC* pm_memDC);
	void EraseFrameWHITE_MiddlePart(CDC* pm_memDC);
	void EraseFrameWHITE_3Quarters_MiddlePart(CDC* pm_memDC);
	void RectangleAB(double X1, double Y1, double X2, double Y2, CDC* pDC); 
	void RepaintPane_2D( CDC* pDC); 
	void RefreshPane( CDC* objDC, CDC* pm_memDC);
	void RefreshFrame( CDC* objDC, CDC* pm_memDC);
	void RefreshFrame_OneThird_MiddlePart( CDC* objDC, CDC* pm_memDC);
	void RefreshFrame_3Quarters_MiddlePart( CDC* objDC, CDC* pm_memDC);
};