// LotsOfFiles.h : main header file for the LOTSOFFILES application
//

#if !defined(AFX_LOTSOFFILES_H__5591D217_A989_40AD_A371_9D51CA0A2640__INCLUDED_)
#define AFX_LOTSOFFILES_H__5591D217_A989_40AD_A371_9D51CA0A2640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLotsOfFilesApp:
// See LotsOfFiles.cpp for the implementation of this class
//

class CLotsOfFilesApp : public CWinApp
{
public:
	CLotsOfFilesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLotsOfFilesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLotsOfFilesApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOTSOFFILES_H__5591D217_A989_40AD_A371_9D51CA0A2640__INCLUDED_)
