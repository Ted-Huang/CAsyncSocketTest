/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/^\
\                                                                              /
/   CAOISocketFile -- Fix NDK large message issue.                             \
\                                                            Beagle 2013/1/4   /
/                                                                              \
\_/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/

#pragma once
#include <afxsock.h>

class CAOISocketFile : public CSocketFile
{
//	DECLARE_DYNAMIC(CAOISocketFile)
public:
	explicit CAOISocketFile(CSocket* pSocket, BOOL bArchiveCompatible = TRUE) :
		CSocketFile(pSocket, bArchiveCompatible) {};
	virtual void Write(const void* lpBuf, UINT nCount);
};

/* End of AOISocketFile.h */
