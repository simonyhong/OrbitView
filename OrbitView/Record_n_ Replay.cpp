#include "stdafx.h"
#include "OrbitView.h"

#include "Graph2D.h"
#include "ThreeD.h"
#include "OrbitViewDoc.h"
#include "OrbitViewView.h"
#include "SinTable.h"

UINT PLAY(LPVOID WinObjPtr)
{
	COrbitViewView* pBbView=(COrbitViewView*) WinObjPtr;

	pBbView->Play();

	return 0;
}

//Newly paly
void COrbitViewView::OnPlay() // |>
{
	if(!m_bDataLoaded){OnPlayFromFile(); return;}
	m_bPlay=0; //Quit all the functions.
	Sleep(100);//while(m_IsExecute||ThD.m_bIs3D); //Make sure that everything has stopped.

	/////////  Multi-Threads  ////////////
	AfxBeginThread(PLAY, this);
}

void COrbitViewView::OnUpdatePlay(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bPlay);
}

//This one replays the eye posions and related parameters recorded by the user during the current run OR from a file.
void COrbitViewView::Play() // |>
{	
	VirMemDC();
	ThD.m_pView=this;
	m_nShowTimeStart=0;m_nShowTimeEnd=10000; m_prvTime=0; 
	SetGraphs();
	//////////////////////////
	//////////////////////////
	int g;
	if(m_bIsToShowHead==0)g=EyeBall();
	else g=Head_Shape();
	if(g==0){AfxMessageBox("Cound not find the \"Head_Shape.txt\" file."); return;}

	//////////////////////////
	//////////////////////////
	ThD.ObjCntrX=0 ;ThD.ObjCntrY=0; ThD.ObjCntrZ=0;
	ThD.ObserverCntrRotX=0; ThD.ObserverCntrRotY=0;ThD.ObserverCntrRotZ=0;
	Observer_Origin();

	/////////////////////////////////////////////////////////////////////////////////////////

	m_bPlay=1;
	ThD.HVT_EyePosition(g);// This function is defined in "ThreeD.h" and implemented in EyePos.cpp.
	m_bPlay=0;
}

//###########################################################################################################################################################################
//###########################################################################################################################################################################

