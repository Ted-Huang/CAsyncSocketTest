// ChatClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "ChatMessage.h"
#include "NDKMessage.h"

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
// CChatClientDlg dialog

CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatClientDlg)
	m_strIp = _T("127.0.0.1");
	m_lPort = 6000;
	m_strChatInput = _T("");
	m_strNickname = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	srand(time(NULL));

	m_strNickname.Format(IDS_USER_X, rand() % 100);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatClientDlg)
	DDX_Control(pDX, IDC_EDIT_CHAT_OUTPUT, m_editChatOutput);
	DDX_Control(pDX, IDC_EDIT_CHAT_INPUT, m_editChatInput);
	DDX_Control(pDX, IDC_BUTTON_SEND_BIG_MESSAGE, m_buttonSendBigMessage);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_buttonSend);
	DDX_Control(pDX, IDC_BUTTON_PING_SERVER, m_buttonPingServer);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_lPort);
	DDX_Text(pDX, IDC_EDIT_CHAT_INPUT, m_strChatInput);
	DDX_Text(pDX, IDC_EDIT_NICKNAME, m_strNickname);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialog)
	//{{AFX_MSG_MAP(CChatClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_PING_SERVER, OnButtonPingServer)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_SEND_BIG_MESSAGE, OnButtonSendBigMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatClientDlg message handlers

BOOL CChatClientDlg::OnInitDialog()
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

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatClientDlg::OnPaint() 
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
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

////////////////////////////////////////////////////////////////////////////////
// NDK: overrides OnMessage, OnDisconnect and OnPing

// Called when a message is received. The derived class must override this
// method.
void CChatClientDlg::OnMessage(CNDKMessage& message)
{
	switch (message.GetId())
	{
	case ChatUserJoin:
		{
			CString strNickname;

			message.GetAt(0, strNickname);

			CString strUserJoin;
			strUserJoin.Format(IDS_USER_JOIN, strNickname);
			
			AddSystemText(strUserJoin);
		}
		break;

	case ChatText:
		{
			CString strNickname;
			CString strText;

			message.GetAt(0, strNickname);
			message.GetAt(1, strText);

			AddText(strNickname + _T(": ") + strText);
		}
		break;

	case ChatUserQuit:
		{
			CString strNickname;

			message.GetAt(0, strNickname);

			CString strUserQuit;
			strUserQuit.Format(IDS_USER_QUIT, strNickname);
			
			AddSystemText(strUserQuit);
		}
		break;
	}
}

// Called whenever an unexpected disconnection occurs. The only case when
// this method isn't call is when CloseConnection is used. CloseConnection
// don't need to be called when when OnDisconnect is called. The derived 
// class must override this method.
void CChatClientDlg::OnDisconnect(NDKClientDisconnection disconnectionType)
{
	UINT unResId = 0;

	switch (disconnectionType)
	{
	case NDKClient_NormalDisconnection:
		unResId = IDS_DISCONNECTED;
		break;

	case NDKClient_ServerCloseConnection:	
		unResId = IDS_SERVER_CLOSE_CONNECTION;	
		break;

    case NDKClient_ServerStop:
		unResId = IDS_SERVER_STOPPED;	
		break;

	case NDKClient_ErrorSendingMessage:
		unResId = IDS_ERROR_SENDING_MESSAGE;	
		break;

	case NDKClient_ErrorReceivingMessage:
		unResId = IDS_ERROR_RECEIVING_MESSAGE;	
		break;

	default:
		break;
	}

	AddSystemText((LPCSTR)unResId);

	UpdateUI();
}

// Called when the ping from the server is received. The number of
// milliseconds is returned since PingServer was called.
void CChatClientDlg::OnPing(long lNbMilliseconds)
{
	CString strPing;
	strPing.Format(IDS_PING_RECEIVED, lNbMilliseconds);

	AddSystemText(strPing);
}

void CChatClientDlg::OnButtonConnect() 
{
	if (UpdateData(TRUE))
	{
		if (IsConnected())
		{
			CloseConnection();
		}
		else
		{
			if (OpenConnection(m_strIp, m_lPort))
			{
				AddSystemText((LPCSTR)IDS_CONNECTED);

				CNDKMessage message(ChatUserJoin);
				message.Add(m_strNickname);

				SendMessageToServer(message);
			}
			else
			{
				AfxMessageBox(IDS_CANNOT_CONNECT, MB_ICONSTOP);
			}
		}

		UpdateUI();
	}
}

void CChatClientDlg::OnButtonPingServer() 
{
	PingServer();	
	
	AddSystemText((LPCSTR)IDS_PING_SERVER);
}

void CChatClientDlg::OnButtonSend() 
{
	if (UpdateData(TRUE))
	{
		CNDKMessage message(ChatText);
		message.Add(m_strChatInput);

		SendMessageToServer(message);

		AddText(m_strChatInput);

		m_strChatInput.Empty();
		UpdateData(FALSE);
	}
}

void CChatClientDlg::OnButtonSendBigMessage() 
{
	CNDKMessage message(ChatBigMessage);
		
	char szBigMessage[] = ("The big message contains all C++ basic types.");
	message.Add(szBigMessage, sizeof(szBigMessage));

	message.Add((UCHAR)_T('a'));
	message.Add((char)_T('b'));
	message.Add((USHORT) 1);
	message.Add((short)-2);
	message.Add((UINT)3);
	message.Add((int)-4);
	message.Add((ULONG)5);
	message.Add((long)-6);
	message.Add((float)123.456f);
	message.Add((double)-789.654f);

	double dValues[10000];
	dValues[5555] = 1234.56789f;
	message.Add(dValues, sizeof(dValues));

	message.Add(CString(_T("End of the big message")));

	SendMessageToServer(message);
}

void CChatClientDlg::UpdateUI()
{
	BOOL bEnable = IsConnected();

	if (bEnable)
		m_buttonConnect.SetWindowText(_T("&Disconnect"));
	else
		m_buttonConnect.SetWindowText(_T("&Connect"));
		
	m_buttonSendBigMessage.EnableWindow(bEnable);
	m_buttonPingServer.EnableWindow(bEnable);
	m_buttonSend.EnableWindow(bEnable);

	m_editChatOutput.EnableWindow(bEnable);
	m_editChatInput.EnableWindow(bEnable);
}

void CChatClientDlg::AddText(const CString& strText)
{
	m_editChatOutput.SetSel(m_editChatOutput.GetWindowTextLength(), m_editChatOutput.GetWindowTextLength());
	m_editChatOutput.ReplaceSel(strText + "\r\n");
}

void CChatClientDlg::AddSystemText(const CString& strText)
{
	AddText(_T("* ") + strText + _T(" *"));
}
