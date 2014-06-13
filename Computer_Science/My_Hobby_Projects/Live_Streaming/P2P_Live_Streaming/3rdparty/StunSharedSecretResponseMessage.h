#pragma once
#include "stunmessage.h"

class CStunSharedSecretResponseMessage :
	public CStunMessage
{
public:
	CStunSharedSecretResponseMessage(const char *pszUsername,
									 const char *pszPassword);
	CStunSharedSecretResponseMessage(char *pBuffer);
	~CStunSharedSecretResponseMessage(void);

	bool GetPassword (const char **pszPassword);
	bool GetUsername (const char **pszUsername);
};