//This is called from the 3D module
void COrbitViewView::PlotEyeTraces(int FrameNum)
{ 
	CClientDC aDC(this);

	int nowT=FrameNum*m_dSampleAtEvery_Xms;
	if(m_prvTime<=0){//If this is the first point, just register.
		m_prvTime=nowT;
		m_prevlH=ThD.m_dlH[FrameNum]-m_avelH*m_SubtractAvg+m_d4FifthOrdinateAxis;
		m_prevlV=ThD.m_dlV[FrameNum]-m_avelV*m_SubtractAvg+m_dHalfOrdinateAxis;
		m_prevlT=ThD.m_dlT[FrameNum]-m_avelT*m_SubtractAvg+m_d1FifthOrdinateAxis;
		m_prevrH=ThD.m_drH[FrameNum]-m_averH*m_SubtractAvg+m_d4FifthOrdinateAxisPlus;
		m_prevrV=ThD.m_drV[FrameNum]-m_averV*m_SubtractAvg+m_dHalfOrdinateAxisPlus;
		m_prevrT=ThD.m_drT[FrameNum]-m_averT*m_SubtractAvg+m_d1FifthOrdinateAxisPlus;
		for(int i=0;i<m_howmanyAuxVectors;i++){m_prvAux[i]=m_AuxVec[FrameNum]-m_avgAux[i]*m_SubtractAvg;}
		return;
	}
	if(m_prvTime>=m_nShowTimeEnd){int tmpTime; tmpTime=m_nShowTimeEnd; m_nShowTimeEnd+=(m_nShowTimeEnd-m_nShowTimeStart); m_nShowTimeStart=tmpTime; SetGraphs();}
	int ColorIdx,i,j;
	//Eye Rotation Vector Plots
	CDC *pmDC=&m_DC[0];
	//NwID==LEFT
	aDC.SelectObject(m_RED);	pmDC->SelectObject(m_RED);
	m_TwoD1.Graph2D (m_prvTime,m_prevlH,nowT, ThD.m_dlH[FrameNum]-m_avelH*m_SubtractAvg+m_d4FifthOrdinateAxis, 2,&aDC   ); 
	m_TwoD1.Graph2D (m_prvTime,m_prevlH,nowT, ThD.m_dlH[FrameNum]-m_avelH*m_SubtractAvg+m_d4FifthOrdinateAxis, 2,pmDC   ); 
	m_prevlH=ThD.m_dlH[FrameNum]-m_avelH*m_SubtractAvg+m_d4FifthOrdinateAxis;
	aDC.SelectObject(m_GREEN);	pmDC->SelectObject(m_GREEN);		
	m_TwoD1.Graph2D (m_prvTime,m_prevlV,nowT, ThD.m_dlV[FrameNum]-m_avelV*m_SubtractAvg+m_dHalfOrdinateAxis  , 2,&aDC   );     
	m_TwoD1.Graph2D (m_prvTime,m_prevlV,nowT, ThD.m_dlV[FrameNum]-m_avelV*m_SubtractAvg+m_dHalfOrdinateAxis  , 2,pmDC   );     
	m_prevlV=ThD.m_dlV[FrameNum]-m_avelV*m_SubtractAvg+m_dHalfOrdinateAxis;
	aDC.SelectObject(m_BLUE);	pmDC->SelectObject(m_BLUE);			
	m_TwoD1.Graph2D (m_prvTime,m_prevlT,nowT, ThD.m_dlT[FrameNum]-m_avelT*m_SubtractAvg+m_d1FifthOrdinateAxis, 2,&aDC   );   
	m_TwoD1.Graph2D (m_prvTime,m_prevlT,nowT, ThD.m_dlT[FrameNum]-m_avelT*m_SubtractAvg+m_d1FifthOrdinateAxis, 2,pmDC   );  
	m_prevlT=ThD.m_dlT[FrameNum]-m_avelT*m_SubtractAvg+m_d1FifthOrdinateAxis;
	
	//NwID==RIGHT (GRAY color)
	aDC.SelectObject(m_GRAY);	pmDC->SelectObject(m_GRAY);
	m_TwoD1.Graph2D (m_prvTime,m_prevrH,nowT, ThD.m_drH[FrameNum]-m_averH*m_SubtractAvg+m_d4FifthOrdinateAxisPlus, 2,&aDC   ); 
	m_TwoD1.Graph2D (m_prvTime,m_prevrH,nowT, ThD.m_drH[FrameNum]-m_averH*m_SubtractAvg+m_d4FifthOrdinateAxisPlus, 2,pmDC   ); 
	m_prevrH=ThD.m_drH[FrameNum]-m_averH*m_SubtractAvg+m_d4FifthOrdinateAxisPlus;
	m_TwoD1.Graph2D (m_prvTime,m_prevrV,nowT, ThD.m_drV[FrameNum]-m_averV*m_SubtractAvg+m_dHalfOrdinateAxisPlus  , 2,&aDC   );     
	m_TwoD1.Graph2D (m_prvTime,m_prevrV,nowT, ThD.m_drV[FrameNum]-m_averV*m_SubtractAvg+m_dHalfOrdinateAxisPlus  , 2,pmDC   );     
	m_prevrV=ThD.m_drV[FrameNum]-m_averV*m_SubtractAvg+m_dHalfOrdinateAxisPlus;
	m_TwoD1.Graph2D (m_prvTime,m_prevrT,nowT, ThD.m_drT[FrameNum]-m_averT*m_SubtractAvg+m_d1FifthOrdinateAxisPlus, 2,&aDC   );     
	m_TwoD1.Graph2D (m_prvTime,m_prevrT,nowT, ThD.m_drT[FrameNum]-m_averT*m_SubtractAvg+m_d1FifthOrdinateAxisPlus, 2,pmDC   );     
	m_prevrT=ThD.m_drT[FrameNum]-m_averT*m_SubtractAvg+m_d1FifthOrdinateAxisPlus;

	//Aux figure
	CGraph2D *pBd2=&m_TwoD2;
	int idx;
	float AuxVec;
	for(i=0;i<m_howmanyAuxVectors;i++){
		idx=(int)((MaxPen-1)*(i+1.0)/m_howmanyAuxVectors);
		AuxVec=m_AuxVec[m_SizeOfEachChannel*i+FrameNum]-m_avgAux[i]*m_SubtractAvg;
		aDC.SelectObject(&m_rgbPEN1[idx]);	pmDC->SelectObject(&m_rgbPEN1[idx]);
		pBd2->Graph2D (m_prvTime,m_prvAux[i],nowT, AuxVec, 2,&aDC ); 
		pBd2->Graph2D (m_prvTime,m_prvAux[i],nowT, AuxVec, 2,pmDC ); 
		m_prvAux[i]=AuxVec;
	}

	m_prvTime=nowT;
	if(m_refreshScreen){
		aDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	}
}


