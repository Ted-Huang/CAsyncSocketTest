// ChatServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "ChatMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg dialog

CChatServerDlg::CChatServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatServerDlg)
	m_lPort = 6000;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatServerDlg)
	DDX_Control(pDX, IDC_EDIT_MESSAGES, m_editMessages);
	DDX_Control(pDX, IDC_LIST_USERS, m_listBoxUsers);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON_PING_USER, m_buttonPingUser);
	DDX_Control(pDX, IDC_BUTTON_PING_ALL_USERS, m_buttonPingAllUsers);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT_USER, m_buttonDisconnectUser);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT_ALL_USERS, m_buttonDisconnectAllUsers);
	DDX_Text(pDX, IDC_EDIT_PORT, m_lPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	//{{AFX_MSG_MAP(CChatServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_USER, OnButtonDisconnectUser)
	ON_BN_CLICKED(IDC_BUTTON_PING_USER, OnButtonPingUser)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_PING_ALL_USERS, OnButtonPingAllUsers)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_ALL_USERS, OnButtonDisconnectAllUsers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg message handlers

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	UpdateUI();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

////////////////////////////////////////////////////////////////////////////////
// NDK: overrides OnIsConnectionAccepted, OnConnect, OnMessage, OnDisconnect
//      and OnPing

// Called when a user tries to connect to the server. Return TRUE to accept
// the connection or FALSE otherwise. The derived class must override this
// method.
BOOL CChatServerDlg::OnIsConnectionAccepted()
{
	// Accept only 20 users as an example, you can change this number or always
	// return TRUE;
	ASSERT(GetNbUsers() < 20);

	return (GetNbUsers() < 20);
}

// Called when a user is connected to the server. The derived class must 
// override this method.
void CChatServerDlg::OnConnect(long /*lUserId*/)
{
	// OnMessage: ChatUserJoin adds the user in the map, so nothing to do here
}

// Called whenever a message is received from a user. The derived class must 
// override this method.
void CChatServerDlg::OnMessage(long lUserId, CNDKMessage& message)
{
	switch (message.GetId())
	{
	case ChatUserJoin:
		{
			CString strNickname;
			message.GetAt(0, strNickname);

			AddUser(lUserId, strNickname);

			CString strUserJoin;
			strUserJoin.Format(IDS_USER_JOIN, strNickname);
						
			AddSystemText(strUserJoin);

			SendMessageToAllUsersExceptFor(lUserId, message);
		}
		break;

	case ChatText:
		{
			CString strNickname;
			m_mapIdsNicknames.Lookup(lUserId, strNickname);

			CString strText;
			message.GetAt(0, strText);

			AddText(strNickname + _T(": ") + strText);

			message.SetAt(0, strNickname);
			message.SetAt(1, strText);

			// Send the text to all other users
			SendMessageToAllUsersExceptFor(lUserId, message);
		}
		break;

	case ChatBigMessage:
		{		
			char szBigMessage[300];
			message.GetAt(0, szBigMessage, sizeof(szBigMessage));

			UCHAR uc;
			message.GetAt(1, uc);

			char c;
			message.GetAt(2, c);

			USHORT us;
			message.GetAt(3, us);

			short s;
			message.GetAt(4, s);

			UINT un;
			message.GetAt(5, un);

			int n;
			message.GetAt(6, n);

			ULONG ul;
			message.GetAt(7, ul);

			long l;
			message.GetAt(8, l);

			float f;
			message.GetAt(9, f);

			double d;
			message.GetAt(10, d);

			double dValues[10000];
			message.GetAt(11, dValues, sizeof(dValues));

			ASSERT(dValues[5555] == 1234.56789f);

			CString strEndMessage;
			message.GetAt(12, strEndMessage);

			CString strNickname;
			m_mapIdsNicknames.Lookup(lUserId, strNickname);

			CString strBigMessage;
			strBigMessage.Format(IDS_BIG_MESSAGE, strNickname);
			
			AddSystemText(strBigMessage);
		}
		break;
	}
}

// Called whenever a user is disconnected (the the user might have closed 
// the connection or an error occurs when sending a message, for example). 
// OnDisconnect callback isn't called when DisconnectUser or 
// DisconnectAllUsers is used. DisconnectUser don't
// need to be called when OnDisconnect callback is called. The derived class
// must override this method.
void CChatServerDlg::OnDisconnect(long lUserId, NDKServerDisconnection disconnectionType)
{
	CString strNickname;
	m_mapIdsNicknames.Lookup(lUserId, strNickname);

	CNDKMessage message(ChatUserQuit);
	message.Add(strNickname);

	// Inform all users that a user is disconnected
	SendMessageToAllUsers(message);

	UINT unResId = 0;

	switch (disconnectionType)
	{
	case NDKServer_NormalDisconnection:
		unResId = IDS_SERVER_CLOSE_USER;	
		break;

	case NDKServer_ClientCloseConnection:	
		unResId = IDS_USER_QUIT;
		break;

    case NDKServer_ErrorSendingMessage:
		unResId = IDS_ERROR_SENDING_MESSAGE;	
		break;

	case NDKServer_ErrorReceivingMessage:
		unResId = IDS_ERROR_SENDING_MESSAGE;	
		break;

	default:
		break;
	}

	CString strUserQuit;
	strUserQuit.Format(unResId, strNickname);

	AddSystemText(strUserQuit);
	
	DeleteUser(lUserId);
}

