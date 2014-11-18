
#include "CommonMethod.h"
#include "stdafx.h"
#include "Wtsapi32.h"
#pragma comment(lib,"Wtsapi32.lib")

BOOL GetLogUser(CString& str_name)
{
    BOOL bRet = FALSE;
    str_name = _T("");
    //for xp or above 
    TCHAR *szLogName = NULL;
    DWORD dwSize = 0;
    if (WTSQuerySessionInformation(((HANDLE)NULL),
        ((DWORD)-1),
        WTSUserName,
        &szLogName,
        &dwSize))
    {
        str_name = szLogName;
        WTSFreeMemory(szLogName);
        bRet = TRUE;
    }

    return bRet;
}
