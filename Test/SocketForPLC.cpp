#include "stdafx.h"
#include "SocketForPLC.h"

CSorketServerForPLC::CSorketServerForPLC()
{
	Init();
}

CSorketServerForPLC::~CSorketServerForPLC()
{

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