// Called when the ping from the user is received. The number of
// milliseconds is returned since PingUser was called.
void CChatServerDlg::OnPing(long lUserId, long lNbMilliseconds)
{
	CString strNickname;
	m_mapIdsNicknames.Lookup(lUserId, strNickname);

	CString strPingReceived;
	strPingReceived.Format(IDS_PING_RECEIVED, strNickname, lNbMilliseconds);

	AddSystemText(strPingReceived);
}

void CChatServerDlg::OnButtonStart() 
{
	if (UpdateData(TRUE))
	{
		if (IsStarted())
		{
			Stop();
			
			UpdateUI();

			AddSystemText((LPCSTR)IDS_STOPPED);
		}
		else
		{
			if (StartListening(m_lPort))
			{
				UpdateUI();

				AddSystemText((LPCSTR)IDS_STARTED);
			}
			else
			{
				AfxMessageBox(IDS_CANNOT_START, MB_ICONSTOP);
			}
		}
	}
}

void CChatServerDlg::OnButtonPingUser() 
{
	long lUserId = GetUserFromCurrentSelection();

	if (lUserId != -1)
		PingUser(lUserId);
}

void CChatServerDlg::OnButtonPingAllUsers() 
{
	PingAllUsers();

	AddSystemText((LPCSTR)IDS_PING_ALL_USERS);
}

void CChatServerDlg::OnButtonDisconnectUser() 
{
	long lUserId = GetUserFromCurrentSelection();
	
	if (lUserId != -1)
		DisconnectUser(lUserId);
}

void CChatServerDlg::OnButtonDisconnectAllUsers() 
{
	DisconnectAllUsers();

	m_mapIdsNicknames.RemoveAll();

	m_listBoxUsers.ResetContent();
}

void CChatServerDlg::UpdateUI()
{
	BOOL bEnable = IsStarted();

	if (bEnable)
		m_buttonStart.SetWindowText(_T("&Stop"));
	else
		m_buttonStart.SetWindowText(_T("&Start"));

	m_buttonPingUser.EnableWindow(bEnable);
	m_buttonPingAllUsers.EnableWindow(bEnable);
	m_buttonDisconnectUser.EnableWindow(bEnable);
	m_buttonDisconnectAllUsers.EnableWindow(bEnable);
}

void CChatServerDlg::AddText(const CString& strText)
{
	m_editMessages.SetSel(m_editMessages.GetWindowTextLength(), m_editMessages.GetWindowTextLength());
	m_editMessages.ReplaceSel(strText + "\r\n");
}

void CChatServerDlg::AddSystemText(const CString& strText)
{
	AddText(_T("* ") + strText + _T(" *"));
}

void CChatServerDlg::AddUser(long lUserId, const CString& strNickname)
{
	m_mapIdsNicknames.SetAt(lUserId, strNickname);

	int nPos = m_listBoxUsers.AddString(strNickname);

	m_listBoxUsers.SetCurSel(nPos);
}

void CChatServerDlg::DeleteUser(long lUserId)
{
	CString strNickname;

	m_mapIdsNicknames.Lookup(lUserId, strNickname);

	m_listBoxUsers.DeleteString(m_listBoxUsers.FindStringExact(0, strNickname));

	m_mapIdsNicknames.RemoveKey(lUserId);

	if (m_listBoxUsers.GetCount() > 0)
		m_listBoxUsers.SetCurSel(0);
}

long CChatServerDlg::GetUserFromCurrentSelection() const
{
	long lResultUserId = -1;

	int nCurSel = m_listBoxUsers.GetCurSel();

	if (nCurSel != LB_ERR)
	{
		CString strNickname;
		
		m_listBoxUsers.GetText(nCurSel, strNickname);

		POSITION pos = m_mapIdsNicknames.GetStartPosition();

		while (pos != NULL)
		{
			long lUserId;
			CString strNicknameMap;

			m_mapIdsNicknames.GetNextAssoc(pos, lUserId, strNicknameMap);

			if (strNickname == strNicknameMap)
			{
				lResultUserId = lUserId;
				pos = NULL;
			}
		}
	}
	
	return lResultUserId;
}