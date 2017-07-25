// GraphTraceDlg.h : header file
//

#pragma once

#define GUI_TEST

// CGraphTraceDlg dialog
class CGraphTraceDlg : public CDialog
{
// Construction
public:
	CGraphTraceDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CGraphTraceDlg();
// Dialog Data
	enum { IDD = IDD_GRAPHTRACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	HANDLE m_hGraphTraceHandle;
	HANDLE m_hPutDataHandle;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	static DWORD WINAPI Graph_Task(LPVOID);
	static DWORD WINAPI PutData_Task(LPVOID);
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonEnd();

private:
#ifdef GUI_TEST
	HBRUSH m_hBrush, m_hOldBrush;
#endif
public:
	afx_msg void OnBnClickedBtnOpenDXF();

private:
	CString m_strDxfFilePathName;
};
