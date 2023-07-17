
// PandaKillDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PandaKill.h"
#include "PandaKillDlg.h"
#include "afxdialogex.h"
#include"CTool.h"
#include "DelIniPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 全局变量路径
CString g_Path;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPandaKillDlg 对话框



CPandaKillDlg::CPandaKillDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PANDAKILL_DIALOG, pParent)
	, StaticString1(_T(""))
	, PathString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPandaKillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, OrderPath);
	DDX_Control(pDX, IDC_EDIT1, StaticString);
	DDX_Text(pDX, IDC_EDIT1, PathString);
	// DDX_Control(pDX, IDC_LIST1, PEList);
	DDX_Control(pDX, IDC_LIST2, PEList); 
}

BEGIN_MESSAGE_MAP(CPandaKillDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, &CPandaKillDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON1, &CPandaKillDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPandaKillDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPandaKillDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPandaKillDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPandaKillDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CPandaKillDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CPandaKillDlg 消息处理程序

BOOL CPandaKillDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CTool::SetList(PEList);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPandaKillDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPandaKillDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPandaKillDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
	获取指定目录下路径
*/
CString GetPathFrom()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("从下面选文件夹目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return CString("");
	}
	SHGetPathFromIDList(idl, szBuffer);
	return szBuffer;
}
// 指定文件路径
void CPandaKillDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ThisPath = GetPathFrom();
	if (m_ThisPath == TEXT(""))
	{
		OrderPath.SetCheck(FALSE);
		return;
	}
	StaticString.SetWindowText(m_ThisPath);
	PathString.SetString(m_ThisPath);
}

// 删除ini文件
void CPandaKillDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	g_Path = PathString;
	DelIniPath Ini;
	Ini.DoModal();
}

// 查杀PE文件
void CPandaKillDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	PEList.DeleteAllItems();
	CTool::AllPE(PEList, m_ThisPath);
}

// 恢复PE文件
void CPandaKillDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < PEList.GetItemCount(); i++)
	{
		CTool::FixPEfile(PEList.GetItemText(i, 1));
	}
}

// 修复注册表
void CPandaKillDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CTool::DelRegister();
}

// 结束相关进程
void CPandaKillDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CTool::KillProcess();
}

// 删除C盘根目录下病毒文件
void CPandaKillDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CTool::delFile(L"C:\\autorun.inf");
	CTool::delFile(L"C:\\setup.exe");
	CTool::delFile(L"C:\\Windows\\System32\\drivers\\spo0lsv.exe");
}
