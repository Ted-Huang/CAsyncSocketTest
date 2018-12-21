#pragma once

#include "afxsock.h"

#define AOI_PLC_PORT	35001

class CSorketServerForPLC : public CAsyncSocket{
public:
	CSorketServerForPLC();
	virtual ~CSorketServerForPLC();

	BOOL Start();
private:
	void Init();

protected:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

private:
	int m_nPort;
};