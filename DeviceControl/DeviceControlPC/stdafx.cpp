// stdafx.cpp : source file that includes just the standard includes
// GSM_TopUp.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CWnd *m_gWnd = NULL;

HINSTANCE hDbLib			= NULL;
void* g_dbConx				= NULL;

char *GetAString(const TCHAR *t) 
{
	if(!t) return 0;

#ifdef _UNICODE
	char *ret;

	int code_page = CP_ACP;
	//if(ServiceExists(MS_LANGPACK_GETCODEPAGE)) code_page = CallService(MS_LANGPACK_GETCODEPAGE, 0, 0);
	int size = WideCharToMultiByte(code_page, 0, t, -1, 0, 0, 0, 0);
	ret = (char *)malloc(size);
	WideCharToMultiByte(code_page, 0, t, -1, ret, size, 0, 0);
	return ret;
#else
	return _strdup(t);
#endif
}

TCHAR *GetTString(const char *asc) {
	if(!asc) return 0;

#ifdef _UNICODE 
	wchar_t *ret;
	int code_page = CP_ACP;
	//if(ServiceExists(MS_LANGPACK_GETCODEPAGE)) code_page = CallService(MS_LANGPACK_GETCODEPAGE, 0, 0);
	int size = MultiByteToWideChar(code_page, MB_PRECOMPOSED, asc, -1, 0, 0);
	ret = (wchar_t *)malloc(sizeof(wchar_t) * size);
	MultiByteToWideChar(code_page, MB_PRECOMPOSED, asc, -1, ret, size);
	return ret;
#else
	return _strdup(asc);
#endif
}