int COrbitViewView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD)
{	
	CFileDialog dialog(IsToREAD, "txt", "*.txt", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "Data files (*.txt)|All files(*.*)|*.*|| ",NULL);
	if(dialog.DoModal()==IDCANCEL){return 0;}
	sprintf(m_FileName,"%s", dialog.GetPathName()); 
	sprintf(m_JustFileName,"%s", dialog.GetFileName()); 
	strcpy_s(m_FolderPath,m_FileName);
	m_FolderPath[strlen(m_FileName)-strlen(m_JustFileName)]='\0';
	return 1;
}

#include <sys/stat.h>
//This plays the patient's eye movements
void COrbitViewView::Load_and_Play_fromFile() 
{
	m_bPlay=0; //Quit all the functions.

	int g=0;
	CClientDC aDC(this);
	double lH,lV,lT,rH,rV,rT,Aux;
	char strBuff[1024],labels[1024];
	VirMemDC();
	ThD.m_pView=this;
	//////////////////////////
	//////////////////////////
	LONG i,j, dataSize=0; 
	double Dump;
	if(GetFile_WholePath_Name_CurPath(1)==0)return;//Canceled
	FILE *fp;
	if((fp=fopen(m_FileName,"r"))== NULL){AfxMessageBox("Failed to open the file", MB_OK); exit(0);}
	

	//##############################  Read the Header #######################################
	//##########  The first line is required to contain a "sampling rate" in Hz. ############
	//Extract the sampling frequency from the first line
	fgets(strBuff, 1024,fp); _strlwr_s(strBuff); //Read the first line
	int sizeofChar=strlen(strBuff);
	for(i=0;i<sizeofChar;i++){ //Scan if the first line contains a number
		if(*(strBuff+i)>='0' && *(strBuff+i)<='9')break; //Found a number!!
	}
	if(i<sizeofChar){//the first line contains a number
		m_OriginalFrameRate=Find_LastNum_inString((strBuff+i));
		if(wildcmp(labels, "*khz*"))m_OriginalFrameRate*=1000.;
	}
	else{
		AfxMessageBox("The first line does not contain a number (Hz)"); return;
	}
	//##########  The second line is required to contain the lables of the vectors. Each label is divided by a blank (' ') or a tab ('\\t') of any of the combination of both for the fscanf() to itemize. ############
	int howManyDigits=1; 
	m_totVectors=0;	m_howmanyEyeVectors=0;	m_is_left_H=0; 	m_is_left_V=0; 	m_is_left_T=0; 	m_is_right_H=0;	m_is_right_V=0;	m_is_right_T=0;
	m_howmanyAuxVectors=0; //By default, it assumes there is no auxiliary signal other than the eye ball signals.
	m_bIsToShowHead=0; 
	while(1){
		fscanf(fp,"%s",&labels); strcpy(strBuff,labels); _strlwr_s(labels);
		if(*(labels)>='0' && *(labels)<='9'){//Check the first component of the item ==>The labels should not srart with a number
			fseek(fp,-strlen(labels),SEEK_CUR);break; //Found a number!!
		}

		if(		wildcmp(labels, "lefth") ||wildcmp(labels, "left_h") ||wildcmp(labels, "left-h") ){m_VctrID[m_totVectors++]=0; m_is_left_H=1; m_howmanyEyeVectors++;}
		else if(wildcmp(labels, "leftv") ||wildcmp(labels, "left_v") ||wildcmp(labels, "left-v") ){m_VctrID[m_totVectors++]=1; m_is_left_V=1; m_howmanyEyeVectors++;}
		else if(wildcmp(labels, "leftt") ||wildcmp(labels, "left_t") ||wildcmp(labels, "left-t") ){m_VctrID[m_totVectors++]=2; m_is_left_T=1; m_howmanyEyeVectors++;}
		else if(wildcmp(labels, "righth")||wildcmp(labels, "right_h")||wildcmp(labels, "right-h")){m_VctrID[m_totVectors++]=3; m_is_right_H=1;m_howmanyEyeVectors++;}
		else if(wildcmp(labels, "rightv")||wildcmp(labels, "right_v")||wildcmp(labels, "right-v")){m_VctrID[m_totVectors++]=4; m_is_right_V=1;m_howmanyEyeVectors++;}
		else if(wildcmp(labels, "rightt")||wildcmp(labels, "right_t")||wildcmp(labels, "right-t")){m_VctrID[m_totVectors++]=5; m_is_right_T=1;m_howmanyEyeVectors++;}
		else if(wildcmp(labels, "headh")||wildcmp(labels, "head_h")||wildcmp(labels, "head-h")){m_bIsToShowHead=1; m_VctrID[m_totVectors++]=0; m_is_left_H=1;m_howmanyEyeVectors++;} //The head data is stored in LEFT H, V, T array.
		else if(wildcmp(labels, "headv")||wildcmp(labels, "head_v")||wildcmp(labels, "head-v")){m_bIsToShowHead=1; m_VctrID[m_totVectors++]=1; m_is_left_V=1;m_howmanyEyeVectors++;} //The head data is stored in LEFT H, V, T array.
		else if(wildcmp(labels, "headt")||wildcmp(labels, "head_t")||wildcmp(labels, "head-t")){m_bIsToShowHead=1; m_VctrID[m_totVectors++]=2; m_is_left_T=1;m_howmanyEyeVectors++;} //The head data is stored in LEFT H, V, T array.
		else {m_VctrID[m_totVectors]=MaxEyeVectors+m_howmanyAuxVectors; strcpy(m_AuxLabels[m_howmanyAuxVectors],strBuff); m_totVectors++; m_howmanyAuxVectors++;}
	}
	if(m_is_left_H==0&&m_is_left_V==0&&m_is_left_T==0)m_isLeftEye=0; else m_isLeftEye=1;
	if(m_is_right_H==0&&m_is_right_V==0&&m_is_right_T==0)m_isRightEye=0; else m_isRightEye=1;
	//############################### End of reading the Header ###############################
	
	if(m_bIsToShowHead==0)g=EyeBall();
	else{
		g=Head_Shape();
		if(g==ThD.m_nNumPointsForCross){AfxMessageBox("Cound not find the \"Head_Shape.txt\" file."); return;}
	}
	//////////////////////////

	m_dSampleAtEvery_Xms=1000./m_OriginalFrameRate;//sampling frequency
	m_FramesPerSec=30;//30Hz
	m_nSampleRate=m_OriginalFrameRate/m_FramesPerSec;

	// Check the size of the whole data
	do{		
		if(EOF==fscanf(fp,"%lf",&Dump) )break; 
		dataSize++;
	}while(1);
	if(dataSize/(double)m_totVectors!=dataSize/(int)m_totVectors){AfxMessageBox("This file seems to contain vectors not equvalent in sizes"); return;}
	m_SizeOfEachChannel=dataSize/m_totVectors;
	rewind(fp);fgets(strBuff,1000,fp);fgets(strBuff,1000,fp);// rewind to the begining of vectors

	//Set the sizes of memory
	ThD.ObserverMON.SetSize(g);
	ThD.m_dlH.SetSize(m_SizeOfEachChannel);	ThD.m_dlV.SetSize(m_SizeOfEachChannel);	ThD.m_dlT.SetSize(m_SizeOfEachChannel);	
	ThD.m_drH.SetSize(m_SizeOfEachChannel);	ThD.m_drV.SetSize(m_SizeOfEachChannel);	ThD.m_drT.SetSize(m_SizeOfEachChannel);
	if(m_howmanyAuxVectors>0)m_AuxVec.SetSize(m_SizeOfEachChannel*m_howmanyAuxVectors); 

	//Load the data for each channel
	j=-1;    
	m_MaxH=-1000; m_MinH=1000; m_MaxV=-1000; m_MinV=1000; m_MaxT=-1000; m_MinT=1000;
	m_avelH=0;m_avelV=0;m_avelT=0; m_averH=0;m_averV=0;m_averT=0;
	m_MaxAux=-1000000000; m_MinAux=1000000000; 
	for(i=0;i<m_howmanyAuxVectors;i++)m_avgAux[i]=0;
	int idx;
	for(i=0;i<dataSize;i++){
		if(i%m_totVectors==0)j++;//Increase the row
		idx=m_VctrID[i%m_totVectors];
		switch(idx){
			case 0: 
			if(EOF==fscanf(fp,"%lf",&lH)){AfxMessageBox("Error 1");fclose(fp); return; };
			ThD.m_dlH[j]=lH; m_avelH+=lH; if(m_MaxH<lH)m_MaxH=lH;if(m_MinH>lH)m_MinH=lH;
			break;
			case 1: 
			if(EOF==fscanf(fp,"%lf",&lV)){AfxMessageBox("Error 1");fclose(fp); return; };
			ThD.m_dlV[j]=lV; m_avelV+=lV; if(m_MaxV<lV)m_MaxV=lV;if(m_MinV>lV)m_MinV=lV;
			break;
			case 2: 
			if(EOF==fscanf(fp,"%lf",&lT)){AfxMessageBox("Error 1");fclose(fp); return; };
			ThD.m_dlT[j]=lT; m_avelT+=lT; if(m_MaxT<lT)m_MaxT=lT;if(m_MinT>lT)m_MinT=lT;
			break;
			case 3: 
			if(EOF==fscanf(fp,"%lf",&rH)){AfxMessageBox("Error 1");fclose(fp); return; };
			ThD.m_drH[j]=rH; m_averH+=rH; if(m_MaxH<rH)m_MaxH=rH;if(m_MinH>rH)m_MinH=rH;
			break;
			case 4: 
			if(EOF==fscanf(fp,"%lf",&rV)){AfxMessageBox("Error 1");fclose(fp); return; };
			ThD.m_drV[j]=rV; m_averV+=rV; if(m_MaxV<rV)m_MaxV=rV;if(m_MinV>rV)m_MinV=rV;
			break;
			case 5: 
			if(EOF==fscanf(fp,"%lf",&rT)){AfxMessageBox("Error 1");fclose(fp); return; };
			ThD.m_drT[j]=rT; m_averT+=rT; if(m_MaxT<rT)m_MaxT=rT;if(m_MinT>rT)m_MinT=rT;
			break;
			default://Aux signals
			if(EOF==fscanf(fp,"%lf",&Aux)){AfxMessageBox("Error Aux");fclose(fp); return; };
				m_AuxVec[m_SizeOfEachChannel*(idx-MaxEyeVectors)+j]=Aux;
				m_avgAux[idx-MaxEyeVectors]+=Aux;if(m_MaxAux<Aux)m_MaxAux=Aux;if(m_MinAux>Aux)m_MinAux=Aux;
			break;
		}
	}
	for(i=0;i<m_SizeOfEachChannel;i++){
		if(m_is_left_H==0){ThD.m_dlH[i]=0;}
		if(m_is_left_V==0){ThD.m_dlV[i]=0;}
		if(m_is_left_T==0){ThD.m_dlT[i]=0;}
		if(m_is_right_H==0){ThD.m_drH[i]=0; if(m_is_left_H==0){m_MaxH=0; m_MinH=0;} }
		if(m_is_right_V==0){ThD.m_drV[i]=0; if(m_is_left_V==0){m_MaxV=0; m_MinV=0;} }
		if(m_is_right_T==0){ThD.m_drT[i]=0; if(m_is_left_T==0){m_MaxT=0; m_MinT=0;} }
	}
	m_avelH/=m_SizeOfEachChannel;m_avelV/=m_SizeOfEachChannel;m_avelT/=m_SizeOfEachChannel; m_averH/=m_SizeOfEachChannel;m_averV/=m_SizeOfEachChannel;m_averT/=m_SizeOfEachChannel;
	for(i=0;i<m_howmanyAuxVectors;i++)m_avgAux[i]/=m_SizeOfEachChannel;
	ThD.m_AveLH=m_avelH; ThD.m_AveLV=m_avelV; ThD.m_AveLT=m_avelT; ThD.m_AveRH=m_averH;ThD.m_AveRV=m_averV;ThD.m_AveRT=m_averT;
	ThD.m_nSizeOfEachChannel=m_SizeOfEachChannel; m_DataEndsAt=(m_SizeOfEachChannel/m_OriginalFrameRate)*1000.-1; m_endAt=m_DataEndsAt;

	//Measure the data and adjust the axes to accomodate them
	float BiggestH,BiggestV,BiggestT, Biggest;
	if(fabs(m_MaxH)>fabs(m_MinH))BiggestH=m_MaxH; else BiggestH=m_MinH; if(fabs(m_MaxV)>fabs(m_MinV))BiggestV=m_MaxV; else BiggestV=m_MinV;  if(fabs(m_MaxT)>fabs(m_MinT))BiggestT=m_MaxT; else BiggestT=m_MinT;
	if(fabs(BiggestH)>=fabs(BiggestV) && fabs(BiggestH)>=fabs(BiggestT))Biggest=BiggestH; 
	else if(fabs(BiggestV)>=fabs(BiggestH) && fabs(BiggestV)>=fabs(BiggestT))Biggest=BiggestV;
	else if(fabs(BiggestT)>=fabs(BiggestH) && fabs(BiggestT)>=fabs(BiggestV))Biggest=BiggestT;
	else AfxMessageBox("What is this?");
	OrdinateScale(Biggest);


	/////////////////////////////////////////////////////////////////////////////////////////
	ThD.ObjCntrX=0 ;ThD.ObjCntrY=0; ThD.ObjCntrZ=0;
	ThD.ObserverCntrRotX=0; ThD.ObserverCntrRotY=0;ThD.ObserverCntrRotZ=0;
	Observer_Origin();
	m_bDataLoaded=1;
	/////////////////////////////////////////////////////////////////////////////////////////
	m_nShowTimeStart=0;m_nShowTimeEnd=10000;  m_prvTime=0; 
	SetGraphs();

	m_bPlay=1;
	ThD.HVT_EyePosition(g);// This function is defined in "ThreeD.h" and implemented in EyePos.cpp.
	m_bPlay=0;
}

