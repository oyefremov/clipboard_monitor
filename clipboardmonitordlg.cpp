// clipboardmonitordlg.cpp : implementation file
//

#include "stdafx.h"
#include "clipboard_monitor.h"
#include "clipboardmonitordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClipboardMonitorDlg dialog




CClipboardMonitorDlg::CClipboardMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClipboardMonitorDlg::IDD, pParent)
	, m_nextWindowInChain(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClipboardMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClipboardMonitorDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CHANGECBCHAIN()
	ON_WM_DESTROY()
	ON_WM_DRAWCLIPBOARD()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CClipboardMonitorDlg message handlers

BOOL CClipboardMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitializeIncludes();
	InitializeMonitor();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClipboardMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClipboardMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClipboardMonitorDlg::InitializeMonitor()
{
	m_nextWindowInChain = SetClipboardViewer();
}
						   
void CClipboardMonitorDlg::OnChangeCbChain(HWND hWndRemove, HWND hWndAfter)
{
	if (hWndAfter == m_nextWindowInChain)
		m_nextWindowInChain = hWndAfter;
	::SendMessage(m_nextWindowInChain, WM_CHANGECBCHAIN, reinterpret_cast<WPARAM>(hWndRemove), reinterpret_cast<LPARAM>(hWndAfter));
}

void CClipboardMonitorDlg::OnDrawClipboard()
{
	CString str = PasteClipboardString();
	::SendMessage(m_nextWindowInChain, WM_DRAWCLIPBOARD, 0, 0);	

	ProcessString(str);
}

CString CClipboardMonitorDlg::PasteClipboardString()
{
	CString result;
	if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard())
	{
		HANDLE hClipboard = GetClipboardData(CF_UNICODETEXT);
		if (hClipboard)
		{
			wchar_t* ptr = reinterpret_cast<wchar_t*>(GlobalLock(hClipboard)); 
			if (ptr != NULL) 
			{ 
				const int MAX_LEN = 1024;
				const int MIN_LEN = 10;
				size_t len = wcsnlen(ptr, MAX_LEN);
				if (MIN_LEN < len && len < MAX_LEN)
					result = ptr;

				GlobalUnlock(hClipboard); 
			} 
		}		
		CloseClipboard();
	}
	return result;
}

void CClipboardMonitorDlg::CopyToClipboard(const wchar_t* str)
{
	size_t len = wcslen(str);
	size_t size = (len + 1) * sizeof(wchar_t);

	HANDLE hMemory = GlobalAlloc(GMEM_MOVEABLE, size); 
	if (hMemory == 0) return; 

	memcpy(GlobalLock(hMemory), str, size);
	GlobalUnlock(hMemory);

	if (OpenClipboard())
	{
		EmptyClipboard(); 
		SetClipboardData(CF_UNICODETEXT, hMemory);
		CloseClipboard();
	}
}

void CClipboardMonitorDlg::ProcessString(CString& str)
{
	if (str.IsEmpty())
		return;

	int len = str.GetLength();

	// remove quotes if present
	if (str[0] == L'\"' && str[len - 1] == L'\"')
	{
		str.Delete(len - 1);
		str.Delete(0);
		len -= 2;
	}

	str.MakeLower();
	
	// check extension
	if (str.Right(2) != ".h")
		return;

	// make UNIX-style slashes
	str.Replace('\\', '/');

	for (int i=0; i<m_includes.GetSize(); ++i)
	{
		const CString& path = m_includes[i]; 
		int pos = str.Find(path);
		if (pos == -1) continue;

		// check to be a valid file name
		DWORD attr = GetFileAttributes(str);
		if (attr ==  INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
			return;

		// remove extra path part
		str.Delete(0, pos + path.GetLength());
		if (str[0] == '/')
			str.Delete(0);

		// create c-style #include string
		CString include;
		include.Format(L"#include \"%s\"\r\n", str); 

		// put it on the clipboard
		CopyToClipboard(include);
		break;
	}
}

void CClipboardMonitorDlg::InitializeIncludes()
{
	m_includes.Add(L"/coreplatform/code/");
}

void CClipboardMonitorDlg::OnDestroy( )
{
	ChangeClipboardChain(m_nextWindowInChain);
}
