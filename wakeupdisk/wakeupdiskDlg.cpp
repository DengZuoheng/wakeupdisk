
// wakeupdiskDlg.cpp : ʵ���ļ�
//
#pragma comment(lib,"Wtsapi32.lib")
#include "stdafx.h"
#include "wakeupdisk.h"
#include "wakeupdiskDlg.h"
#include "afxdialogex.h"
#include "Wtsapi32.h"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <fstream>
#include <exception>
using std::string;
using std::vector;
using std::exception;
using namespace boost::property_tree;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SHOWTASK (WM_USER+100)


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CwakeupdiskDlg �Ի���



CwakeupdiskDlg::CwakeupdiskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwakeupdiskDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwakeupdiskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CwakeupdiskDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CwakeupdiskDlg::OnBnClickedOk)
    ON_MESSAGE(WM_SHOWTASK, &CwakeupdiskDlg::OnShowtask)
    ON_BN_CLICKED(IDCANCEL, &CwakeupdiskDlg::OnBnClickedCancel)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_CHECKC, &CwakeupdiskDlg::OnBnClickedCheckc)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CwakeupdiskDlg ��Ϣ�������

BOOL CwakeupdiskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    m_vecSetting.resize(24);
    ReadJSONFile();
    Reset();
    SetTimer(1, m_fre, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CwakeupdiskDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CwakeupdiskDlg::ReadJSONFile()
{
    try{
        read_json("\\init.json", pt);
        m_runonstartup = pt.get<bool>("runonstartup");
        m_fre = pt.get<int>("frequency");
        Ptreetovec();
    }
    catch (exception& e){
        string error = e.what();
        CString cstr;
        
        MessageBox(TEXT("init.json��ʧ����!"));
        
        CDialogEx::OnCancel();

    }
}

void CwakeupdiskDlg::VectoPtree()
{
    for (int i = 0; i < m_vecSetting.size(); ++i)
    {
        string strQuery = "C";
        strQuery[0] += i;
        pt.put("disksetting."+strQuery, m_vecSetting[i] == 1 ? true : false);
    }
}

void CwakeupdiskDlg::Ptreetovec()
{
    for (int i = 0; i < m_vecSetting.size(); ++i)
    {
        string strQuery = "C";
        strQuery[0] += i;
        m_vecSetting[i] = static_cast<int>(pt.get<bool>("disksetting." + strQuery));
    }
}

void CwakeupdiskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CwakeupdiskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CwakeupdiskDlg::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    //CDialogEx::OnOK();
    //��ȡitem
    //����ɾ��ָ��
    CButton* pButton0 = (CButton*)GetDlgItem(IDC_CHECKC);
    CButton* pButton1 = (CButton*)GetDlgItem(IDC_CHECKD);
    CButton* pButton2 = (CButton*)GetDlgItem(IDC_CHECKE);
    CButton* pButton3 = (CButton*)GetDlgItem(IDC_CHECKF);
    CButton* pButton4 = (CButton*)GetDlgItem(IDC_CHECKG);
    CButton* pButton5 = (CButton*)GetDlgItem(IDC_CHECKH);
    CButton* pButton6 = (CButton*)GetDlgItem(IDC_CHECKI);
    CButton* pButton7 = (CButton*)GetDlgItem(IDC_CHECKJ);
    CButton* pButton8 = (CButton*)GetDlgItem(IDC_CHECKK);
    CButton* pButton9 = (CButton*)GetDlgItem(IDC_CHECKL);
    CButton* pButton10 = (CButton*)GetDlgItem(IDC_CHECKM);
    CButton* pButton11 = (CButton*)GetDlgItem(IDC_CHECKN);
    CButton* pButton12 = (CButton*)GetDlgItem(IDC_CHECKO);
    CButton* pButton13 = (CButton*)GetDlgItem(IDC_CHECKP);
    CButton* pButton14 = (CButton*)GetDlgItem(IDC_CHECKQ);
    CButton* pButton15 = (CButton*)GetDlgItem(IDC_CHECKR);
    CButton* pButton16 = (CButton*)GetDlgItem(IDC_CHECKS);
    CButton* pButton17 = (CButton*)GetDlgItem(IDC_CHECKT);
    CButton* pButton18 = (CButton*)GetDlgItem(IDC_CHECKU);
    CButton* pButton19 = (CButton*)GetDlgItem(IDC_CHECKV);
    CButton* pButton20 = (CButton*)GetDlgItem(IDC_CHECKW);
    CButton* pButton21 = (CButton*)GetDlgItem(IDC_CHECKX);
    CButton* pButton22 = (CButton*)GetDlgItem(IDC_CHECKY);
    CButton* pButton23 = (CButton*)GetDlgItem(IDC_CHECKZ);
    m_vecSetting[0] = pButton0->GetCheck();
    m_vecSetting[1] = pButton1->GetCheck();
    m_vecSetting[2] = pButton2->GetCheck();
    m_vecSetting[3] = pButton3->GetCheck();
    m_vecSetting[4] = pButton4->GetCheck();
    m_vecSetting[5] = pButton5->GetCheck();
    m_vecSetting[6] = pButton6->GetCheck();
    m_vecSetting[7] = pButton7->GetCheck();
    m_vecSetting[8] = pButton8->GetCheck();
    m_vecSetting[9] = pButton9->GetCheck();
    m_vecSetting[10] = pButton10->GetCheck();
    m_vecSetting[11] = pButton11->GetCheck();
    m_vecSetting[12] = pButton12->GetCheck();
    m_vecSetting[13] = pButton13->GetCheck();
    m_vecSetting[14] = pButton14->GetCheck();
    m_vecSetting[15] = pButton15->GetCheck();
    m_vecSetting[16] = pButton16->GetCheck();
    m_vecSetting[17] = pButton17->GetCheck();
    m_vecSetting[18] = pButton18->GetCheck();
    m_vecSetting[19] = pButton19->GetCheck();
    m_vecSetting[20] = pButton20->GetCheck();
    m_vecSetting[21] = pButton21->GetCheck();
    m_vecSetting[22] = pButton22->GetCheck();
    m_vecSetting[23] = pButton23->GetCheck();

    CButton* pButton = (CButton*)GetDlgItem(IDC_START);
    m_runonstartup = pButton->GetCheck();
    pt.put("runonstartup", m_runonstartup == 1 ? true : false);
    m_fre = GetDlgItemInt(IDC_EDIT_FRE);
    pt.put("frequency", m_fre);
    KillTimer(1);
    SetTimer(1, m_fre, NULL);
    VectoPtree();
    //SetRunOnStartUp(m_runonstartup);
    ToTray();
}

