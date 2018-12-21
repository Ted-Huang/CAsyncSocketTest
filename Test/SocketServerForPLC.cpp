#include "stdafx.h"
#include "SocketServerForPLC.h"

CSorketServerForPLC::CSorketServerForPLC()
{
	Init();
}

CSorketServerForPLC::~CSorketServerForPLC()
{
	while (m_vSession.size()){
		if (m_vSession.back()){
			delete m_vSession.back();
		}
		m_vSession.pop_back();
	}
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
		TRACE("\nInit Socket Error!");
	}
	m_nPort = AOI_PLC_PORT;
}
void CSorketServerForPLC::OnAccept(int nErrorCode)
{
	CAsyncSocket::OnAccept(nErrorCode);
	if (nErrorCode != NO_ERROR)
		return;

	CSocketSessionForPLC* pSession = new CSocketSessionForPLC;
	m_vSession.push_back(pSession);
	Accept(*pSession);
	TRACE("\nAccept session!");
}

void CSorketServerForPLC::OnClose(int nErrorCode)
{

}

void CSorketServerForPLC::OnConnect(int nErrorCode)
{

}

void CSorketServerForPLC::OnReceive(int nErrorCode)
{

}

void CSorketServerForPLC::OnSend(int nErrorCode)
{

}