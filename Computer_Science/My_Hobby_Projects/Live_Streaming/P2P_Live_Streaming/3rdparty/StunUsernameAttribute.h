#pragma once
#include "stunattribute.h"

class CStunUsernameAttribute :
	public CStunAttribute
{
private:
	char *m_pszUsername;

public:
	CStunUsernameAttribute(const char *pszUsername);
	CStunUsernameAttribute(char *pBuffer, int nDummy);

	~CStunUsernameAttribute(void);

	void GetBuffer (char *Buffer);
	const char *GetUsername ();
	string ToString ();
};
