#pragma once
#include "afxsock.h"
#include <vector>
using namespace std;
#define MAX_RECEIVE_BUFFER_SIZE 64000
#define MAX_SEND_BUFFER_SIZE 16000

class ISessionNotify
{
public:
	enum {
		ERR_SDK_SOCKET_CLOSE = 0,
		ERR_SDK_SOCKET_CONNECT = 1,
		ERR_SDK_SOCKET_SEND = 2,
		ERR_SDK_SOCKET_NOECHO = 3,
	};
	virtual void OnError(void *pInstance, long ErrorId, long ErrorData) = 0;
	virtual void OnPLCChangeIndex(int nType, int nIndex) = 0;
	//virtual void OnSdkSetup(void *pInstance, long nSubType, long nData) = 0;
	//virtual void OnSdkMass(void *pInstance, long nSubType, LPBYTE pData, long nDataSize) = 0;
};

class CSocketSessionForPLC : public CAsyncSocket, public ISessionNotify{
public:
	CSocketSessionForPLC();
	~CSocketSessionForPLC();

	void AttachNotify(ISessionNotify *pLink) { m_pINotify = pLink; };
	BOOL SendData(int nSize, BYTE* pData);
private:
	void Init();
	void ParseCommand(CMap<int, int, CString, CString>* pMap);
	void Split(CString str, CString strSplitter, vector<CString>* vResult);
	void ParseMsgToMap(CString str, CMap<int, int, CString, CString>* pMap);
	BOOL HandleCheckSum(int nBodyLength);
	void HandleReceive();
	BOOL IsMatch(BYTE *ppCurrent, int nLeftSize);
	BOOL MovePacketToStart(BYTE **ppCurrent, int &DataSize);

protected:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	//ISessionNotify
	virtual void OnError(void *pInstance, long ErrorId, long ErrorData) {};
	virtual void OnPLCChangeIndex(int nType, int nIndex) {};
private:
	ISessionNotify* m_pINotify;
	int m_nReceiveSize;
	BYTE m_cReceiveBuf[MAX_RECEIVE_BUFFER_SIZE];
	int m_nSendSize;
	BYTE m_cSendBuf[MAX_SEND_BUFFER_SIZE];

	enum FIXMsg
	{
		BeginString = 8,
		BodyLength = 9,
		CheckSum = 10,
		MsgType = 35,
		MsgTime = 52,
		IndexType = 10001,
		Index = 10002,
	};
};