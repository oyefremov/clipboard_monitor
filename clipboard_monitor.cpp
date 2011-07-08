// clipboard_monitor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "clipboard_monitor.h"
#include "clipboardmonitordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClipboardMonitorApp

BEGIN_MESSAGE_MAP(CClipboardMonitorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClipboardMonitorApp construction

CClipboardMonitorApp::CClipboardMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CClipboardMonitorApp object

CClipboardMonitorApp theApp;


// CClipboardMonitorApp initialization

BOOL CClipboardMonitorApp::InitInstance()
{
	CWinApp::InitInstance();

	CClipboardMonitorDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
