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
	memset(m_cReceiveBuf, 0, sizeof(m_cReceiveBuf));
	memset(m_cSendBuf, 0, sizeof(m_cSendBuf));
}

bool CSocketSessionForPLC::ParseCommand(BYTE *pData, int DataSize)
{
	bool bFlag = false;
	if (PacketCheck(pData, DataSize)){
		AOI_MSG_HEADER *pHdr = (AOI_MSG_HEADER*)pData;
		int nBodySize = pHdr->nBodySize;
		BYTE *pBody = (BYTE*)(pHdr + 1);
		switch (pHdr->cType){
		case MSG_HEARTBEAT:
			{
				AOI_MSG_HEARTBEAT *pHeartBeat = (AOI_MSG_HEARTBEAT*)pBody;
				if (pHeartBeat->cEcho == TYPE_QUERY){
					AOI_MSG_HEARTBEAT xHeartBeat;
					xHeartBeat.cEcho = TYPE_ECHO;
					SendCommand(MSG_HEARTBEAT, (BYTE*)&xHeartBeat, 1, FALSE);
				}
				else if (pHeartBeat->cEcho == TYPE_ECHO){
				}
			}
			break;
		case MSG_DEFECTINDEX:
			if (m_pINotify){
				AOI_MSG_DEFECTINDEX *pDefectIndex = (AOI_MSG_DEFECTINDEX*)pBody;
				m_pINotify->OnDefectIndex(pDefectIndex->cIndexType, pDefectIndex->nIndex);
			}
			break;
		default:
			TRACE("Unknown Command Type! \n");
			break;
		}
	}
	return bFlag;
}

void CSocketSessionForPLC::CheckDataBuf()
{
	BYTE *pStart = m_cReceiveBuf;
	MovePacketToStart(&pStart, m_nReceiveSize);
	AOI_MSG_HEADER *pHdr = (AOI_MSG_HEADER*)m_cReceiveBuf;
	int nCmdPacketSize = sizeof(AOI_MSG_HEADER) + pHdr->nBodySize + 1; //Header+BodySize+CheckSum
	while ((m_nReceiveSize >= sizeof(AOI_MSG_HEADER)) && (m_nReceiveSize >= nCmdPacketSize)){
		ParseCommand((BYTE*)pHdr, nCmdPacketSize);
		pHdr = (AOI_MSG_HEADER*)((BYTE*)pHdr + nCmdPacketSize);
		m_nReceiveSize -= nCmdPacketSize;
		nCmdPacketSize = sizeof(AOI_MSG_HEADER) + pHdr->nBodySize + 1;
	}
}

bool CSocketSessionForPLC::PacketCheck(BYTE* pData, int DataSize)
{
	AOI_MSG_HEADER *pHdr = (AOI_MSG_HEADER*)pData;
	if (pHdr->dStart == AOI_PACKET_START){
#if 0
		if (pHdr->wVer != AOI_PACKET_VER){ //Need Disconnect or Notify Upgrade Library!!
			return false;
		}
#endif //0
		if (pHdr->wReserved & 1){
			return true;
		}
		return CheckSum(pData, DataSize);
	}
	return false;
}

bool CSocketSessionForPLC::CheckSum(BYTE* pData, int DataSize)
{
	BYTE sum = 0;
	for (int i = 4; i<(DataSize - 1); i++)
		sum ^= pData[i];
	if (sum == pData[DataSize - 1])
		return true;
	return false;
}

bool CSocketSessionForPLC::SendCommand(int nType, BYTE *pData, int DataSize, BOOL bWaitResponse)
{
	bool bFlag = false;
	CByteArray cAoiPacket;
	MakeAoiSdkPacket(cAoiPacket, nType, pData, DataSize, bWaitResponse);
	BYTE *pSendData = cAoiPacket.GetData();
	int nSendLength = (int)cAoiPacket.GetSize();
	int nSend = Send(pSendData, nSendLength);

	if (nSend > 0){
		if (nSend == nSendLength){
			bFlag = true;
		}
		else{
			int nKeepSize = nSendLength - nSend;
			if ((nKeepSize + m_nSendSize) <MAX_SEND_BUFFER_SIZE){
				memcpy(m_cSendBuf + m_nSendSize, pSendData + nSend, nKeepSize);
				m_nSendSize += nKeepSize;
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

void CSocketSessionForPLC::MakeAoiSdkPacket(CByteArray &Packet, BYTE cCmdType, BYTE *pData, long DataSize, BOOL bWaitResponse)
{
	int nFullCmdSize = DataSize + sizeof(AOI_MSG_HEADER) + 1; //AOI_MSG_HEADER + CheckSum
	Packet.SetSize(nFullCmdSize);
	BYTE *pPtr = Packet.GetData();
	AOI_MSG_HEADER *pHeader = (AOI_MSG_HEADER*)pPtr;
	pHeader->dStart = AOI_PACKET_START;
	pHeader->wVer = AOI_PACKET_VER;
	pHeader->wReserved = 0;

	pHeader->cType = cCmdType; 
	pHeader->nBodySize = DataSize;
	memcpy(pHeader + 1, pData, DataSize); // Command Body
	BYTE cCheckSum = 0;
	for (int i = 4; i<(nFullCmdSize - 1); i++){
		cCheckSum ^= *(pPtr + i);
	}
	*(pPtr + nFullCmdSize - 1) = cCheckSum;
}

void CSocketSessionForPLC::MovePacketToStart(BYTE **ppCurrent, int &DataSize)
{
	if (ppCurrent && (*ppCurrent) && DataSize){
		BYTE *pPtr = *ppCurrent;
		long nNewSize = DataSize;
		while ((*(DWORD*)(pPtr)) != AOI_PACKET_START){
			if (nNewSize <= 0){
				break;
			}
			pPtr++;
			nNewSize--;
		}
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
		CheckDataBuf();
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