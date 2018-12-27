
// TestDlg.cpp : ��@��
//

#include "stdafx.h"
#include "AOISocketServer.h"
#include "AOISocketServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CTestDlg ��ܤ��



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


// CTestDlg �T���B�z�`��

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �N [����...] �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
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

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO:  �b���[�J�B�~����l�]�w
	//m_pBtn = new CButton;
	//m_pBtn->Create(L"���~", WS_CHILD | WS_VISIBLE, CRect(0, 0, 40, 40), this, BTN1);

	//m_pBtn2 = new CButton;
	//m_pBtn2->Create(L"�o�~", WS_CHILD | WS_VISIBLE, CRect(40, 0, 80, 40), this, BTN2);

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
	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
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

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
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