void COrbitViewView::OrdinateScale(float individualMaxOrdinateAxis) 
{
	m_individualMaxOrdinateAxis=fabs(individualMaxOrdinateAxis);
	if(m_individualMaxOrdinateAxis>=1.0)m_individualMaxOrdinateAxis=(int)m_individualMaxOrdinateAxis;//Remove the fraction part
	m_dMaxOrdinateAxis=10*fabs(m_individualMaxOrdinateAxis);//One sub-figure (H,V or T) takes up 20% of the whole scale.
	m_dHalfOrdinateAxis    =m_dMaxOrdinateAxis/2.;   m_d1FifthOrdinateAxis    =m_dMaxOrdinateAxis/5.;   m_d4FifthOrdinateAxis    =m_dMaxOrdinateAxis*(4./5.);
	//m_Ybias4Disp (in degree): Give some bias to the right eye to display, so that it is more distinguisable from the left eye trace
	m_dHalfOrdinateAxisPlus=m_dMaxOrdinateAxis/2.+m_Ybias4Disp; m_d1FifthOrdinateAxisPlus=m_dMaxOrdinateAxis/5.+m_Ybias4Disp; m_d4FifthOrdinateAxisPlus=m_dMaxOrdinateAxis*(4./5.)+m_Ybias4Disp;
}	

