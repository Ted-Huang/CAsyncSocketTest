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
	int nSend = Send(pSendData, nSendLength);
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

void CSocketSessionForPLC::CheckDataBuf()
{
	BYTE *pStart = m_cReceiveBuf;
	MovePacketToStart(&pStart, m_nReceiveSize);
	//AOI_SDK_PACKET_HEADER *pHdr = (AOI_SDK_PACKET_HEADER*)m_cReceiveBuf;
	//int nCmdPacketSize = sizeof(AOI_SDK_PACKET_HEADER) + pHdr->nSize + 1; //Header+BodySize+CheckSum
	//while ((m_nReceiveSize >= sizeof(AOI_SDK_PACKET_HEADER)) && (m_nReceiveSize >= nCmdPacketSize)){
		//ParseCommand((BYTE*)pHdr, nCmdPacketSize);
		//pHdr = (AOI_SDK_PACKET_HEADER*)((BYTE*)pHdr + nCmdPacketSize);
		//m_nReceiveSize -= nCmdPacketSize;
		//nCmdPacketSize = sizeof(AOI_SDK_PACKET_HEADER) + pHdr->nSize + 1;
	//}
}

void CSocketSessionForPLC::MovePacketToStart(BYTE **ppCurrent, int &DataSize)
{
	if (ppCurrent && (*ppCurrent) && DataSize){
		BYTE *pPtr = *ppCurrent;
		long nNewSize = DataSize;
		//while ((*(DWORD*)(pPtr)) != SDK_PACKET_START){
		//	if (nNewSize <= 0){
		//		break;
		//	}
		//	pPtr++;
		//	nNewSize--;
		//}
		long nOffset = DataSize - nNewSize;
		if (nOffset >0){
			//Move Memory & Clear Memory
			memmove(*ppCurrent, pPtr, nNewSize);
			memset(*ppCurrent + nNewSize, 0, nOffset);
			DataSize = nNewSize;
		}
	}
}

void CSocketSessionForPLC::OnClose(int nErrorCode)
{
	if (m_pINotify){
		m_pINotify->OnError(this, ERR_SDK_SOCKET_CLOSE, NULL);
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void CSocketSessionForPLC::OnReceive(int nErrorCode)
{
	int nRead = 0;
	nRead = Receive(m_cReceiveBuf + m_nReceiveSize, sizeof(m_cReceiveBuf) - m_nReceiveSize);
	bool bSuccess = true;
	switch (nRead){
	case 0: //already disconnect
		Close();
		bSuccess = false;
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK){
			AfxMessageBox(_T("Error occurred"));
			Close();
			bSuccess = false;
		}
		break;
	default:
		m_nReceiveSize += nRead;
		break;
	}
	if (bSuccess){
		TCHAR* pStr = (TCHAR*)m_cReceiveBuf;
		TRACE(L"received: %s \n", pStr);
		//CheckDataBuf();
	}
	CAsyncSocket::OnReceive(nErrorCode);
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