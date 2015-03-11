
#include "CommonMethod.h"
#include "stdafx.h"
#include "Wtsapi32.h"
#include <string>
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

std::string GetModuleProfileName(std::string basic_file_name)
{
    char FilePath[MAX_PATH];
    ::GetModuleFileNameA(NULL, FilePath, MAX_PATH);
    (strrchr(FilePath, '\\'))[1] = 0;
    std::string retpath(FilePath);
    return retpath + basic_file_name;
}