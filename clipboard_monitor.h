// clipboard_monitor.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CClipboardMonitorApp:
// See clipboard_monitor.cpp for the implementation of this class
//

class CClipboardMonitorApp : public CWinApp
{
public:
	CClipboardMonitorApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CClipboardMonitorApp theApp;