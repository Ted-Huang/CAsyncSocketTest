#include "stdafx.h"
#include "SocketServerForPLC.h"

CSorketServerForPLC::CSorketServerForPLC()
{
	Init();
}

CSorketServerForPLC::~CSorketServerForPLC()
{
	Finalize();
}

BOOL CSorketServerForPLC::Start()
{
	BOOL bFlag = FALSE;
	if (Create(m_nPort)){
		if (Listen()){
			bFlag = TRUE;
		}
	}
	return bFlag;
}

void CSorketServerForPLC::Test()
{
	for (auto pSession : m_vSession){
		if (pSession){
			CString str("abc");
			int ii = 10000;
			//int nn = pSession->Send(str, _tcslen(str) * sizeof(TCHAR));
			//int nn = pSession->Send(&ii, sizeof(int));
			pSession->SendData(0, 0);
			//TRACE(L"%d \n", nn);
		}
	}
}

void CSorketServerForPLC::Init()
{
	if (!AfxSocketInit()){
		TRACE("Init Socket Error! \n");
	}

	m_nPort = AOI_PLC_PORT;
}

void CSorketServerForPLC::Finalize()
{
	while (m_vSession.size()){
		if (m_vSession.back()){
			delete m_vSession.back();
		}
		m_vSession.pop_back();
	}
}

void CSorketServerForPLC::OnAccept(int nErrorCode)
{
	CAsyncSocket::OnAccept(nErrorCode);
	if (nErrorCode != NO_ERROR)
		return;

	CSocketSessionForPLC* pSession = new CSocketSessionForPLC;
	m_vSession.push_back(pSession);
	Accept(*pSession);

	pSession->AttachNotify(this);
}

void CSorketServerForPLC::OnError(void *pInstance, long ErrorId, long ErrorData)
{
	if (!pInstance)
		return;

	auto it = m_vSession.begin();
	while (it != m_vSession.end()){
		if (*it && *it == pInstance){
			delete *it;
			*it = NULL;
			m_vSession.erase(it);
			break;
		}
	}
}
