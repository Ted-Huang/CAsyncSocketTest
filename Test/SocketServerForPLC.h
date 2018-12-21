#pragma once

#include "afxsock.h"
#include "SocketSessionForPLC.h"
#include <vector>

using namespace std;
#define AOI_PLC_PORT	35001


class CSorketServerForPLC : public CAsyncSocket, public ISessionNotify{
public:
	CSorketServerForPLC();
	virtual ~CSorketServerForPLC();

	BOOL Start();
private:
	void Init();
	void Finalize();
protected:
	virtual void OnAccept(int nErrorCode);
	// no use
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	// no use

	//ISessionNotify
	virtual void OnError(void *pInstance, long ErrorId, long ErrorData);
private:
	int m_nPort;
	vector<CSocketSessionForPLC*> m_vSession;

};