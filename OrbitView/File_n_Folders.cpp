#include "stdafx.h"
#include "OrbitView.h"

#include "Graph2D.h"
#include "ThreeD.h"
#include "OrbitViewDoc.h"
#include "OrbitViewView.h"

//This routine extracts numbers from a string. 
//There could be many isolated numbers scattered among letters, but it registers only the last one (number e.g. 123 from 76DR123.txt).
int COrbitViewView::Find_LastNum_inString(char *GivenString)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1,foundNum;
	int num = 0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			num=0;//This one is here to make sure that the only the last group of digits are captured (e.g. 123 from 76DR123.txt).
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				num = num + (*q-48) * pow((double)10,(double)iCount);
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			++p ;
		}
	}		
	if(foundNum==0){
		char strbuf[1000];
		_snprintf(strbuf, 999, "$s => does not contain a number", GivenString);
		AfxMessageBox(strbuf);
	}
	return num;
}

int COrbitViewView::wildcmp(char *wild, char *string) 
{
  char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } 
	else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } 
	else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  int a=strlen(wild);
  if(a==0)return 1;
  return !*wild;
}


CString COrbitViewView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

//This one discards the parent folder name out of the full pathway "FolderPath"
int COrbitViewView::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first two (\*) letters
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(strcmp("\\", FldrName)==0)break;
	}
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
	return 1;
}

int COrbitViewView::FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[])
{
	char tmpStr[1024], FindNameWild[100],FindName[100];
	strcpy(tmpStr,fromA);
	sprintf(FindNameWild,"*\\%s*",findB);
	sprintf(FindName,"*\\%s",findB);
	if(wildcmp(FindNameWild,tmpStr)==1){
		RE_Search:;
		if(wildcmp(FindName,tmpStr)==1){
			sprintf(giveD,"%s\\",tmpStr);
		}
		else {GetGrandParentFolderName(tmpStr,tmpStr); goto RE_Search;}
		if(addC[0]=='\\')RemoveForwardSlashAtEnd(giveD);
		strcat(giveD,addC);
	}		
	else{
		//See if the parent folder of fromA has "addC".
		GetGrandParentFolderName(fromA,tmpStr);
		if(FindAllFiles(tmpStr,0,addC)==1){
			AddFrwdSlashAtEndIfNotHave(tmpStr);
			strcpy(giveD,tmpStr);
			strcat(giveD,addC);
		}
		else{
			sprintf(tmpStr,"Couldn't find %s",findB);
			AfxMessageBox(tmpStr);
			return 0;
		}
	}
	return 1;
}

// This function will find all files in a directory and its subdirectory
// For each file found, it calls a function.
int COrbitViewView::FindAllFiles(char* searchThisDir, bool isToSearchSubDirs, char* FindJustFileName)
{
	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Build the file search string
	char searchDir[2048] = {0};

	// If it already is a path that ends with \, only add the *
	if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir, 2047, "%s*", searchThisDir);
	// Otherwise, add the \* to the end of the path
	else _snprintf(searchDir, 2047, "%s\\*", searchThisDir);

	// Find the first file in the directory.
	hFind = FindFirstFile(searchDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return 0;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		

		// If we find a directory
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			// If we want to search subdirectories
			if(isToSearchSubDirs){
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				// If it already is a path that ends with \, only add the *
				if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir2, 2047, "%s%s", searchThisDir, FindFileData.cFileName);
				// Otherwise, add the \* to the end of the path
				else _snprintf(searchDir2, 2047, "%s\\%s", searchThisDir, FindFileData.cFileName);
				FindAllFiles(searchDir2, isToSearchSubDirs,FindJustFileName);
			}
			// Do not need to process anymore
			continue;
		}
		//Found a file name.
		if(strcmp(FindFileData.cFileName, FindJustFileName)==0)return 1;
	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
	return 0;
}
