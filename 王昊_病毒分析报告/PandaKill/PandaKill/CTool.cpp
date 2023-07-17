#include "pch.h"
#include "CTool.h"
#include<TlHelp32.h>
// 熊猫烧香专杀功能：1.删除在各个路径下生成的.ini 文件
void CTool::Killini(CString& PathString, CListCtrl& IniList)
{
	// 拼接路径
	WIN32_FIND_DATA FileInfo = { 0 };
	CString FindPath = PathString + L"\\*";
	HANDLE FindHandle = FindFirstFile(FindPath, &FileInfo);
	if (FindHandle != INVALID_HANDLE_VALUE)
	{
		int idex = 0;
		do {
			// 如果当前遍历到的是一个目录，那么就递归
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 需要在遍历的路径中排除 . 和 .. 路径
				if (wcscmp(FileInfo.cFileName, L".") && wcscmp(FileInfo.cFileName, L".."))
				{
					CString FilePath = PathString + L"\\" + FileInfo.cFileName;
					Killini(FilePath,IniList);
				}
			}
			/*
				如果不是目录的话，就进行文件名字对比，并删除
			*/
			else
			{
				if (lstrcmp(FileInfo.cFileName, L"Desktop_.ini") == 0)
				{
					CString FilePath = PathString + L"\\" + FileInfo.cFileName;
					IniList.InsertItem(idex, FilePath);
					// 去除文件的隐藏、系统以及只读属性
					DWORD dwFileAttributes = GetFileAttributes(FilePath);
					dwFileAttributes &= ~FILE_ATTRIBUTE_HIDDEN;
					dwFileAttributes &= ~FILE_ATTRIBUTE_SYSTEM;
					dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
					SetFileAttributes(FilePath, dwFileAttributes);
					DeleteFileW(FilePath);
					idex++;
				}
			}
		} while (FindNextFile(FindHandle, &FileInfo));// 如果文件遍历成功，就继续遍历下一个文件
		FindClose(FindHandle);
	}
}
// 熊猫烧香专杀功能：2.结束指定进程
void CTool::KillProcess() 
{
	// 创建快照句柄
	HANDLE Handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Handle != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 Processentry = { sizeof(PROCESSENTRY32) };
		BOOL Success = Process32First(Handle, &Processentry);
		if (Success != FALSE)
		{
			do {
				if (wcscmp(Processentry.szExeFile, L"spo0lsv.exe") == 0)
				{
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Processentry.th32ProcessID);
					TerminateProcess(hProcess, 1);
					CloseHandle(hProcess);
				}
			} while (Process32Next(Handle, &Processentry));
		}
	}
}
// 熊猫烧香专杀功能：3.修复被感染的exe文件
void CTool::FixPEfile(CString& PathString)
{
	CHAR* pFilebuf = NULL;
	//打开文件获取句柄
	HANDLE hFile = CreateFile(PathString,
		GENERIC_READ | GENERIC_WRITE,
		FALSE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL,"文件打开失败","修复exe",NULL);
		return ;
	}
	//获取文件大小
	DWORD FileSize = GetFileSize(hFile, NULL);

	pFilebuf = new CHAR[FileSize]{};
	//读文件
	DWORD dwCount = 1;
	BOOL bRet = ReadFile(hFile, pFilebuf, FileSize, &dwCount, NULL);

	if (!bRet)
	{
		//释放资源 返回失败
		CloseHandle(hFile);
		delete pFilebuf;
		return ;
	}
	char* pFileOffset = pFilebuf + 0x1F000;

	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	WriteFile(hFile, pFileOffset, FileSize - 0x1F000 - 0x18, &dwCount, NULL);
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	MessageBox(0, 0, L"恢复成功", 0);
	delete[] pFilebuf;
	return ;
}
// 遍历文件，找到PE文件并判断PE文件是否被感染
void CTool::AllPE(CListCtrl& PEList, CString& PathString)
{
	CString FilePath = PathString + L"\\*";
	WIN32_FIND_DATAW FileInfo = {sizeof(WIN32_FIND_DATAW)};
	HANDLE FileHandle=FindFirstFile(FilePath, &FileInfo);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		int idex = 0;
		do {
			// 如果当前遍历到的是一个目录，那么就递归
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 需要在遍历的路径中排除 . 和 .. 路径
				if (wcscmp(FileInfo.cFileName, L".") && wcscmp(FileInfo.cFileName, L".."))
				{
					if (lstrcmpi(FileInfo.cFileName, L"WINDOWS") == 0)
					{
						continue;
					}
					CString FilePath = PathString + L"\\" + FileInfo.cFileName;
					AllPE(PEList,FilePath);
				}
			}
			/*
				如果不是目录的话，就进行文件名字对比，并删除
			*/
			else
			{
				CString Str = FileInfo.cFileName;
				CString PerPath = PathString +L"\\"+ Str;
				// CString PerPath1 = PathString + L"\\" + Str;
				int nPos = Str.ReverseFind('.');
				CString StrCut = Str.Right(Str.GetLength() - nPos - 1);
				if (wcscmp(StrCut, L"exe") == 0)
				{
					if (isRejectPE(PerPath) == TRUE)
					{
						PEList.InsertItem(idex, Str);
						PEList.SetItemText(idex, 1, PerPath);
						idex++;
					}
				}
				// idex++;
			}
		} while (FindNextFile(FileHandle, &FileInfo));
	}
}
// 初始化列表
void CTool::SetList(CListCtrl& PEList)
{
	/*
	 设置文件列表风格
	*/
	DWORD Foldstyle = PEList.GetExtendedStyle();
	PEList.SetExtendedStyle(Foldstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect Frc;
	PEList.GetClientRect(Frc);
	int FWidth = Frc.Width();
	/*
	 插入列表头
	*/
	PEList.InsertColumn(0, L"被感染的PE文件", 0, FWidth/3);
	PEList.InsertColumn(1, L"文件路径", 0, 2*FWidth/3);
}
// 熊猫烧香专杀功能：4.去除注册表
void CTool::DelRegister()
{
	// 首先检查启动项
	WCHAR RegRun[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKeyHKCU = NULL;
	LONG lSize = MAXBYTE;
	char cData[MAXBYTE] = { 0 };

	long lRet = RegOpenKey(HKEY_CURRENT_USER, RegRun, &hKeyHKCU);
	if (lRet == ERROR_SUCCESS)
	{
		lRet = RegQueryValueEx(hKeyHKCU, L"svcshare", NULL, NULL, (unsigned char*)cData, (unsigned long*)&lSize);
		if (lRet == ERROR_SUCCESS)
		{
			if (strcmp(cData, "C:\\WINDOWS\\system32\\drivers\\spo0lsv.exe") == 0)
			{
				// printf("注册表启动项中存在病毒信息\n");
				MessageBox(0, L"注册表启动项中存在病毒信息", 0, 0);
			}

			lRet = RegDeleteValue(hKeyHKCU, L"svcshare");
			if (lRet == ERROR_SUCCESS)
			{
				// printf("注册表启动项中的病毒信息已删除！\n");
				MessageBox(0, L"注册表启动项中的病毒信息已删除!", L"注册表启动项中的病毒信息已删除!", 0);
			}
			else
			{
				// printf("注册表启动项中的病毒信息无法删除\n");
				MessageBox(0, 0, L"注册表启动项中的病毒信息无法删除\n", 0);
			}
		}
		else
		{
			// printf("注册表启动项中不存在病毒信息\n");
			MessageBox(0, 0, L"注册表启动项中不存在病毒信息", 0);
		}
		RegCloseKey(hKeyHKCU);
	}
	else
	{
		MessageBox(0, 0, L"注册表启动项信息读取失败", 0);
		// printf("注册表启动项信息读取失败\n");
	}
	// 接下来修复文件的隐藏显示，需要将CheckedValue的值设置为1
	WCHAR RegHide[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\Folder\\Hidden\\SHOWALL";
	HKEY hKeyHKLM = NULL;
	DWORD dwFlag = 1;

	long lRetHide = RegOpenKey(HKEY_LOCAL_MACHINE, RegHide, &hKeyHKLM);
	if (lRetHide == ERROR_SUCCESS)
	{
		// printf("检测注册表的文件隐藏选项...\r\n");
		if (ERROR_SUCCESS == RegSetValueEx(
			hKeyHKLM,             //subkey handle  
			L"CheckedValue",       //value name  
			0,                    //must be zero  
			REG_DWORD,            //value type  
			(CONST BYTE*) & dwFlag, //pointer to value data  
			4))                   //length of value data
		{
			// printf("注册表修复完毕！\n");
			MessageBox(0, 0, L"注册表修复完毕！", 0);
		}
		else
		{
			// printf("无法恢复注册表的文件隐藏选项\n");
			MessageBox(0, 0, L"无法恢复注册表的文件隐藏选项！", 0);
		}
	}
}
// 读取文件并获取文件大小
char* CTool::GetFileBuf(CString& pstrFilePath, DWORD* FileSize)
{
	char* pFilebuf = NULL;
	//打开文件获取句柄
	HANDLE hFile = CreateFile(pstrFilePath,
		GENERIC_READ,
		FALSE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("文件打开失败\n");
		return 0;
	}

	//获取文件大小
	*FileSize = GetFileSize(hFile, NULL);

	pFilebuf = new char[*FileSize]{};
	//读文件
	DWORD dwCount = 1;
	BOOL bRet = ReadFile(hFile, pFilebuf, *FileSize, &dwCount, NULL);

	if (bRet)
	{
		CloseHandle(hFile);
		return pFilebuf;
	}
	//释放资源
	CloseHandle(hFile);
	delete pFilebuf;
	return 0;
}
// 判断文件是否被感染
BOOL CTool::isRejectPE(CString& PathString)
{
	//感染文件最后一个字节为01
	//向前找到00的后五个字节是WhBoy
	CHAR* pFileBuf = NULL;
	DWORD dwFileSize = 0;
	pFileBuf = GetFileBuf(PathString, &dwFileSize);
	if (pFileBuf == 0)
	{
		return false;
	}
	BYTE* pFileOffset = (BYTE*)pFileBuf;
	*pFileOffset;
	pFileOffset += (dwFileSize - 1);
	//判断是否为0x01
	if (*pFileOffset != 0x01)
	{
		delete[] pFileBuf;
		return  false;
	}
	while (*pFileOffset != 0x00)
	{
		--pFileOffset;
	}
	pFileOffset++;
	CHAR temp[6] = { 0 };
	memcpy_s(temp, 5, pFileOffset, 5);
	if (!strcmp(temp, "WhBoy"))
	{
		delete[] pFileBuf;
		return  true;
	}
	delete[] pFileBuf;
	return  false;
}
// 删除文件操作
BOOL CTool::delFile(const WCHAR* PathString)
{
	// 去除文件的隐藏、系统以及只读属性
	DWORD dwFileAttributes = GetFileAttributes(PathString);
	dwFileAttributes &= ~FILE_ATTRIBUTE_HIDDEN;
	dwFileAttributes &= ~FILE_ATTRIBUTE_SYSTEM;
	dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
	SetFileAttributes(PathString, dwFileAttributes);

	int delRet = DeleteFile(PathString);
	if (delRet)
	{
		return TRUE;
	}
	else
	{
		MessageBox(0, 0, L"删除失败", 0);
		return FALSE;
	}
}