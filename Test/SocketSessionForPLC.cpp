#include "stdafx.h"
#include "SocketSessionForPLC.h"

CSocketSessionForPLC::CSocketSessionForPLC()
{
	m_pINotify = NULL;
}

CSocketSessionForPLC::~CSocketSessionForPLC()
{

}

BOOL CSocketSessionForPLC::SendData(int nSize, BYTE* pData)
{
	BOOL bFlag = FALSE;
	CByteArray cAoiPacket;

	CString* str = new CString;
	for (int x = 0; x < 50; x++){ //test onsend
		if (x < 25)
			(*str) += L"a";
		else
			(*str) += L"b";
	}
	cAoiPacket.SetSize(str->GetLength() * 2);
	memcpy(cAoiPacket.GetData(), str->GetBuffer(), str->GetLength() * 2);
	BYTE *pSendData = cAoiPacket.GetData();
	int nSendLength = (int)cAoiPacket.GetSize();
	int nSend = Send(pSendData, 50);
	if (nSend > 0){
		if (nSend == nSendLength){
			bFlag = true;
		}
		else{
			int nKeepSize = nSendLength - nSend;
			if ((nKeepSize + m_nSendSize) < MAX_SEND_BUFFER_SIZE){
				memcpy(m_cSendBuf + m_nSendSize, pSendData + nSend, nKeepSize);
				m_nSendSize += nKeepSize;
				OnSend(0);
			}
		}
	}
	else{
		int nError = GetLastError();
		if (m_pINotify){
			m_pINotify->OnError(this, ERR_SDK_SOCKET_SEND, nError);
		}
	}
	return bFlag;
}
void CSocketSessionForPLC::Init()
{
	m_nReceiveSize = 0;
	m_nSendSize = 0;
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
	while (m_nSendSize >0){
		int dwBytes;
		if ((dwBytes = Send((LPCTSTR)m_cSendBuf, m_nSendSize)) == SOCKET_ERROR){
			if (GetLastError() == WSAEWOULDBLOCK) {
			}
			else{
				Close();
			}
			break;
		}
		else{
			int nDataSize = m_nSendSize - dwBytes;
			if (nDataSize){
				memmove(m_cSendBuf, m_cSendBuf + dwBytes, nDataSize);
				memset(m_cSendBuf + dwBytes, 0, nDataSize);
			}
			m_nSendSize -= dwBytes;
		}
	}
	CAsyncSocket::OnSend(nErrorCode);
}