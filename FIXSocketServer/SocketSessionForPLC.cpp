#include "stdafx.h"
#include "SocketSessionForPLC.h"

const char ctBeginString[] = "8=AOI|";
const int ctHeaderSize = 13;
const int ctTrailerSize = 7;
const CString ctFieldSplit = L"|";
const CString ctDataSplit = L"=";

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

void CSocketSessionForPLC::ParseCommand(CMap<int, int, CString, CString>* pMap)
{
	CString strMsgType;
	if (!pMap || !pMap->Lookup(MsgType, strMsgType) || !m_pINotify)
		return;

	if (strMsgType == "C"){
		CString strIndexType, strIndex;
		if (pMap->Lookup(IndexType, strIndexType) && pMap->Lookup(Index, strIndex)){
			m_pINotify->OnPLCChangeIndex(_ttoi(strIndexType), _ttoi(strIndex));
		}
	}
}

void CSocketSessionForPLC::Split(CString str, CString strSplitter, vector<CString>* vResult)
{
	int nTokenPos = 0; 
	CString strToken = str.Tokenize(strSplitter, nTokenPos);
	while (!strToken.IsEmpty()){
		vResult->push_back(strToken);
		strToken = str.Tokenize(strSplitter, nTokenPos);
	}
}

void CSocketSessionForPLC::ParseMsgToMap(CString str, CMap<int, int, CString, CString>* pMap)
{
	int nFieldTokenPos = 0, nDataTokenPos = 0;
	vector<CString> vField;
	Split(str, ctFieldSplit, &vField);
	
	for (int x = 0; x < vField.size(); x++){
		vector<CString> vKeyValue;
		Split(vField.at(x), ctDataSplit, &vKeyValue);

		if (vKeyValue.size() != 2)
			continue;

		CString strKey, strValue;
		pMap->SetAt(_ttoi(vKeyValue.at(0)), vKeyValue.at(1));
	}
}

BOOL CSocketSessionForPLC::HandleCheckSum(int nBodyLength)
{
	BOOL bRtn = FALSE;
	char cTrailer[ctTrailerSize];
	CMap<int, int, CString, CString> mTrailer;
	CString strCheckSum;
	//checksum
	memset(cTrailer, 0, ctTrailerSize);
	memcpy(cTrailer, m_cReceiveBuf + ctHeaderSize + nBodyLength, ctTrailerSize);
	ParseMsgToMap(CString(cTrailer, ctTrailerSize), &mTrailer);
	int nCheck = 0;
	if (mTrailer.GetSize() == 1 && mTrailer.Lookup(CheckSum, strCheckSum)){
		for (int x = 0; x < m_nReceiveSize - ctTrailerSize + 1; x++){
			nCheck += m_cReceiveBuf[x];
		}
		bRtn = (nCheck % 256) == _ttoi(strCheckSum);
	}
	return bRtn;
}


void CSocketSessionForPLC::HandleReceive()
{
	BYTE *pStart = m_cReceiveBuf;
	//check header
	if (!MovePacketToStart(&pStart, m_nReceiveSize))
		return;


	//body
	char cHeader[ctHeaderSize], cTrailer[ctTrailerSize];
	memset(cHeader, 0, ctHeaderSize);
	memcpy(cHeader, m_cReceiveBuf, ctHeaderSize);
	CMap<int, int, CString, CString> mHeader, mBody;
	ParseMsgToMap(CString(cHeader, ctHeaderSize), &mHeader);
	CString strBodyLength;
	int nBodyLength = 0; 
	if (mHeader.GetSize() == 2 && mHeader.Lookup(BodyLength, strBodyLength) && _ttoi(strBodyLength)> 0){
		nBodyLength = _ttoi(strBodyLength);
		
		//if (!HandleCheckSum(nBodyLength))
		//	return;

		char *pBody = new char[nBodyLength];
		memset(pBody, 0, nBodyLength);
		memcpy(pBody, m_cReceiveBuf + ctHeaderSize, nBodyLength);
		CString str = CString(pBody, nBodyLength);
		ParseMsgToMap(str, &mBody);

		ParseCommand(&mBody);
		m_nReceiveSize -= (ctHeaderSize + nBodyLength + ctTrailerSize);

		delete[] pBody;
	}
}

BOOL CSocketSessionForPLC::IsMatch(BYTE *ppCurrent, int nLeftSize)
{
	if (_countof(ctBeginString) > nLeftSize)
		return FALSE;
	BYTE *pSrc = ppCurrent;
	BYTE *pDst = (BYTE*)ctBeginString;
	for (int x = 0; x < _countof(ctBeginString) - 1 /*end of string*/; x++)
	{
		if (*pSrc != *pDst)
		{
			return FALSE;
		}
		pSrc++;
		pDst++;
	}

	return TRUE;
}

BOOL CSocketSessionForPLC::MovePacketToStart(BYTE **ppCurrent, int &DataSize)
{
	BOOL bRtn = FALSE;
	for (int x = 0; x < DataSize; x++){
		int nNewSize = DataSize - x;
		bRtn = IsMatch(*ppCurrent + x, nNewSize);
		if (bRtn){
			if (x > 0){
				//Move Memory & Clear Memory
				memmove(*ppCurrent, *ppCurrent + x, nNewSize);
				memset(*ppCurrent + nNewSize, 0, x);
				DataSize = nNewSize;
			}
			break;
		}
	}
	return bRtn;
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
		char *pBody = new char[nRead];
		memset(pBody, 0, nRead);
		memcpy(pBody, m_cReceiveBuf, nRead);
		TRACE(L"received: %s \n", CString(pBody, nRead));
		HandleReceive();
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