void CwakeupdiskDlg::SetRunOnStartUp(bool bFlag)
{
    /*CString strName;
    strName = _T("");
    //for xp or above 
    TCHAR *szLogName = NULL;
    DWORD dwSize = 0;
    if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
        WTS_CURRENT_SESSION,
        WTSUserName,
        &szLogName,
        &dwSize))
    {
        strName = szLogName;
        WTSFreeMemory(szLogName);
    }
    HKEY hKey;
    //�ҵ�ϵͳ��������   
    CString lpRun = CString(_T("C:\\Users\\"))
        +strName
        +CString(_T("\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup"));
    //��������Key   
    long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
    if (lRet == ERROR_SUCCESS)
    {
        TCHAR pFileName[MAX_PATH] = { 0 };
        //�õ����������ȫ·��   
        DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
        //���һ����Key,������ֵ // �����"test"��Ӧ�ó������֣����Ӻ�׺.exe��  
        if (bFlag == true)
        {
            lRet = RegSetValueEx(hKey, _T("wakeupdisk"), 0, REG_SZ, (BYTE *)pFileName, dwRet);
        }
        else
        {
            lRet = RegDeleteValue(hKey, _T("wakeupdisk"));
        }
        
        //�ر�ע���   
        RegCloseKey(hKey);
        if (lRet != ERROR_SUCCESS)
        {
            AfxMessageBox(_T("ϵͳ��������,������ɿ�����������"));
        }
        else
        {
            AfxMessageBox(_T("���������ɹ�"));
        }
    }
    else
        AfxMessageBox(_T("ϵͳ��������,������ɿ�����������"));*/
}

void CwakeupdiskDlg::ToTray()
{
   
    NOTIFYICONDATA nid;
    nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
    nid.hWnd = this->m_hWnd;
    nid.uID = IDR_MAINFRAME;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_SHOWTASK;//�Զ������Ϣ����
    nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
    //strcpy(nid.szTip, TEXT("�ƻ���������"));//��Ϣ��ʾ��Ϊ���ƻ��������ѡ�
    wsprintf(nid.szTip, TEXT("%s"), TEXT("���̲���"));
    Shell_NotifyIcon(NIM_ADD, &nid);//�����������ͼ��
    ShowWindow(SW_HIDE);//����������
   
}


afx_msg LRESULT CwakeupdiskDlg::OnShowtask(WPARAM wParam, LPARAM lParam)
{
    if (lParam == WM_LBUTTONDOWN)
    {
        ShowWindow(SW_SHOW);//����������
    }
    return 0;
}

