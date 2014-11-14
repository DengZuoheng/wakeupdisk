
// wakeupdiskDlg.h : 头文件
//

#pragma once
#include <vector>
#include <boost/property_tree/ptree.hpp>
using std::vector;
using namespace boost::property_tree;

// CwakeupdiskDlg 对话框
class CwakeupdiskDlg : public CDialogEx
{
    // 构造
public:
    CwakeupdiskDlg(CWnd* pParent = NULL);	// 标准构造函数

    // 对话框数据
    enum { IDD = IDD_WAKEUPDISK_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    void ToTray();
protected:
    //    afx_msg LRESULT OnShowtask(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShowtask(WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnBnClickedCancel();
    afx_msg void OnClose();
    afx_msg void OnBnClickedCheckc();
public:
    vector<int> m_vecSetting;
    ptree pt;
    int m_fre;
    int m_runonstartup;

private:
    void Reset();
    void ReadJSONFile();
    void VectoPtree();
    void Ptreetovec();
    void SetRunOnStartUp(bool);

public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
