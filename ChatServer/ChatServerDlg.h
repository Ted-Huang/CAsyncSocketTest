// ChatServerDlg.h : header file
//

#if !defined(AFX_CHATSERVERDLG_H__9B9B5C0C_1F5D_435B_8E0B_AB02C62DCF67__INCLUDED_)
#define AFX_CHATSERVERDLG_H__9B9B5C0C_1F5D_435B_8E0B_AB02C62DCF67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDKServer.h"

typedef CMap<long, long, CString, CString> CMapLongToString;

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg dialog

class CChatServerDlg : public CDialog, public CNDKServer
{
// Construction
public:
	CChatServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChatServerDlg)
	enum { IDD = IDD_CHATSERVER_DIALOG };
	CEdit	m_editMessages;
	CListBox	m_listBoxUsers;
	CButton	m_buttonStart;
	CButton	m_buttonPingUser;
	CButton	m_buttonPingAllUsers;
	CButton	m_buttonDisconnectUser;
	CButton	m_buttonDisconnectAllUsers;
	long	m_lPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Called when a user tries to connect to the server. Return TRUE to accept
	// the connection or FALSE otherwise. The derived class must override this
	// method.
	virtual BOOL OnIsConnectionAccepted();

	// Called when a user is connected to the server. The derived class must 
	// override this method.
	virtual void OnConnect(long lUserId);

	// Called whenever a message is received from a user. The derived class must 
	// override this method.
	virtual void OnMessage(long lUserId, CNDKMessage& message);

	// Called whenever a user is disconnected (the the user might have closed 
	// the connection or an error occurs when sending a message, for example). 
	// OnDisconnect callback isn't called when DisconnectUser or 
	// DisconnectAllUsers is used. DisconnectUser don't
	// need to be called when OnDisconnect callback is called. The derived class
	// must override this method.
	virtual void OnDisconnect(long lUserId, NDKServerDisconnection disconnectionType);

	// Called when the ping from the user is received. The number of
	// milliseconds is returned since PingUser was called.
	virtual void OnPing(long lUserId, long lNbMilliseconds);

	// Generated message map functions
	//{{AFX_MSG(CChatServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonDisconnectUser();
	afx_msg void OnButtonPingUser();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonPingAllUsers();
	afx_msg void OnButtonDisconnectAllUsers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateUI();
	void AddText(const CString& strText);
	void AddSystemText(const CString& strText);
	void AddUser(long lUserId, const CString& strNickname);
	void DeleteUser(long lUserId);
	long GetUserFromCurrentSelection() const;

private:
	CMapLongToString m_mapIdsNicknames;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSERVERDLG_H__9B9B5C0C_1F5D_435B_8E0B_AB02C62DCF67__INCLUDED_)
