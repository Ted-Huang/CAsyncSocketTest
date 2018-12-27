
// TestDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "AOISocketServer.h"
#include "AOISocketServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CTestDlg 對話方塊



CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSocketServer = NULL;
}

CTestDlg::~CTestDlg()
{
	if (m_pSocketServer){
		delete m_pSocketServer;
		m_pSocketServer = NULL;
	}
	if (m_pBtn){
		delete m_pBtn;
		m_pBtn = NULL;
	}
	if (m_pBtn2){
		delete m_pBtn2;
		m_pBtn2 = NULL;
	}
	if (m_pEdit){
		delete m_pEdit;
		m_pEdit = NULL;
	}
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
enum{
	BTN1=990,
	BTN2,
	EDIT1,
};
BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(BTN1, click)
	//ON_BN_CLICKED(BTN2, click2)
END_MESSAGE_MAP()


// CTestDlg 訊息處理常式

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
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

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO:  在此加入額外的初始設定
	//m_pBtn = new CButton;
	//m_pBtn->Create(L"產品", WS_CHILD | WS_VISIBLE, CRect(0, 0, 40, 40), this, BTN1);

	//m_pBtn2 = new CButton;
	//m_pBtn2->Create(L"廢品", WS_CHILD | WS_VISIBLE, CRect(40, 0, 80, 40), this, BTN2);

	//m_pEdit = new CEdit;
	//m_pEdit->Create(WS_CHILD | WS_VISIBLE, CRect(0, 40, 40, 80), this, EDIT1);

	CWnd* pWnd = this;

	HWND hWnd = ::FindWindow(NULL, _T("AOITek_MasterWork_Slave"));
	if (hWnd){
		pWnd = CWnd::FromHandle(hWnd);
	}

	m_pSocketServer = new CSorketServerForPLC(pWnd); //send aoi hwnd
	if (!m_pSocketServer->Start()){
		AfxMessageBox(L"socket start error!!");
	}
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//enum SYNC_TYPE{
//	SYNC_PRODUCT_INDEX = 0,
//	SYNC_DEFECT_INDEX,
//};

//
//#define WM_TEST					(WM_APP + 3022)
//void CTestDlg::click()
//{
//	HWND hWnd = ::FindWindow(NULL, _T("AOI Master"));
//	CString str;
//	m_pEdit->GetWindowText(str);
//	//LPARAM lParam = MAKELPARAM(PRODUCT, _ttoi(str);
//	//int cidx = LOWORD(lParam);
//	//int iData = HIWORD(lParam);
//	if (hWnd){
//		::SendMessage(hWnd, WM_TEST, 233, MAKELPARAM(SYNC_PRODUCT_INDEX, _ttoi(str)));
//	}
//}
//
//void CTestDlg::click2()
//{
//	
//	HWND hWnd = ::FindWindow(NULL, _T("AOI Master"));
//	CString str;
//	m_pEdit->GetWindowText(str);
//	if (hWnd){
//		::SendMessage(hWnd, WM_TEST, 233, MAKELPARAM(SYNC_DEFECT_INDEX, _ttoi(str)));
//	}
//}
