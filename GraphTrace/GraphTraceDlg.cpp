// GraphTraceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraphTrace.h"
#include "GraphTraceDlg.h"

#include "Graph Files/Graph.h"

#include "DXF Files/Test_CreationClass.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAboutDlg dialog used for App About

CGraph g_Graph;
bool g_bInterruptFlag = true;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGraphTraceDlg dialog

extern CArray<CBaseGraph*> g_ptrGraphArray;


CGraphTraceDlg::CGraphTraceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphTraceDlg::IDD, pParent)
, m_hGraphTraceHandle(NULL)
, m_hPutDataHandle(NULL)
, m_strDxfFilePathName(L"")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

#ifdef GUI_TEST
	m_hBrush = ::CreateSolidBrush(RGB(0, 0, 0));
#endif
}

CGraphTraceDlg::~CGraphTraceDlg()
{
#ifdef GUI_TEST
	if(m_hBrush != NULL)
	{
		::DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}
	if(m_hOldBrush != NULL)
	{
		::DeleteObject(m_hOldBrush);
		m_hOldBrush = NULL;
	}
#endif

	if(m_hGraphTraceHandle != NULL)
	{
		CloseHandle(m_hGraphTraceHandle);
		m_hGraphTraceHandle = NULL;
	}

	if(m_hPutDataHandle != NULL)
	{
		CloseHandle(m_hPutDataHandle);
		m_hPutDataHandle = NULL;
	}

	if(g_ptrGraphArray.GetSize() != 0)
	{
		g_ptrGraphArray.RemoveAll();
	}
}

void CGraphTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGraphTraceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CGraphTraceDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_END, &CGraphTraceDlg::OnBnClickedButtonEnd)
	ON_BN_CLICKED(IDC_BTN_OPEN_DXF, &CGraphTraceDlg::OnBnClickedBtnOpenDXF)
END_MESSAGE_MAP()


// CGraphTraceDlg message handlers

BOOL CGraphTraceDlg::OnInitDialog()
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
	CWnd *pWnd = GetDlgItem(IDC_STATIC_PAINT);
	
	CRect rt;
	pWnd->GetWindowRect(&rt);
	rt.right = rt.left + 250;
	rt.bottom = rt.top + 250;
	pWnd->MoveWindow(&rt);
	
	/*CDC* pDC = pWnd->GetDC();
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(rt.left + 10, rt.top + 10, rt.right - 10, rt.bottom - 10);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();*/

#ifdef GUI_TEST
	HDC hDC = ::GetDC(pWnd->m_hWnd);
	m_hOldBrush = (HBRUSH)::SelectObject(hDC, m_hBrush);
	::FillRect(hDC, CRect(0, 0, rt.Width(), rt.Height()), m_hBrush);
	::SelectObject(hDC, m_hOldBrush);
#endif

	g_Graph.SetGraphWnd(GetDlgItem(IDC_STATIC_PAINT)->m_hWnd);
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGraphTraceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGraphTraceDlg::OnPaint()
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
HCURSOR CGraphTraceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//绘图线程
DWORD WINAPI CGraphTraceDlg::Graph_Task(LPVOID lpParam)
{
	DWORD dwReturn=0;
	while (!g_bInterruptFlag)
	{
		g_Graph.GraphMain();
		Sleep(CNC_GRAPH_PERIOD_TIME);
	}
	return dwReturn;
}

