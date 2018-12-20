// ChatClientDlg.h : header file
//

#if !defined(AFX_CHATCLIENTDLG_H__2BEFF042_8F5F_41E9_B02A_06FE15E3BF09__INCLUDED_)
#define AFX_CHATCLIENTDLG_H__2BEFF042_8F5F_41E9_B02A_06FE15E3BF09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDKClient.h"

/////////////////////////////////////////////////////////////////////////////
// CChatClientDlg dialog

// NDK: public CNDKClient added
class CChatClientDlg : public CDialog, public CNDKClient
{
// Construction
public:
	CChatClientDlg(CWnd* pParent = NULL);	// standard constructor
	
// Dialog Data
	//{{AFX_DATA(CChatClientDlg)
	enum { IDD = IDD_CHATCLIENT_DIALOG };
	CEdit	m_editChatOutput;
	CEdit	m_editChatInput;
	CButton	m_buttonSendBigMessage;
	CButton	m_buttonSend;
	CButton	m_buttonPingServer;
	CButton	m_buttonConnect;
	CString	m_strIp;
	long	m_lPort;
	CString	m_strChatInput;
	CString	m_strNickname;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

// NDK: overrides OnMessage, OnDisconnect and OnPing

	// Called when a message is received. The derived class must override this
	// method.
	virtual void OnMessage(CNDKMessage& message);

	// Called whenever an unexpected disconnection occurs. The only case when
	// this method isn't call is when CloseConnection is used. CloseConnection
	// don't need to be called when when OnDisconnect is called. The derived 
	// class must override this method.
	virtual void OnDisconnect(NDKClientDisconnection disconnectionType);

	// Called when the ping from the server is received. The number of
	// milliseconds is returned since PingServer was called.
	virtual void OnPing(long lNbMilliseconds);

	// Generated message map functions
	//{{AFX_MSG(CChatClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonPingServer();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonSendBigMessage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateUI();
	void AddText(const CString& strText);
	void AddSystemText(const CString& strText);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATCLIENTDLG_H__2BEFF042_8F5F_41E9_B02A_06FE15E3BF09__INCLUDED_)
