// LotsOfFilesDlg.h : header file
//

#include "afxcmn.h"
#if !defined(AFX_LOTSOFFILESDLG_H__E8CACF92_757B_45D5_87CE_95FF6156A38A__INCLUDED_)
#define AFX_LOTSOFFILESDLG_H__E8CACF92_757B_45D5_87CE_95FF6156A38A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLotsOfFilesDlg dialog

class CLotsOfFilesDlg : public CDialog
{
// Construction
public:
	CLotsOfFilesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLotsOfFilesDlg)
	enum { IDD = IDD_LOTSOFFILES_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLotsOfFilesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	HICON m_hIcon;
	CWinThread * m_pThread;
	afx_msg LRESULT HandleThreadEnd(WPARAM, LPARAM);
	afx_msg LRESULT UpdateProgress(WPARAM, LPARAM);
	// Generated message map functions
	//{{AFX_MSG(CLotsOfFilesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCreateFiles();
	afx_msg void OnFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnOK();
	CProgressCtrl m_ProgCtl;
public:
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOTSOFFILESDLG_H__E8CACF92_757B_45D5_87CE_95FF6156A38A__INCLUDED_)
