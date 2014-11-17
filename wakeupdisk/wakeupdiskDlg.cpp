
// wakeupdiskDlg.cpp : 实现文件
//
#pragma comment(lib,"Wtsapi32.lib")
#include "stdafx.h"
#include "wakeupdisk.h"
#include "wakeupdiskDlg.h"
#include "afxdialogex.h"
#include "Wtsapi32.h"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
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
#define ID_TIMER 1
#define STATUS_CHECKED 1


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CwakeupdiskDlg 对话框



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
END_MESSAGE_MAP()


// CwakeupdiskDlg 消息处理程序

BOOL CwakeupdiskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
    ReadJSONFile();
    Reset();
    SetTimer(ID_TIMER, pt.get<int>("frequency"), NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CwakeupdiskDlg::ReadJSONFile()
{
    try{
        read_json("init.json", pt);
    }
    catch (exception& e){
        MessageBox(TEXT("init.json丢失或损坏!"));      
        CDialogEx::OnCancel();
    }
}


void CwakeupdiskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CwakeupdiskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CwakeupdiskDlg::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码
    //CDialogEx::OnOK();
    //获取item
    //不能删除指针
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

    //SetRunOnStartUp(m_runonstartup);
    OnTimer(ID_TIMER);
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
    //找到系统的启动项   
    CString lpRun = CString(_T("C:\\Users\\"))
        +strName
        +CString(_T("\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup"));
    //打开启动项Key   
    long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
    if (lRet == ERROR_SUCCESS)
    {
        TCHAR pFileName[MAX_PATH] = { 0 };
        //得到程序自身的全路径   
        DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
        //添加一个子Key,并设置值 // 下面的"test"是应用程序名字（不加后缀.exe）  
        if (bFlag == true)
        {
            lRet = RegSetValueEx(hKey, _T("wakeupdisk"), 0, REG_SZ, (BYTE *)pFileName, dwRet);
        }
        else
        {
            lRet = RegDeleteValue(hKey, _T("wakeupdisk"));
        }
        
        //关闭注册表   
        RegCloseKey(hKey);
        if (lRet != ERROR_SUCCESS)
        {
            AfxMessageBox(_T("系统参数错误,不能完成开机启动设置"));
        }
        else
        {
            AfxMessageBox(_T("设置启动成功"));
        }
    }
    else
        AfxMessageBox(_T("系统参数错误,不能完成开机启动设置"));*/
}

void CwakeupdiskDlg::ToTray()
{
   
    NOTIFYICONDATA nid;
    nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
    nid.hWnd = this->m_hWnd;
    nid.uID = IDR_MAINFRAME;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称
    nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
    //strcpy(nid.szTip, TEXT("计划任务提醒"));//信息提示条为“计划任务提醒”
    wsprintf(nid.szTip, TEXT("%s"), TEXT("托盘测试"));
    Shell_NotifyIcon(NIM_ADD, &nid);//在托盘区添加图标
    ShowWindow(SW_HIDE);//隐藏主窗口
   
}

afx_msg LRESULT CwakeupdiskDlg::OnShowtask(WPARAM wParam, LPARAM lParam)
{
    if (lParam == WM_LBUTTONDOWN)
    {
        ShowWindow(SW_SHOW);//隐藏主窗口
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
    // TODO:  在此添加控件通知处理程序代码
    ToTray();
    Reset();
    //CDialogEx::OnCancel();
}

void CwakeupdiskDlg::OnClose()
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    if (::MessageBox(
        this->m_hWnd,
        TEXT("关闭程序可能导致硬盘休眠,确定关闭?"),
        TEXT("确定关闭"),
        MB_OKCANCEL) == 1)
    {
        try{
            write_json("init.json", pt);
        }
        catch (exception& e)
        {
            MessageBox(TEXT("????"));
        }
       
        CDialogEx::OnCancel();
        CDialogEx::OnClose();
    }
    
}


void CwakeupdiskDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    /*for (int i = 0; i < m_vecSetting.size(); ++i)
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
    }*/
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