//
void CwakeupdiskDlg::Reset()
{
    //reset disksetting
    CButton* pButton0 = (CButton*)GetDlgItem(IDC_CHECKC);
    CButton* pButton1 = (CButton*)GetDlgItem(IDC_CHECKD);
    CButton* pButton2 = (CButton*)GetDlgItem(IDC_CHECKE);
    CButton* pButton3 = (CButton*)GetDlgItem(IDC_CHECKF);
    CButton* pButton4 = (CButton*)GetDlgItem(IDC_CHECKG);
    CButton* pButton5 = (CButton*)GetDlgItem(IDC_CHECKH);
    CButton* pButton6 = (CButton*)GetDlgItem(IDC_CHECKI);
    CButton* pButton7 = (CButton*)GetDlgItem(IDC_CHECKJ);
    CButton* pButton8 = (CButton*)GetDlgItem(IDC_CHECKK);
    CButton* pButton9 = (CButton*)GetDlgItem(IDC_CHECKL);
    CButton* pButton10 = (CButton*)GetDlgItem(IDC_CHECKM);
    CButton* pButton11 = (CButton*)GetDlgItem(IDC_CHECKN);
    CButton* pButton12 = (CButton*)GetDlgItem(IDC_CHECKO);
    CButton* pButton13 = (CButton*)GetDlgItem(IDC_CHECKP);
    CButton* pButton14 = (CButton*)GetDlgItem(IDC_CHECKQ);
    CButton* pButton15 = (CButton*)GetDlgItem(IDC_CHECKR);
    CButton* pButton16 = (CButton*)GetDlgItem(IDC_CHECKS);
    CButton* pButton17 = (CButton*)GetDlgItem(IDC_CHECKT);
    CButton* pButton18 = (CButton*)GetDlgItem(IDC_CHECKU);
    CButton* pButton19 = (CButton*)GetDlgItem(IDC_CHECKV);
    CButton* pButton20 = (CButton*)GetDlgItem(IDC_CHECKW);
    CButton* pButton21 = (CButton*)GetDlgItem(IDC_CHECKX);
    CButton* pButton22 = (CButton*)GetDlgItem(IDC_CHECKY);
    CButton* pButton23 = (CButton*)GetDlgItem(IDC_CHECKZ);
    pButton0->SetCheck(m_vecSetting[0]);
    pButton1->SetCheck(m_vecSetting[1]);
    pButton2->SetCheck(m_vecSetting[2]);
    pButton3->SetCheck(m_vecSetting[3]);
    pButton4->SetCheck(m_vecSetting[4]);
    pButton5->SetCheck(m_vecSetting[5]);
    pButton6->SetCheck(m_vecSetting[6]);
    pButton7->SetCheck(m_vecSetting[7]);
    pButton8->SetCheck(m_vecSetting[8]);
    pButton9->SetCheck(m_vecSetting[9]);
    pButton10->SetCheck(m_vecSetting[10]);
    pButton11->SetCheck(m_vecSetting[11]);
    pButton12->SetCheck(m_vecSetting[12]);
    pButton13->SetCheck(m_vecSetting[13]);
    pButton14->SetCheck(m_vecSetting[14]);
    pButton15->SetCheck(m_vecSetting[15]);
    pButton16->SetCheck(m_vecSetting[16]);
    pButton17->SetCheck(m_vecSetting[17]);
    pButton18->SetCheck(m_vecSetting[18]);
    pButton19->SetCheck(m_vecSetting[19]);
    pButton20->SetCheck(m_vecSetting[20]);
    pButton21->SetCheck(m_vecSetting[21]);
    pButton22->SetCheck(m_vecSetting[22]);
    pButton23->SetCheck(m_vecSetting[23]);
    //reset runonstartup
    CButton* pButton = (CButton*)GetDlgItem(IDC_START);
    pButton->SetCheck(m_runonstartup);
    //reset frequency
    SetDlgItemInt(IDC_EDIT_FRE, m_fre);
}

void CwakeupdiskDlg::OnBnClickedCancel()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    ToTray();
    Reset();
    //CDialogEx::OnCancel();
}

void CwakeupdiskDlg::OnClose()
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (::MessageBox(
        this->m_hWnd,
        TEXT("�رճ�����ܵ���Ӳ������,ȷ���ر�?"),
        TEXT("ȷ���ر�"),
        MB_OKCANCEL) == 1)
    {
        write_json("\\init.json", pt);
        CDialogEx::OnCancel();
        CDialogEx::OnClose();
    }
    
}

void CwakeupdiskDlg::OnBnClickedCheckc()
{
}


void CwakeupdiskDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    for (int i = 0; i < m_vecSetting.size(); ++i)
    {
        
        if (m_vecSetting[i] == 1)
        {
            string drive = "C:\\";
            drive[0] += i;
            drive += "wkd.log";
            std::ofstream fs(drive.c_str());
            fs << "hehe" << std::endl;
            fs.close();
        }
    }
    CDialogEx::OnTimer(nIDEvent);
}
