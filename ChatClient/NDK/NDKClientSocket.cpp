////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// NDK 2.0 - Network Development Kit                                          //
//                                                                            //
// Authors: Sebastien Lachance                                                //
//                                                                            //
// E-mail:  netblitz@rocler.qc.ca                                             //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// Permission to use, copy, modify, and distribute this software for any      //
// purpose and without fee is hereby granted. This is no guarantee about the  //
// use of this software. For any comments, bugs or thanks, please email us.   //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// Targeted Platform: Any Windows version                                     //
//                                                                            //
// Compiled with:     Visual C++ 6.0                                          //
//                                                                            //
// Last modification: January 2002                                            //
//                                                                            //
// History:                                                                   //
//                                                                            //
// 1- First release of this file.                                             //
// 2- The class is renamed and some optimizations are applied. Hungarian      //
//    notation is used.                                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes                                                                   //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "NDKClientSocket.h"
#include "NDKClient.h"


////////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor                                                   //
////////////////////////////////////////////////////////////////////////////////

// Constructor.
CNDKClientSocket::CNDKClientSocket(CNDKClient* pClient)
{
	ASSERT(pClient != NULL);

	m_pClient = pClient;
}


// Destructor.
CNDKClientSocket::~CNDKClientSocket()
{
}


////////////////////////////////////////////////////////////////////////////////
// Overrides from CSocket                                                     //
////////////////////////////////////////////////////////////////////////////////

// Called when data is received.
void CNDKClientSocket::OnReceive(int nErrorCode) 
{
	// With Visual C++ 2003, this patch is needed because for an unknown reason socket notifications are not send
	//VERIFY(AsyncSelect(/*FD_READ |*/ FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE));
	VERIFY(AsyncSelect(FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE));	// modified by danny, important for speed up

	CSocket::OnReceive(nErrorCode);

	ASSERT(m_pClient != NULL);

	if (m_pClient != NULL)
	{
		if (m_pClient->ProcessPendingRead(nErrorCode))
		{
			CString strSocketAddress;
			UINT    unPort = 0;

			// Restore to the default notification
			if (GetSockName(strSocketAddress, unPort) != 0)
				VERIFY(AsyncSelect());
		}
	}
}
