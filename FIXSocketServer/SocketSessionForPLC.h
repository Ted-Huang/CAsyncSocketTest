#pragma once
#include "afxsock.h"
#include <vector>
using namespace std;
#define MAX_RECEIVE_BUFFER_SIZE 64000
#define MAX_SEND_BUFFER_SIZE 16000

const DWORD AOI_PACKET_START = 0x494f41;	//"AOI"
const DWORD AOI_PACKET_VER = 0x0100;		//ver. 1.0

enum AOI_MSG{
	MSG_HEARTBEAT = 0,
	MSG_DEFECTINDEX,
};

enum ECHO_TYPE{
	TYPE_QUERY = 0,
	TYPE_ECHO = 1
};

#pragma pack(push) /* push current alignment to stack */
#pragma pack(1) /* set alignment to 1 byte boundary */
struct AOI_MSG_HEADER{
	DWORD	dStart;										//4
	WORD	wVer;		// 0x0100 Ver 1.0				//2
	WORD	wReserved;									//2
	BYTE	cType;									    //1
	long	nMsgDate;	//yyyyMMdd						//4
	long	nMsgTime;	// HHmmssfff					//4
	long	nBodySize;	// Body Size					//4
};

struct AOI_MSG_HEARTBEAT{
	BYTE	cEcho;		// 0 ==>Ask,1==>Echo			//1
};

struct AOI_MSG_DEFECTINDEX{
	BYTE	cIndexType;	//
	long	nIndex;		//
};
#pragma pack(pop) /* restore original alignment from stack */

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
	virtual void OnDefectIndex(int nType, int nIndex) = 0;
};

class CSocketSessionForPLC : public CAsyncSocket, public ISessionNotify{
public:
	CSocketSessionForPLC();
	~CSocketSessionForPLC();

	void AttachNotify(ISessionNotify *pLink) { m_pINotify = pLink; };
private:
	void Init();
	bool ParseCommand(BYTE *pData, int DataSize);
	void CheckDataBuf();
	void MovePacketToStart(BYTE **ppCurrent, int &DataSize);
	bool PacketCheck(BYTE* pData, int DataSize);
	bool CheckSum(BYTE* pData, int DataSize);
	bool SendCommand(int nType, BYTE *pData, int DataSize, BOOL bWaitResponse);
	void MakeAoiSdkPacket(CByteArray &Packet, BYTE cCmdType, BYTE *pData, long DataSize, BOOL bWaitResponse);

protected:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	//ISessionNotify
	virtual void OnError(void *pInstance, long ErrorId, long ErrorData) {};
	virtual void OnDefectIndex(int nType, int nIndex) {};
private:
	ISessionNotify* m_pINotify;
	int m_nReceiveSize;
	BYTE m_cReceiveBuf[MAX_RECEIVE_BUFFER_SIZE];
	int m_nSendSize;
	BYTE m_cSendBuf[MAX_SEND_BUFFER_SIZE];
};