// OrbitViewDoc.cpp : implementation of the COrbitViewDoc class
//

#include "stdafx.h"
#include "OrbitView.h"

#include "OrbitViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrbitViewDoc

IMPLEMENT_DYNCREATE(COrbitViewDoc, CDocument)

BEGIN_MESSAGE_MAP(COrbitViewDoc, CDocument)
	//{{AFX_MSG_MAP(COrbitViewDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrbitViewDoc construction/destruction

COrbitViewDoc::COrbitViewDoc()
{
	// TODO: add one-time construction code here

}

COrbitViewDoc::~COrbitViewDoc()
{
}

BOOL COrbitViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COrbitViewDoc serialization

void COrbitViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// COrbitViewDoc diagnostics

#ifdef _DEBUG
void COrbitViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COrbitViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COrbitViewDoc commands
