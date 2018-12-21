#pragma once
#include "afxsock.h"
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
	//virtual void OnSdkAction(void *pInstance, long nSubType) = 0;
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

protected:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	//ISessionNotify
	virtual void OnError(void *pInstance, long ErrorId, long ErrorData) {};
private:
	ISessionNotify* m_pINotify;
	int m_nReceiveSize;
	BYTE m_cReceiveBuf[MAX_RECEIVE_BUFFER_SIZE];
	int m_nSendSize;
	BYTE m_cSendBuf[MAX_SEND_BUFFER_SIZE];
};