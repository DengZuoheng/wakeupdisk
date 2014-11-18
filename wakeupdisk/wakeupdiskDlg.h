
// wakeupdiskDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>
#include <boost/property_tree/ptree.hpp>
using std::vector;
using namespace boost::property_tree;

// CwakeupdiskDlg �Ի���
class CwakeupdiskDlg : public CDialogEx
{
    // ����
public:
    CwakeupdiskDlg(CWnd* pParent = NULL);	// ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_WAKEUPDISK_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


    // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
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
public:   
    ptree pt;
 
private:
    void Reset();
    void ReadJSONFile();
    void SetRunOnStartUp(bool);

public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
//    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
