
// wakeupdisk.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CwakeupdiskApp: 
// �йش����ʵ�֣������ wakeupdisk.cpp
//

class CwakeupdiskApp : public CWinApp
{
public:
    CwakeupdiskApp();

    // ��д
public:
    virtual BOOL InitInstance();

    // ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CwakeupdiskApp theApp;