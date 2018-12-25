#pragma once

#include "afxsock.h"
#include "SocketSessionForPLC.h"
#include <vector>

using namespace std;
#define AOI_PLC_PORT	35001


class CSorketServerForPLC : public CAsyncSocket, public ISessionNotify{
public:
	CSorketServerForPLC(CWnd* pParent);
	virtual ~CSorketServerForPLC();

	BOOL Start();
private:
	void Init();
	void Finalize();
protected:
	virtual void OnAccept(int nErrorCode);

	//ISessionNotify
	virtual void OnError(void *pInstance, long ErrorId, long ErrorData);
private:
	int m_nPort;
	vector<CSocketSessionForPLC*> m_vSession;
	CWnd* m_pParent;
};