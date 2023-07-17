#pragma once
class CTool
{
public:
	// 熊猫烧香专杀功能：1.删除在各个路径下生成的.ini 文件
	static void Killini(CString& PathString, CListCtrl& PEList);
	// 熊猫烧香专杀功能：2.结束进程
	static void KillProcess();
	// 遍历文件，找到PE文件
	static void AllPE(CListCtrl& PEList, CString& PathString);
	// 熊猫烧香专杀功能：3.修复被感染的exe文件
	static void FixPEfile(CString& PathString);
	// 初始化列表
	static void SetList(CListCtrl& PEList);
	// 熊猫烧香专杀功能：4.去除注册表
	static void DelRegister();
	// 读取文件并获取文件大小
	static char* GetFileBuf(CString& pstrFilePath, DWORD* FileSize);
	// 判断文件是否被感染
	static BOOL isRejectPE(CString& PathString);
	// 删除文件操作
	static BOOL delFile(const WCHAR* PathString);
};

