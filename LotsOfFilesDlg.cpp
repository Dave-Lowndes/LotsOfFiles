// LotsOfFilesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LotsOfFiles.h"
#include "LotsOfFilesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLotsOfFilesDlg dialog

CLotsOfFilesDlg::CLotsOfFilesDlg(CWnd* pParent /*=NULL*/)
	: 	m_pThread( NULL ), CDialog(CLotsOfFilesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLotsOfFilesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLotsOfFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLotsOfFilesDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROGRESS, m_ProgCtl);
}

#define UWM_THREAD_FINISHED WM_APP+1
#define UWM_UPDATE_THREAD_PROGRESS WM_APP+2

BEGIN_MESSAGE_MAP(CLotsOfFilesDlg, CDialog)
	//{{AFX_MSG_MAP(CLotsOfFilesDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CREATE, OnCreateFiles)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	//}}AFX_MSG_MAP
	ON_MESSAGE( UWM_THREAD_FINISHED, CLotsOfFilesDlg::HandleThreadEnd )
	ON_MESSAGE( UWM_UPDATE_THREAD_PROGRESS, CLotsOfFilesDlg::UpdateProgress )
//	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLotsOfFilesDlg message handlers

BOOL CLotsOfFilesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetDlgItemText( IDC_FNAME, _T(".\\Test\\Filena%d.ext") );
	SetDlgItemText( IDC_MATCH, _T(".\\Test\\Filena*") );
	
	SetDlgItemInt( IDC_NUM_FILES, 100 );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLotsOfFilesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLotsOfFilesDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLotsOfFilesDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

class CThreadParams
{
public:
	CString m_sFileNameTemplate;
	size_t m_NumFiles;
	HWND m_hDlg;
};

static bool bStopThread;
static LARGE_INTEGER pc1;
static LARGE_INTEGER pc2;
static LARGE_INTEGER freq;

UINT __cdecl CreateFilesThread( LPVOID pParam )
{
	/* Take a copy of the thread parameters */
	CThreadParams * pTPs = static_cast<CThreadParams *>( pParam );
	CThreadParams TPs = *pTPs;
	/* And free the passed ones */
	delete pTPs;

	CString sFileName;
	const size_t NumFiles = TPs.m_NumFiles;
	CString strFName( TPs.m_sFileNameTemplate );
	const size_t UpdatePoint = NumFiles / 100;

	QueryPerformanceFrequency( &freq );
	QueryPerformanceCounter( &pc1 );

	for ( size_t indx = 0; (indx < NumFiles ) && !bStopThread; ++indx )
	{
		sFileName.Format( strFName, indx );

		/* Update the UI appropriately (i.e. not too often) */
		size_t rem = indx % UpdatePoint;
		if ( rem == 0 )
		{
			PostMessage( TPs.m_hDlg, UWM_UPDATE_THREAD_PROGRESS, 0, indx );
		}

		const HANDLE hFile = CreateFile( sFileName, GENERIC_WRITE | GENERIC_READ, 0, NULL,
											CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

		if ( INVALID_HANDLE_VALUE != hFile )
		{
			CloseHandle( hFile );
		}
		else
		{
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);

			CString sMsg;
			sMsg.Format( "Could not create file %s\n\n%s", static_cast< LPCTSTR>( sFileName ), static_cast<LPCTSTR>( lpMsgBuf ) );

			// Display the string.
			AfxMessageBox( sMsg );
			// Free the buffer.
			LocalFree( lpMsgBuf );

			break;
		}
	}

	QueryPerformanceCounter( &pc2 );
	pc2.QuadPart -= pc1.QuadPart;
	pc2.QuadPart = pc2.QuadPart*1000 / freq.QuadPart;

	/* Signal the dialog that we've finished */
	PostMessage( TPs.m_hDlg, UWM_THREAD_FINISHED, 0, 0 );

	return 0;
}

static CString sButtonText;

void CLotsOfFilesDlg::OnCreateFiles() 
{
	/* If we're not creating files, start off the process */
	if ( m_pThread == NULL )
	{
		/* Hide the text message and make the progress control visible */
		GetDlgItem( IDC_MSG_TXT )->ShowWindow( SW_HIDE );
		m_ProgCtl.ShowWindow( SW_SHOWNA );

		/* Change the button text */
		GetDlgItem( IDC_CREATE )->GetWindowText( sButtonText );
		GetDlgItem( IDC_CREATE )->SetWindowText( _T("Stop") );
	
		/* Disable all the functional controls on the dialog */
		//IDC_FNAME
		//	IDC_NUM_FILES
		//	IDC_CREATE
		//	IDCANCEL
	
		/* Create the worker thread */
		bStopThread = false;
		CThreadParams * pTPs = new CThreadParams();
		pTPs->m_hDlg = m_hWnd;
		pTPs->m_NumFiles = GetDlgItemInt( IDC_NUM_FILES );
		GetDlgItemText( IDC_FNAME, pTPs->m_sFileNameTemplate );

		/* Scale the progress control */
		m_ProgCtl.SetRange32( 0, pTPs->m_NumFiles );
	
		m_pThread = AfxBeginThread( CreateFilesThread, pTPs, THREAD_PRIORITY_BELOW_NORMAL );
	}
	else
	{
		/* If the thread is running, we want to stop it */
		bStopThread = true;
	}
}

LRESULT CLotsOfFilesDlg::HandleThreadEnd(WPARAM, LPARAM)
{
	m_pThread = NULL;

	TCHAR szMsg[100];
//	wsprintf( szMsg, _T("Time taken: %I64d ms"), pc2.QuadPart );
	_ui64tot_s( pc2.QuadPart, szMsg, _countof( szMsg ), 10 );
	SetDlgItemText( IDC_TIME_TXT, szMsg );

	/* Restore the UI */
	GetDlgItem( IDC_CREATE )->SetWindowText( sButtonText );
	/* Hide the progress and restore the text */
	GetDlgItem( IDC_MSG_TXT )->ShowWindow( SW_SHOWNA );
	m_ProgCtl.ShowWindow( SW_HIDE );

	return 0;
}

LRESULT CLotsOfFilesDlg::UpdateProgress(WPARAM, LPARAM indx )
{
	m_ProgCtl.SetPos( indx );

	return 0;
}

void CLotsOfFilesDlg::OnFind() 
{
	static_cast<CListBox*>( GetDlgItem( IDC_LIST ) )->ResetContent();

	CString sFind;
	
	GetDlgItemText( IDC_MATCH , sFind );

//	sFind = ".\\Test\\" + sFind;

	CFileFind ff;

	int iCount = 0;

	if ( ff.FindFile( sFind ) )
	{
		int OK;

		do
		{
			OK = ff.FindNextFile();
			CString sMatchName;
			sMatchName = ff.GetFileName();

			static_cast<CListBox*>( GetDlgItem( IDC_LIST ) )->AddString( sMatchName );

			iCount++;

		} while ( OK );

		ff.Close();

		CString sMsg;
		sMsg.Format( "There are %d files", iCount );
		AfxMessageBox( sMsg );
	}
}

void CLotsOfFilesDlg::OnOK()
{
	/* Don't call the default - so pressing Enter doesn't close the dialog */
}

void CLotsOfFilesDlg::OnDestroy()
{
	CDialog::OnDestroy();

	/* Signal the worker thread to stop */
	bStopThread = true;
}
