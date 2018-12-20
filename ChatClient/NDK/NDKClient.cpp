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
#include "NDKClient.h"
#include "NDKDefines.h"
#include "NDKClientSocket.h"
#include "NDKMessage.h"


static const long m_sendLength = 1024000;
static const long m_receiveLength = 1024000;

////////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor                                                   //
////////////////////////////////////////////////////////////////////////////////

// Constructor.
CNDKClient::CNDKClient()
{
	m_pClientSocket = NULL;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}


// Destructor.
CNDKClient::~CNDKClient()
{
	CloseConnection();
	CleanUp();
}


////////////////////////////////////////////////////////////////////////////////
// Attributes                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Returns if the client is connected to the server.
BOOL CNDKClient::IsConnected() const
{
	return m_pClientSocket != NULL;
}


// Gets the IP address and port number of the client.
BOOL CNDKClient::GetIpAndPort(CString& strIp, long& lPort) const
{
	BOOL bResult = FALSE;

	if (m_pClientSocket != NULL)
	{
		UINT unPort = 0;

		bResult = m_pClientSocket->GetSockName(strIp, unPort);

		lPort = unPort;
	}

	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Opens a connection to the server, given its IP address and the port 
// number. The IP can be a machine name as "ftp.ndk.com", or a dotted number 
// as "123.456.78.9".
BOOL CNDKClient::OpenConnection(const CString& strServerIp, long lPort)
{
	BOOL bResult = FALSE;

	CloseConnection();

	if (AfxSocketInit())
	{
		m_pClientSocket = new CNDKClientSocket(this);

		// Create the socket and try to connect to the server.
		if ((m_pClientSocket != NULL) && m_pClientSocket->Create() && 
			m_pClientSocket->Connect(strServerIp, lPort))
		{
			// added by danny at 20101108
			//int bNodelay = 1;
			//m_pClientSocket->SetSockOpt(TCP_NODELAY, &bNodelay, sizeof(int), IPPROTO_TCP);

			// --------------------------------------------------------------------------------- added by danny
			long sendBufferLength=0;
			long receiveBufferLength=0;
			int   optionLen=4;

			BOOL goodSock = m_pClientSocket->GetSockOpt(SO_RCVBUF,&receiveBufferLength,&optionLen,SOL_SOCKET);
			if(goodSock)
			{
				if(receiveBufferLength < m_receiveLength)
				{
					goodSock = m_pClientSocket->SetSockOpt(SO_RCVBUF,&m_receiveLength,optionLen,SOL_SOCKET);

				}
			}

			goodSock = m_pClientSocket->GetSockOpt(SO_SNDBUF,&sendBufferLength,&optionLen,SOL_SOCKET);
			if(goodSock)
			{
				if(sendBufferLength < m_sendLength)
				{
					goodSock = m_pClientSocket->SetSockOpt(SO_SNDBUF,&m_sendLength,optionLen,SOL_SOCKET);

				}
			} 
			// --------------------------------------------------------------------------------------


			m_pFile = new CAOISocketFile(m_pClientSocket, TRUE);

			if (m_pFile != NULL)
			{
				TRY
				{
					m_pArchiveIn = new CArchive(m_pFile, CArchive::load);
					m_pArchiveOut = new CArchive(m_pFile, CArchive::store);
					bResult = TRUE;
				}
				CATCH(CArchiveException, e)
				{
				}
				CATCH(CFileException, e)
				{
				}
				END_CATCH
			}
		}
	}

	if (!bResult)
		CleanUp();

	return bResult;
}


// Closes an established connection with the server and a message is sent to
// the server. OnDisconnection callback will be call with the value
// NDKNormalDisconnection.
void CNDKClient::CloseConnection()
{
	SendMessageToServer(CNDKMessage(NDKClientDisconnect));

	CloseConnection(NDKClient_NormalDisconnection);
}


// Sends a message to the server. If a problem occurs, the connection will
// be closed and OnDisconnect callback will be called with the value 
// NDKErrorSendingMessage.
BOOL CNDKClient::SendMessageToServer(CNDKMessage& message)
{
	BOOL bResult = FALSE;
	
	if (m_pArchiveOut != NULL)
	{
		TRY
		{
			message.Serialize(*m_pArchiveOut);
			m_pArchiveOut->Flush();
			bResult = TRUE;
		}
		CATCH(CFileException, e)
		{
			m_pArchiveOut->Abort();
			
			CloseConnection(NDKClient_ErrorSendingMessage);
		}
		END_CATCH
	}

	return bResult;
}


// Pings the server. OnPing callback will be called when the client
// receives the response from the server. If a problem occurs, the 
// connection will be closed and OnDisconnect callback will be called with 
// the value NDKErrorSendingMessage. Overrides OnPing in the derived class if
// PingServer is used.
BOOL CNDKClient::PingServer()
{
	CNDKMessage message(NDKPingServer);
	message.Add((long)GetTickCount());

	return SendMessageToServer(message);
}


////////////////////////////////////////////////////////////////////////////////
// Protected Operations                                                       //
////////////////////////////////////////////////////////////////////////////////

// Called when the ping from the server is received. The number of
// milliseconds is returned since PingServer was called
void CNDKClient::OnPing(long lNbMilliseconds)
{
	ASSERT(FALSE); // Overrides this method in the derived class.
}


////////////////////////////////////////////////////////////////////////////////
// Private Operations                                                         //
////////////////////////////////////////////////////////////////////////////////

// Processes pending read. If a problem occurs, the connection will
// be closed and OnDisconnect callback will be called with the value 
// NDKErrorReceivingMessage.
BOOL CNDKClient::ProcessPendingRead(long lErrorCode)
{
	BOOL bIsConnectionOpened = TRUE;
	CNDKMessage message;

	if (lErrorCode == 0)
	{
		do
		{
			TRY
			{
				message.Serialize(*m_pArchiveIn);

				TranslateMessage(message);
			}
			CATCH(CFileException, e)
			{
				if (m_pArchiveOut != NULL)
					m_pArchiveOut->Abort();

				// Close the connection when an error occured
				CloseConnection(NDKClient_ErrorReceivingMessage);
				bIsConnectionOpened = FALSE;
			}
			END_CATCH

		} while ((m_pArchiveIn != NULL) && !m_pArchiveIn->IsBufferEmpty());
	}
	else
	{
		if (m_pArchiveOut != NULL)
			m_pArchiveOut->Abort();

		CloseConnection(NDKClient_ErrorReceivingMessage);
		bIsConnectionOpened = FALSE;
	}
	return bIsConnectionOpened;
}


// Translates message and does the appropriate task for message handled by
// the NDK.
void CNDKClient::TranslateMessage(CNDKMessage& message)
{
	switch (message.GetId())
	{
	case NDKServerClose:
		CloseConnection(NDKClient_ServerStop);
		break;

	case NDKServerDisconnectClient:
		CloseConnection(NDKClient_ServerCloseConnection);
		break;

	case NDKPingClient:
		SendMessageToServer(message);
		break;

	case NDKPingServer:
		{
			long lNbMilliseconds = 0;
			message.GetAt(0, lNbMilliseconds);
			
			OnPing(::GetTickCount() - lNbMilliseconds);
		}
		break;

	default:
		OnMessage(message);
		break;
	}
}


// Closes the connection.
void CNDKClient::CloseConnection(NDKClientDisconnection disconnectType)
{
	BOOL bIsConnected = (m_pClientSocket != NULL);

	CleanUp();

	// Call OnDisconnect only when a connection was there
	if (bIsConnected)
		OnDisconnect(disconnectType);
}


// Cleans up.
void CNDKClient::CleanUp()
{
	if (m_pArchiveOut != NULL)
	{
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}

	if (m_pArchiveIn != NULL)
	{
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
	}

	if (m_pFile != NULL)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

	if (m_pClientSocket != NULL)
	{
		m_pClientSocket->Close();

		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}