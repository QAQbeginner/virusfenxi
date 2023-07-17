// CKill.cpp: 实现文件
//

#include "pch.h"
#include "PandaKill.h"
#include "CKill.h"
#include "afxdialogex.h"


// CKill 对话框

IMPLEMENT_DYNAMIC(CKill, CDialogEx)

CKill::CKill(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PANDAKILL_DIALOG, pParent)
{

}

CKill::~CKill()
{
}

void CKill::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKill, CDialogEx)
END_MESSAGE_MAP()


// CKill 消息处理程序
