#pragma once
class CTool
{
public:
	// ��è����רɱ���ܣ�1.ɾ���ڸ���·�������ɵ�.ini �ļ�
	static void Killini(CString& PathString, CListCtrl& PEList);
	// ��è����רɱ���ܣ�2.��������
	static void KillProcess();
	// �����ļ����ҵ�PE�ļ�
	static void AllPE(CListCtrl& PEList, CString& PathString);
	// ��è����רɱ���ܣ�3.�޸�����Ⱦ��exe�ļ�
	static void FixPEfile(CString& PathString);
	// ��ʼ���б�
	static void SetList(CListCtrl& PEList);
	// ��è����רɱ���ܣ�4.ȥ��ע���
	static void DelRegister();
	// ��ȡ�ļ�����ȡ�ļ���С
	static char* GetFileBuf(CString& pstrFilePath, DWORD* FileSize);
	// �ж��ļ��Ƿ񱻸�Ⱦ
	static BOOL isRejectPE(CString& PathString);
	// ɾ���ļ�����
	static BOOL delFile(const WCHAR* PathString);
};

