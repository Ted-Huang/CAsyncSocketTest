/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/^\
\                                                                              /
/   CAOISocketFile -- Fix NDK large message issue.                             \
\                                                            Beagle 2013/1/4   /
/                                                                              \
\_/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/

#include "stdafx.h"
#include <stddef.h>
#include "AOISocketFile.h"

void CAOISocketFile::Write(const void* lpBuf, UINT nCount)
{
	unsigned int chunksize, sent=0;

	if (m_pSocket == NULL)
	{
		AfxThrowInvalidArgException();
	}

	if (lpBuf == NULL)
	{
		AfxThrowInvalidArgException();
	}

	while (sent < nCount) {
		chunksize = 1048576;
		if (chunksize > nCount-sent) { chunksize = nCount-sent; }

		int nWritten = m_pSocket->Send((BYTE*)lpBuf+sent, chunksize);
		if (nWritten == SOCKET_ERROR)
		{
			int nError = m_pSocket->GetLastError();
			AfxThrowFileException(CFileException::genericException, nError);
		} else {
			sent += nWritten;
		}
	}
}

/* End of AOISocketFile.cpp */
