#pragma once
#include "stunattribute.h"
#include <winsock2.h>

class CStunAddressAttribute :
	public CStunAttribute
{
private:
	unsigned char m_byAddressFamily;
	unsigned short m_nPort;
	unsigned int m_nIPAddress;

public:
	CStunAddressAttribute(unsigned short nAttributeType, unsigned char byAddressFamily,
		unsigned short nPort, const char *pszIPAddress);
	CStunAddressAttribute(unsigned short nAttributeType, SOCKADDR_IN addr);

	CStunAddressAttribute (char *pBuffer);
	~CStunAddressAttribute(void);

	unsigned short GetAttributeLength ();
	unsigned short GetPort(void);
	unsigned char GetAddressFamily(void);
	unsigned int GetIPAddress ();
	void GetBuffer (char *pBuffer);
	void Initialize(void);
	string ToString ();
};
