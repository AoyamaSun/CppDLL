/********************************************************************
	Created:	2012/05/19  19:20
	Filename: 	CppDLL.cpp
	Author:		rrrfff
	Url:	    http://blog.csdn.net/rrrfff
*********************************************************************/
#include "CppDLL_Class.h"

//-------------------------------------------------------------------------

void main()
{
	printf("��ѡ��Ҫ������DLL..." RLIB_NEWLINEA);

	TCHAR szFiles[RLIB_DEFAULT_BUFFER_SIZE]; 
	szFiles[0]        = _T('\0');
	OPENFILENAME ofn  = { sizeof(OPENFILENAME) };
	ofn.lpstrTitle    = _T("ѡ��C++��̬���ӿ�");
	ofn.lpstrFilter   = _T("��̬���ӿ�*.dll\0*.dll\0�����ļ�*.*\0*.*\0");
	ofn.lpstrFile     = szFiles;
	ofn.nMaxFile      = RLIB_COUNTOF_STR(szFiles);
	ofn.Flags         = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
	while (!GetOpenFileName(&ofn)) {
		printf("    ������ѡ��һ���ļ�! error = 0x%.8lX" RLIB_NEWLINEA, CommDlgExtendedError());
		Thread::Sleep(1000);
	}
	printf("   ������Ҫ����һ��ʱ��, �����ĵȴ�..." RLIB_NEWLINEA);

	intptr_t count = 0;
	if (szFiles[ofn.nFileOffset - 1] != _T('\0')) {
		CppDLL cpp;
		if (cpp.Work(szFiles)) {
			printf("        %s" RLIB_NEWLINEA, RT2A(cpp.m_dll_path + _R("�����ɹ�.")).toGBK());		
		} else {
			printf("        %s" RLIB_NEWLINEA, RT2A(cpp.m_dll_path + _R("����ʧ��, ��C++��д��DLL?")).toGBK());
		} //if
		count = 1;
	} else {
		static String common_dir;
		ManagedObject<ThreadPool> pool = pool.construct();
		if (pool.IsNull()) return;

		common_dir = Path::AddBackslash(szFiles); // Ŀ¼		
		TCHAR *pt  = szFiles + ofn.nFileOffset;
		while (*pt != _T('\0')) {
			pool->InvokeAndWait<TCHAR *>([](TCHAR *pt) {
				CppDLL cpp;
				if (cpp.Work(common_dir + pt)) {
					printf("        %s" RLIB_NEWLINEA, RT2A(cpp.m_dll_path + _R("�����ɹ�.")).toGBK());
				} else {
					printf("        %s" RLIB_NEWLINEA, RT2A(cpp.m_dll_path + _R("����ʧ��, ��C++��д��DLL?") + (IO::File::TryDelete(cpp.m_dll_path) ? _R("����ɾ����") : _R("��ɾ��ʧ�ܡ�"))).toGBK());
				} //if
			}, pt);

			pt += (TLEN(pt) + 1);
			++count;
		}

		pool->WaitForTasksComplete();
	} //if

	printf("   �����ɹ����, %Id�ļ�������." RLIB_NEWLINEA, count);
};