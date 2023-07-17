#include "pch.h"
#include "CTool.h"
#include<TlHelp32.h>
// ��è����רɱ���ܣ�1.ɾ���ڸ���·�������ɵ�.ini �ļ�
void CTool::Killini(CString& PathString, CListCtrl& IniList)
{
	// ƴ��·��
	WIN32_FIND_DATA FileInfo = { 0 };
	CString FindPath = PathString + L"\\*";
	HANDLE FindHandle = FindFirstFile(FindPath, &FileInfo);
	if (FindHandle != INVALID_HANDLE_VALUE)
	{
		int idex = 0;
		do {
			// �����ǰ����������һ��Ŀ¼����ô�͵ݹ�
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// ��Ҫ�ڱ�����·�����ų� . �� .. ·��
				if (wcscmp(FileInfo.cFileName, L".") && wcscmp(FileInfo.cFileName, L".."))
				{
					CString FilePath = PathString + L"\\" + FileInfo.cFileName;
					Killini(FilePath,IniList);
				}
			}
			/*
				�������Ŀ¼�Ļ����ͽ����ļ����ֶԱȣ���ɾ��
			*/
			else
			{
				if (lstrcmp(FileInfo.cFileName, L"Desktop_.ini") == 0)
				{
					CString FilePath = PathString + L"\\" + FileInfo.cFileName;
					IniList.InsertItem(idex, FilePath);
					// ȥ���ļ������ء�ϵͳ�Լ�ֻ������
					DWORD dwFileAttributes = GetFileAttributes(FilePath);
					dwFileAttributes &= ~FILE_ATTRIBUTE_HIDDEN;
					dwFileAttributes &= ~FILE_ATTRIBUTE_SYSTEM;
					dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
					SetFileAttributes(FilePath, dwFileAttributes);
					DeleteFileW(FilePath);
					idex++;
				}
			}
		} while (FindNextFile(FindHandle, &FileInfo));// ����ļ������ɹ����ͼ���������һ���ļ�
		FindClose(FindHandle);
	}
}
// ��è����רɱ���ܣ�2.����ָ������
void CTool::KillProcess() 
{
	// �������վ��
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
// ��è����רɱ���ܣ�3.�޸�����Ⱦ��exe�ļ�
void CTool::FixPEfile(CString& PathString)
{
	CHAR* pFilebuf = NULL;
	//���ļ���ȡ���
	HANDLE hFile = CreateFile(PathString,
		GENERIC_READ | GENERIC_WRITE,
		FALSE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL,"�ļ���ʧ��","�޸�exe",NULL);
		return ;
	}
	//��ȡ�ļ���С
	DWORD FileSize = GetFileSize(hFile, NULL);

	pFilebuf = new CHAR[FileSize]{};
	//���ļ�
	DWORD dwCount = 1;
	BOOL bRet = ReadFile(hFile, pFilebuf, FileSize, &dwCount, NULL);

	if (!bRet)
	{
		//�ͷ���Դ ����ʧ��
		CloseHandle(hFile);
		delete pFilebuf;
		return ;
	}
	char* pFileOffset = pFilebuf + 0x1F000;

	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	WriteFile(hFile, pFileOffset, FileSize - 0x1F000 - 0x18, &dwCount, NULL);
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	MessageBox(0, 0, L"�ָ��ɹ�", 0);
	delete[] pFilebuf;
	return ;
}
// �����ļ����ҵ�PE�ļ����ж�PE�ļ��Ƿ񱻸�Ⱦ
void CTool::AllPE(CListCtrl& PEList, CString& PathString)
{
	CString FilePath = PathString + L"\\*";
	WIN32_FIND_DATAW FileInfo = {sizeof(WIN32_FIND_DATAW)};
	HANDLE FileHandle=FindFirstFile(FilePath, &FileInfo);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		int idex = 0;
		do {
			// �����ǰ����������һ��Ŀ¼����ô�͵ݹ�
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// ��Ҫ�ڱ�����·�����ų� . �� .. ·��
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
				�������Ŀ¼�Ļ����ͽ����ļ����ֶԱȣ���ɾ��
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
// ��ʼ���б�
void CTool::SetList(CListCtrl& PEList)
{
	/*
	 �����ļ��б���
	*/
	DWORD Foldstyle = PEList.GetExtendedStyle();
	PEList.SetExtendedStyle(Foldstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect Frc;
	PEList.GetClientRect(Frc);
	int FWidth = Frc.Width();
	/*
	 �����б�ͷ
	*/
	PEList.InsertColumn(0, L"����Ⱦ��PE�ļ�", 0, FWidth/3);
	PEList.InsertColumn(1, L"�ļ�·��", 0, 2*FWidth/3);
}
// ��è����רɱ���ܣ�4.ȥ��ע���
void CTool::DelRegister()
{
	// ���ȼ��������
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
				// printf("ע����������д��ڲ�����Ϣ\n");
				MessageBox(0, L"ע����������д��ڲ�����Ϣ", 0, 0);
			}

			lRet = RegDeleteValue(hKeyHKCU, L"svcshare");
			if (lRet == ERROR_SUCCESS)
			{
				// printf("ע����������еĲ�����Ϣ��ɾ����\n");
				MessageBox(0, L"ע����������еĲ�����Ϣ��ɾ��!", L"ע����������еĲ�����Ϣ��ɾ��!", 0);
			}
			else
			{
				// printf("ע����������еĲ�����Ϣ�޷�ɾ��\n");
				MessageBox(0, 0, L"ע����������еĲ�����Ϣ�޷�ɾ��\n", 0);
			}
		}
		else
		{
			// printf("ע����������в����ڲ�����Ϣ\n");
			MessageBox(0, 0, L"ע����������в����ڲ�����Ϣ", 0);
		}
		RegCloseKey(hKeyHKCU);
	}
	else
	{
		MessageBox(0, 0, L"ע�����������Ϣ��ȡʧ��", 0);
		// printf("ע�����������Ϣ��ȡʧ��\n");
	}
	// �������޸��ļ���������ʾ����Ҫ��CheckedValue��ֵ����Ϊ1
	WCHAR RegHide[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\Folder\\Hidden\\SHOWALL";
	HKEY hKeyHKLM = NULL;
	DWORD dwFlag = 1;

	long lRetHide = RegOpenKey(HKEY_LOCAL_MACHINE, RegHide, &hKeyHKLM);
	if (lRetHide == ERROR_SUCCESS)
	{
		// printf("���ע�����ļ�����ѡ��...\r\n");
		if (ERROR_SUCCESS == RegSetValueEx(
			hKeyHKLM,             //subkey handle  
			L"CheckedValue",       //value name  
			0,                    //must be zero  
			REG_DWORD,            //value type  
			(CONST BYTE*) & dwFlag, //pointer to value data  
			4))                   //length of value data
		{
			// printf("ע����޸���ϣ�\n");
			MessageBox(0, 0, L"ע����޸���ϣ�", 0);
		}
		else
		{
			// printf("�޷��ָ�ע�����ļ�����ѡ��\n");
			MessageBox(0, 0, L"�޷��ָ�ע�����ļ�����ѡ�", 0);
		}
	}
}
// ��ȡ�ļ�����ȡ�ļ���С
char* CTool::GetFileBuf(CString& pstrFilePath, DWORD* FileSize)
{
	char* pFilebuf = NULL;
	//���ļ���ȡ���
	HANDLE hFile = CreateFile(pstrFilePath,
		GENERIC_READ,
		FALSE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("�ļ���ʧ��\n");
		return 0;
	}

	//��ȡ�ļ���С
	*FileSize = GetFileSize(hFile, NULL);

	pFilebuf = new char[*FileSize]{};
	//���ļ�
	DWORD dwCount = 1;
	BOOL bRet = ReadFile(hFile, pFilebuf, *FileSize, &dwCount, NULL);

	if (bRet)
	{
		CloseHandle(hFile);
		return pFilebuf;
	}
	//�ͷ���Դ
	CloseHandle(hFile);
	delete pFilebuf;
	return 0;
}
// �ж��ļ��Ƿ񱻸�Ⱦ
BOOL CTool::isRejectPE(CString& PathString)
{
	//��Ⱦ�ļ����һ���ֽ�Ϊ01
	//��ǰ�ҵ�00�ĺ�����ֽ���WhBoy
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
	//�ж��Ƿ�Ϊ0x01
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
// ɾ���ļ�����
BOOL CTool::delFile(const WCHAR* PathString)
{
	// ȥ���ļ������ء�ϵͳ�Լ�ֻ������
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
		MessageBox(0, 0, L"ɾ��ʧ��", 0);
		return FALSE;
	}
}