#pragma once
#include "stunattribute.h"
#include <winsock2.h>

#define CHANGE_PORT 2
#define CHANGE_IP	4

class CStunChangeRequestAttribute :
	public CStunAttribute
{
private:
	unsigned int m_nChangeFlag;

public:
	CStunChangeRequestAttribute(unsigned int nChangeFlag);
	CStunChangeRequestAttribute (char *pBuffer);

	~CStunChangeRequestAttribute(void);

	void AddChangeRequestAttribute(unsigned int nChangeFlag);
	
	void GetBuffer (char* pBuffer);
	
	string ToString ();
public:
	unsigned int GetChangeRequestFlag(void);
};
