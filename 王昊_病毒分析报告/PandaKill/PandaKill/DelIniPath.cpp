// DelIniPath.cpp: 实现文件
//

#include "pch.h"
#include "PandaKill.h"
#include "DelIniPath.h"
#include "afxdialogex.h"
#include "CTool.h"

extern CString g_Path;
// DelIniPath 对话框

IMPLEMENT_DYNAMIC(DelIniPath, CDialogEx)

DelIniPath::DelIniPath(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

DelIniPath::~DelIniPath()
{
}

void DelIniPath::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, iniList);
}


BEGIN_MESSAGE_MAP(DelIniPath, CDialogEx)
END_MESSAGE_MAP()


// DelIniPath 消息处理程序


BOOL DelIniPath::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/*
	 设置文件列表风格
	*/
	DWORD Foldstyle = iniList.GetExtendedStyle();
	iniList.SetExtendedStyle(Foldstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect Frc;
	iniList.GetClientRect(Frc);
	int FWidth = Frc.Width();
	/*
	 插入列表头
	*/
	iniList.InsertColumn(1, L"ini文件路径", 0, 2 * FWidth);


	/* 
	进行删除
	*/
	CTool::Killini(g_Path,iniList);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
