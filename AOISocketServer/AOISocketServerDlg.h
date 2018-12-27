
// AOISocketServerDlg.h : 標頭檔
//

#pragma once


#include "SocketServerForPLC.h"
// CTestDlg 對話方塊
class CTestDlg : public CDialogEx
{
// 建構
public:
	CTestDlg(CWnd* pParent = NULL);	// 標準建構函式
	~CTestDlg();
// 對話方塊資料
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void click();
	void click2();
	DECLARE_MESSAGE_MAP()
private:
	CButton* m_pBtn;
	CButton* m_pBtn2;
	CEdit* m_pEdit;
	CSorketServerForPLC* m_pSocketServer;
};
