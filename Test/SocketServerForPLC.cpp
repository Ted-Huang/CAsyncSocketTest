#include "stdafx.h"
#include "SocketServerForPLC.h"

CSorketServerForPLC::CSorketServerForPLC(CWnd* pParent)
{
	Init();
	m_pParent = pParent;
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
