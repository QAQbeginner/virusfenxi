
// PandaKillDlg.h: 头文件
//

#pragma once


// CPandaKillDlg 对话框
class CPandaKillDlg : public CDialogEx
{
// 构造
public:
	CPandaKillDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PANDAKILL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
public:
	CString m_ThisPath;//指定位置扫描
	CButton OrderPath;
	CString StaticString1;
	CEdit StaticString;
	afx_msg void OnBnClickedButton1();
	CString PathString;
	CListCtrl PEList;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