int COrbitViewView::EyeBall() 
{
	VirMemDC();
	ThD.m_pView=this;

/////////////////////// It should be adjusted by the user  ////////////////
#define SizeOfBall 5
	int g=0;
	double X,Y,Z,r,Theta,theta0,dTheta=0.005, randTheta;
#define dTHETA (TWO_PI/(200*theta0))
	//The reference vectors: (1,0,0) (0,1,0) (0,0,1)                                              
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][0]=1;//X axis (1,0,0)  
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][1]=0;                  
	ThD.ObjCntrd_Coordnt_SrfcPoint[g++][2]=0;
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][0]=0;//Y axis (0,1,0)  
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][1]=1;                  
	ThD.ObjCntrd_Coordnt_SrfcPoint[g++][2]=0;
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][0]=0;//Z axis (0,0,1)
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][1]=0; 
	ThD.ObjCntrd_Coordnt_SrfcPoint[g++][2]=1;
	// 3D cross lines
	for( theta0=0.2; theta0<PIover2+1.;theta0+=dTheta ){
		r=SizeOfBall*sin(theta0);
		Y=SizeOfBall*cos(theta0);
		for( Theta=0; Theta<TWO_PI; Theta+=dTHETA){
			if(((int)(Theta*1000)%(int)(PIover2*1000))>20 )continue;
			randTheta=dTHETA*(0.5-RAND_RATE);
			X=r*cos(Theta+randTheta);Z=r*sin(Theta+randTheta);
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][0]=X;                             //
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][1]=Y;                            //
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][2]=Z;
			if(++g==MEM)goto ExitLoop;
		}
	}
	ThD.m_nNumPointsForCross=g;
	// Iris
	for( theta0=0.2; theta0<=0.4;theta0+=0.06 ){//sagittal angle along the eye axis
		for( Theta=0; Theta<TWO_PI; Theta+=TWO_PI/50.){
			r=SizeOfBall*sin(theta0);
			Y=SizeOfBall*cos(theta0);
			X=r*cos(Theta);Z=r*sin(Theta);
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][0]=X;                             //
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][1]=Y;                            //
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][2]=Z;
			if(++g==MEM)goto ExitLoop;
		}
	}
	ThD.m_nEndOfIRIS=g;
	// Sclera
	for( theta0=0.4; theta0<PIover2+1.;theta0+=dTheta ){//sagittal angle along the eye axis
		r=SizeOfBall*sin(theta0);
		Y=SizeOfBall*cos(theta0);
		for( Theta=0; Theta<TWO_PI; Theta+=dTHETA){//theta0*200 =>200 gives the best texture
			if(PROBABILITY(0.99) )continue;
			randTheta=dTHETA*(0.5-RAND_RATE); //This makes the texture of the eye ball more homogeneous, by randomizing the circular components. 
			X=r*cos(Theta+randTheta);Z=r*sin(Theta+randTheta);
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][0]=X;                             //
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][1]=Y;                            //
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][2]=Z;
			if(++g==MEM)goto ExitLoop;
		}
	}
