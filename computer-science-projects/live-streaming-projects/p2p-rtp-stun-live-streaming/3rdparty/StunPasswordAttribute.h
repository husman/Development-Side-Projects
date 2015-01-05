#pragma once
#include "stunattribute.h"

class CStunPasswordAttribute :
	public CStunAttribute
{
private:
	char *m_pszPassword;

public:
	CStunPasswordAttribute(const char *pszPassword);
	CStunPasswordAttribute(char *pBuffer, int nDummy);

	~CStunPasswordAttribute(void);

	void GetBuffer (char *Buffer);
	const char *GetPassword ();
	string ToString ();
};
