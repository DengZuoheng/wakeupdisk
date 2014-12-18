
// wakeupdiskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wakeupdisk.h"
#include "wakeupdiskDlg.h"
#include "afxdialogex.h"
#include "Wtsapi32.h"
#include "CommonMethod.h"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <fstream>
#include <exception>
#include <cstring>
using std::string;
using std::vector;
using std::exception;
using namespace boost::property_tree;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SHOWTASK (WM_USER+100)
#define ID_TIMER 1
#define ID_TIMER_TRAY 2
#define STATUS_CHECKED 1


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
public:
//    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//    ON_WM_CREATE()
//    ON_WM_SHOWWINDOW()
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
    ON_WM_TIMER()
//    ON_WM_CREATE()
//    ON_WM_SHOWWINDOW()
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
    ReadJSONFile();
    Reset();
    SetTimer(ID_TIMER, pt.get<int>("frequency"), NULL);
    
    GetWindowPlacement(&m_wp); //�ָ�ʱ��
    ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//����������ȥ��.


    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    wp.flags = WPF_RESTORETOMAXIMIZED;
    wp.showCmd = SW_HIDE;
    SetWindowPlacement(&wp);

    ToTray();

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
        string jsonpath = GetModuleProfileName("init.json");
        read_json(jsonpath, pt);
        flg_lost_json = false;
    }
    catch (exception& e){
        MessageBox(TEXT("init.json��ʧ����!"));  
        flg_lost_json = true;
        exit(0);
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
    static vector<decltype(IDC_CHECKC)> ctrl_macro_vec = {
        IDC_CHECKC,IDC_CHECKD,IDC_CHECKE,IDC_CHECKF,
        IDC_CHECKG,IDC_CHECKH,IDC_CHECKI,IDC_CHECKJ,
        IDC_CHECKK,IDC_CHECKL,IDC_CHECKN,IDC_CHECKM,
        IDC_CHECKO,IDC_CHECKP,IDC_CHECKQ,IDC_CHECKR,
        IDC_CHECKS,IDC_CHECKT,IDC_CHECKU,IDC_CHECKV,
        IDC_CHECKW,IDC_CHECKX,IDC_CHECKY,IDC_CHECKZ };
    const static int vec_size = static_cast<int>(ctrl_macro_vec.size());
    for (int i = 0; i < vec_size; ++i)
    {
        CButton* pButton = (CButton*)GetDlgItem(ctrl_macro_vec[i]);
        string drive = "C";
        drive[0] += i;
        pt.put("disksetting." + drive, 
            pButton->GetCheck() == STATUS_CHECKED ? true : false);
    }

    CButton* pButton = (CButton*)GetDlgItem(IDC_START);
    pt.put("runonstartup", 
        pButton->GetCheck() == STATUS_CHECKED ? true : false);

    pt.put("frequency", GetDlgItemInt(IDC_EDIT_FRE));
    //reset the timer
    KillTimer(ID_TIMER);
    SetTimer(ID_TIMER, pt.get<int>("frequency"), NULL);

    SetRunOnStartUp(pt.get<bool>("runonstartup"));
    OnTimer(ID_TIMER);
    ToTray();
}

void CwakeupdiskDlg::SetRunOnStartUp(bool bFlag)
{
    
    HKEY RegKey=NULL;
    CString sPath;

    GetModuleFileName(NULL, 
        sPath.GetBufferSetLength(MAX_PATH + 1),
        MAX_PATH);

    sPath.ReleaseBuffer();
    int nPos;
    nPos = sPath.ReverseFind('\\');
    sPath = sPath.Left(nPos);
    CString lpszFile = sPath + "\\wakeupdisk.exe";//���������Ҫ���ҵ�ִ���ļ�����   
    CFileFind fFind;
    BOOL bSuccess;
    bSuccess = fFind.FindFile(lpszFile);
    fFind.Close();

    if (bSuccess)
    {
        CString fullName(lpszFile);

        RegOpenKey(HKEY_LOCAL_MACHINE, 
            TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 
            &RegKey);

        if (bFlag)
        {
            RegSetValueEx(RegKey, 
                TEXT("wakeupdisk"), 
                0, 
                REG_SZ, 
                (BYTE*)(LPCTSTR)fullName, 
                fullName.GetLength() * 2);//�����������Ҫ��ע�����ע�������   
        }
        else
        {
            RegDeleteValue(RegKey, TEXT("wakeupdisk"));
        }
        
        this->UpdateData(FALSE);
    }
    else
    {
        //theApp.SetMainSkin();   
        MessageBox(TEXT("û�ҵ�ִ�г����Զ�����ʧ��"));
        exit(0);
    }

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
    
    wsprintf(nid.szTip, TEXT("%s"), TEXT("���̲���"));
    Shell_NotifyIcon(NIM_ADD, &nid);//�����������ͼ��
    ShowWindow(SW_HIDE);//����������
   
}

afx_msg LRESULT CwakeupdiskDlg::OnShowtask(WPARAM wParam, LPARAM lParam)
{
    if (lParam == WM_LBUTTONDOWN)
    {
        static bool flg_lbd = 0;
        if (flg_lbd == 0)
        {
            SetWindowPlacement(&m_wp);
            flg_lbd++;
        }
        
        ShowWindow(SW_SHOW);//����������
    }
    return 0;
}

//
void CwakeupdiskDlg::Reset()
{
    //reset disksetting
    static vector<decltype(IDC_CHECKC)> ctrl_macro_vec = {
        IDC_CHECKC, IDC_CHECKD, IDC_CHECKE, IDC_CHECKF,
        IDC_CHECKG, IDC_CHECKH, IDC_CHECKI, IDC_CHECKJ,
        IDC_CHECKK, IDC_CHECKL, IDC_CHECKN, IDC_CHECKM,
        IDC_CHECKO, IDC_CHECKP, IDC_CHECKQ, IDC_CHECKR,
        IDC_CHECKS, IDC_CHECKT, IDC_CHECKU, IDC_CHECKV,
        IDC_CHECKW, IDC_CHECKX, IDC_CHECKY, IDC_CHECKZ };
    const static int vec_size = static_cast<int>(ctrl_macro_vec.size());
    for (int i = 0; i < vec_size; ++i)
    {
        CButton* pButton = (CButton*)GetDlgItem(ctrl_macro_vec[i]);
        string drive = "C";
        drive[0] += i;
        pButton->SetCheck(pt.get<bool>("disksetting." + drive) ? STATUS_CHECKED : 0);
    }
    //reset runonstartup
    CButton* pButton = (CButton*)GetDlgItem(IDC_START);
    pButton->SetCheck(pt.get<bool>("runonstartup") ? STATUS_CHECKED : 0);
    //reset frequency
    SetDlgItemInt(IDC_EDIT_FRE, pt.get<int>("frequency"));
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
        try{
            string jsonpath = GetModuleProfileName("init.json");
            write_json(jsonpath, pt);
        }
        catch (exception& e)
        {
            MessageBox(TEXT("init.json��ʧ����!"));
        }
       
        CDialogEx::OnCancel();
        CDialogEx::OnClose();
    }
    
    
}


void CwakeupdiskDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
   
    BOOST_FOREACH(auto& x, pt.get_child("disksetting"))
    {
        if (x.second.data()=="true")
        {
            string drive = x.first + ":\\wkd.log";
            std::ofstream fs(drive.c_str());
            fs << "0" << std::endl;
            fs.close();
        }
    }
    CDialogEx::OnTimer(nIDEvent);
}

