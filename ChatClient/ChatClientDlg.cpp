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
// CChatClientDlg dialog

CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatClientDlg)
	m_strIp = _T("127.0.0.1");
	m_lPort = 2323;
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
	DDX_Control(pDX, IDC_EDIT_TYPE, m_editType);
	
	DDX_Control(pDX, IDC_BUTTON_SEND, m_buttonSend);
	DDX_Control(pDX, IDC_BUTTON_PING_SERVER, m_buttonPingServer);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_lPort);
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
	default:
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

typedef struct _NDK_SYSTEM_INFO_ {
	DWORD	machineId;		// Zero based.
	DWORD	machineGroup;
	DWORD	machineJob;
	DWORD	flag;
	INT64	aoi_ver;
	INT64	aoi_rev;
	wchar_t	serverAddress[16];	// Beagle 20120503 added.
	wchar_t slaveAddress[17]; //eric chao 20140620
	DWORD	cameraNumber;	//	added by eric at 20121115
	DWORD	ewfInfo; //eric chao 20130204
	double  CamRadio; //eric chao 20160120
	int		nNDisplayUnit; //seanchen 20130828-4
	int		defectGroupTypeIndex;	// added by eric at 20150310
	BOOL bAreaCamera;
} NDK_SYSTEM_INFO;


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
				CNDKMessage message(0);
				NDK_SYSTEM_INFO m_systemInfo;
				memset(&m_systemInfo, 0, sizeof(NDK_SYSTEM_INFO));

				m_systemInfo.cameraNumber = 2;
				message.Add((USHORT)(m_systemInfo.machineId));	// client machine id
				message.Add((UINT)sizeof(NDK_SYSTEM_INFO));
				message.Add(&m_systemInfo, sizeof(NDK_SYSTEM_INFO));
				SendMessageToServer(message);
				//CNDKMessage message(234);
				//message.Add(123);	// client machine id
				//message.Add(456);
				//SendMessageToServer(message);
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
		if (IsConnected())
		{
			CloseConnection();
		}
		else
		{
			if (OpenConnection(m_strIp, m_lPort))
			{
				CString strType, strIndex;
				m_editType.GetWindowText(strType);
				m_editChatInput.GetWindowText(strIndex);
				CNDKMessage message(234);
				message.Add(_ttoi(strType));
				message.Add(_ttoi(strIndex));

				SendMessageToServer(message);
			}
			else
			{
				AfxMessageBox(IDS_CANNOT_CONNECT, MB_ICONSTOP);
			}
		}
	}
}


void CChatClientDlg::UpdateUI()
{
	BOOL bEnable = IsConnected();

	if (bEnable)
		m_buttonConnect.SetWindowText(_T("&Disconnect"));
	else
		m_buttonConnect.SetWindowText(_T("&Connect"));
		
	m_buttonPingServer.EnableWindow(bEnable);
	//m_buttonSend.EnableWindow(bEnable);
/*
	m_editChatOutput.EnableWindow(bEnable);
	m_editChatInput.EnableWindow(bEnable);*/
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