//放数据的线程
DWORD WINAPI CGraphTraceDlg::PutData_Task(LPVOID)
{
	GraphDataInfoStruct gd;
	memset(gd.nAxisRealPos, 0, sizeof(double) * CNC_AXIS_NUM);
	int i = 0;

	DWORD dwReturn = 0;
	while (!g_bInterruptFlag)
	{
		if(i == 0)
		{
			gd.nAxisRealPos[0] = 0.0;
			gd.nAxisRealPos[1] = 0.0;
		}
		else if(i < 100)
		{
			gd.nAxisRealPos[0] += 0.3;
			gd.nAxisRealPos[1] += 0.2;
			//gd.nAxisRealPos[2] = i * 0.3;
		}
		else if(i < 200)
		{
			gd.nAxisRealPos[0] += 0.1;
			gd.nAxisRealPos[1] += 0.1;
			//gd.nAxisRealPos[2] = i * 0.1;
		}
		else if(i < 300)
		{
			gd.nAxisRealPos[0] -= 0.1;
			gd.nAxisRealPos[1] += 0.1;
		}
		else if(i < 400)
		{
			gd.nAxisRealPos[0] += 0.2;
			gd.nAxisRealPos[1] += 0.1;
			//gd.nAxisRealPos[2] = i * 0.1;
		}
		else if(i < 500)
		{
			gd.nAxisRealPos[0] += 0.1;
			gd.nAxisRealPos[1] -= 0.1;
		}
		else if(i < 600)
		{
			gd.nAxisRealPos[0] -= 0.2;
			gd.nAxisRealPos[1] -= 0.1;
		}
		else if(i < 700)
		{
			gd.nAxisRealPos[0] -= 0.2;
			gd.nAxisRealPos[1] += 0.1;
		}
		else if(i < 1000)
		{
			gd.nAxisRealPos[0] += 0.1;
			gd.nAxisRealPos[1] += 0.1;
		}
		else if(i < 2000)
		{
			gd.nAxisRealPos[0] -= 0.2;
			gd.nAxisRealPos[1] -= 0.1;
		}
		else if(i < 3000)
		{
			gd.nAxisRealPos[0] += 0.2;
			gd.nAxisRealPos[1] -= 0.2;
		}
		else if(i < 4000)
		{
			//gd.nAxisRealPos[0] -= 0.2;
			gd.nAxisRealPos[1] += 0.5;
		}
		else if(i < 5000)
		{
			gd.nAxisRealPos[0] -= 0.2;
			gd.nAxisRealPos[1] -= 0.1;
		}
		else if(i < 6000)
		{
			gd.nAxisRealPos[0] += 0.8;
			//gd.nAxisRealPos[1] -= 0.1;
		}
		else if(i < 7000)
		{
			gd.nAxisRealPos[0] -= 0.1;
			gd.nAxisRealPos[1] -= 0.1;
		}
		else if(i < 8000)
		{
			gd.nAxisRealPos[0] += 0.3;
			gd.nAxisRealPos[1] -= 0.1;
		}
		else if(i < 9000)
		{
			gd.nAxisRealPos[0] -= 0.1;
			gd.nAxisRealPos[1] += 0.1;
		}
		//gd.nAxisRealPos[0] = i * 0.1;
		else
			continue;

		g_Graph.GraphAddPoint(gd);
		Sleep(CNC_GRAPH_PERIOD_TIME);
		i++;
	}
	return dwReturn;
}

void CGraphTraceDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	if(g_bInterruptFlag)
	{
		g_Graph.GraphInit();
		g_Graph.GraphSetPaintFlag(TRUE);

		g_bInterruptFlag = false;
		m_hGraphTraceHandle = CreateThread(NULL, 0, Graph_Task, NULL, 0, NULL);
		m_hPutDataHandle = CreateThread(NULL, 0, PutData_Task, NULL, 0, NULL);
	}

}

void CGraphTraceDlg::OnBnClickedButtonEnd()
{
	// TODO: Add your control notification handler code here
	g_bInterruptFlag = true;
	g_Graph.GraphSetPaintFlag(FALSE);
}

void CGraphTraceDlg::OnBnClickedBtnOpenDXF()
{
	// TODO: Add your control notification handler code here
	CFileDialog dxfDlg(TRUE);
	dxfDlg.m_ofn.lpstrFilter = L"DXF Files(*.dxf)\0*.dxf\0All Files(*.*)\0*.*\0\0";
	dxfDlg.m_ofn.lpstrDefExt = L"dxf";
	if(IDOK != dxfDlg.DoModal())
	{
		return;
	}

	g_Graph.LoadDxfFile(dxfDlg.GetPathName());


}
