// clipboardmonitordlg.h : header file
//

#pragma once


// CClipboardMonitorDlg dialog
class CClipboardMonitorDlg : public CDialog
{
// Construction
public:
	CClipboardMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLIPBOARD_MONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChangeCbChain(HWND hWndRemove, HWND hWndAfter);
	afx_msg void OnDrawClipboard();
	afx_msg void OnDestroy( );
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void InitializeIncludes();
	void InitializeMonitor();
	CString PasteClipboardString();
	void CopyToClipboard(const wchar_t* str);

	CStringArray m_includes;

	void ProcessString(CString& str);
	HWND m_nextWindowInChain;
};
