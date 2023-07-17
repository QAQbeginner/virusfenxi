#pragma once


// DelIniPath 对话框

class DelIniPath : public CDialogEx
{
	DECLARE_DYNAMIC(DelIniPath)

public:
	DelIniPath(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DelIniPath();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl iniList;
	virtual BOOL OnInitDialog();
};