ExitLoop:
	return g;
}

int COrbitViewView::Head_Shape() 
{
	VirMemDC();
	ThD.m_pView=this;

/////////////////////// It should be adjusted by the user  ////////////////
#define SizeOfBall 5
	int g=0;
	double X,Y,Z,R,Theta,theta0,dTheta=0.005, randTheta;
#define dTHETA (TWO_PI/(200*theta0))
	//The reference vectors: (1,0,0) (0,1,0) (0,0,1)                                              
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][0]=1;//X axis (1,0,0)  
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][1]=0;                  
	ThD.ObjCntrd_Coordnt_SrfcPoint[g++][2]=0;
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][0]=0;//Y axis (0,1,0)  
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][1]=1;                  
	ThD.ObjCntrd_Coordnt_SrfcPoint[g++][2]=0;
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][0]=0;//Z axis (0,0,1)
	ThD.ObjCntrd_Coordnt_SrfcPoint[g  ][1]=0; 
	ThD.ObjCntrd_Coordnt_SrfcPoint[g++][2]=1;
	// 3D cross lines
	for( theta0=0.2; theta0<PIover2+1.;theta0+=dTheta ){
		R=SizeOfBall*sin(theta0);
		Y=SizeOfBall*cos(theta0);
		for( Theta=0; Theta<TWO_PI; Theta+=dTHETA){
			if(((int)(Theta*1000)%(int)(PIover2*1000))>20 )continue;
			randTheta=dTHETA*(0.5-RAND_RATE);
			X=R*cos(Theta+randTheta);Z=R*sin(Theta+randTheta);
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][0]=X;                             
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][1]=Y;                            
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][2]=Z;
			if(++g==MEM)goto ExitLoop1;
		}
	}
