#include "stdafx.h"
#include "SocketSessionForPLC.h"

CSocketSessionForPLC::CSocketSessionForPLC()
{
	m_pINotify = NULL;
}

CSocketSessionForPLC::~CSocketSessionForPLC()
{

}

void CSocketSessionForPLC::Init()
{
	m_nReceiveSize = 0;
	m_nSendSize = 0;
	m_nSendBufferSize = 0;
	memset(m_cReceiveBuf, 0, sizeof(m_cReceiveBuf));
	memset(m_cSendBuf, 0, sizeof(m_cSendBuf));
}

void CSocketSessionForPLC::OnClose(int nErrorCode)
{
	TRACE("CSocketSessionForPLC OnClose! \n");
	if (m_pINotify){
		m_pINotify->OnError(this, ERR_SDK_SOCKET_CLOSE, NULL);
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void CSocketSessionForPLC::OnConnect(int nErrorCode)
{
	TRACE("OnConnect! \n ");
}

void CSocketSessionForPLC::OnReceive(int nErrorCode)
{
	TRACE("OnReceive! \n ");
}

void CSocketSessionForPLC::OnSend(int nErrorCode)
{
	while (m_nSendSize < m_nSendBufferSize)
	{
		int dwBytes;

		if ((dwBytes = Send(m_cSendBuf + m_nSendSize,
			m_nSendBufferSize - m_nSendSize)) == SOCKET_ERROR)
		{
			if (GetLastError() == WSAEWOULDBLOCK)
			{
				break;
			}
			else
			{
				TCHAR szError[256];
				_stprintf_s(szError, _T("Server Socket failed to send: %d"),
					GetLastError());
				Close();
				AfxMessageBox(szError);
			}
		}
		else
		{
			m_nSendSize += dwBytes;
		}
	}

	if (m_nSendSize == m_nSendBufferSize)
	{
		m_nSendSize = m_nSendBufferSize = 0;
		memset(m_cSendBuf, 0, sizeof(m_cSendBuf));
	}

	CAsyncSocket::OnSend(nErrorCode);
	TRACE("OnSend! \n ");
}