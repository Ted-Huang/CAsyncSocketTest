
// AOISocketServerDlg.h : ���Y��
//

#pragma once


#include "SocketServerForPLC.h"
// CTestDlg ��ܤ��
class CTestDlg : public CDialogEx
{
// �غc
public:
	CTestDlg(CWnd* pParent = NULL);	// �зǫغc�禡
	~CTestDlg();
// ��ܤ�����
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
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