ExitLoop1:
	ThD.m_nNumPointsForCross=g;

	FILE *fp;
	float dummyF=1.0;
	double futureUse, x,y,z;
	int Red,Green,Blue;
	char HeadShapeFile[1028];
	CString ExeDirString=GetExecutableName();
	strcpy(HeadShapeFile,ExeDirString);
	//FromA_findB_addC_giveD(HeadShapeFile, "Source_Code", "Head_Shape.txt", HeadShapeFile);
	GetGrandParentFolderName(HeadShapeFile, HeadShapeFile);
	AddFrwdSlashAtEndIfNotHave(HeadShapeFile);
	strcat(HeadShapeFile,"Head_Shape.txt");

	//strcpy(HeadShapeFile,"C:\\Users\\simon\\Desktop\\Orbit_n_head_View\\Head_Shape.txt");

	if((fp=fopen(HeadShapeFile,"r"))!= NULL){
		while(fscanf( fp, "%lf%lf%lf%d%d%d",&x, &y, &z, &Red,&Green,&Blue)!=EOF){
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][0]=x;                             
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][1]=y;                            
			ThD.ObjCntrd_Coordnt_SrfcPoint[g][2]=z;
			if(++g==MEM)goto ExitLoop2;
		}
	}
ExitLoop2:
	return g;
}

